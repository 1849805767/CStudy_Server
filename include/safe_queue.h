#ifndef _SAFE_QUEUE_H
#define _SAFE_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <shared_mutex>

template <typename _Tp, typename _Sequence = std::deque<_Tp>> class SafeQueue {
  public:
    using value_type = _Tp;
    using container_type = _Sequence;
    using size_type = typename std::queue<value_type>::size_type;
    using reference = typename std::queue<value_type>::reference;
    using const_reference = typename std::queue<value_type>::const_reference;

  private:
    std::queue<value_type, container_type> m_queue; //利用模板函数构造队列
    std::shared_timed_mutex m_stm;                  // 访问互斥信号量
    std::condition_variable_any m_cva;              // 信号量

  public:
    // 0代表没有上限
    SafeQueue() {}
    SafeQueue(SafeQueue &&other) = delete;
    ~SafeQueue() = default;

    bool empty() {                                            // 返回队列是否为空
        std::shared_lock<std::shared_timed_mutex> lck(m_stm); // 互斥信号变量加锁，防止m_queue被改变
        return m_queue.empty();
    }

    size_type size() {
        std::shared_lock<std::shared_timed_mutex> lck(m_stm); // 互斥信号变量加锁，防止m_queue被改变
        return m_queue.size();
    }

    bool enqueue(const value_type &t) { // 队列添加元素
        std::unique_lock<std::shared_timed_mutex> lck(m_stm);
        m_queue.emplace(t);
        m_cva.notify_one();
        return true;
    }

    bool enqueue(value_type &&t) { // 队列添加元素
        std::unique_lock<std::shared_timed_mutex> lck(m_stm);
        m_queue.emplace(std::move(t));
        m_cva.notify_one();
        return true;
    }

    template <typename G> bool dequeue(value_type &t, const G &timeout) { // 队列取出元素
        std::unique_lock<std::shared_timed_mutex> lck(m_stm);             // 队列加锁
        bool waitResult{true};
        if (timeout < timeout.zero()) {
            m_cva.wait(lck, [this] { return !m_queue.empty(); });
        } else {
            waitResult = m_cva.wait_for(lck, timeout, [this] { return !m_queue.empty(); }); // false:超时, true:收到信号 或 谓词成立
        }
        if (waitResult) {
            t = std::move(m_queue.front());
            m_queue.pop();
            return true;
        }
        return false;
    }
};

#endif