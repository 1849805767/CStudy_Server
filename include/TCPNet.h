#ifndef _TCPNET_H
#define _TCPNET_H

#include "packdef.h"
#include "thread_pool.h"
#include <mutex>

class IKernel {
public:
  virtual void DealData(int, char *, int) = 0;
};

class TcpNet {
public:
  TcpNet(IKernel *kernel);
  ~TcpNet();
  int InitNetWork();
  void UnInitNetWork();
  static void EPOLL_Jobs();
  static void Accept_Deal();
  static void Info_Recv(int clientfd);
  int SendData(int, char *, int);
  void Addfd(int, int);
  void Deletefd(int);
  void Epoll_Deal(int);

public:
  epoll_event epollarr[_EPOLLSIZE];
  int listenfd; // listen 端口
  int epfd;

  sockaddr_in m_sockaddr_server;
  IKernel *m_kernel; // 与上层kernel交互

  ThreadPool *m_threadpool;
  static TcpNet *m_pThis; // 给线程池中静态函数用
};

#endif
