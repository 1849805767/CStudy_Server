#include "mysql_pool.h"

/**
 * @param pool_name 连接池名字
 * @param db_server_ip 数据库server IP
 * @param db_server_port 数据库port
 * @param pool_usernamename 用户名
 * @param password 密码
 * @param db_name 数据库名
 * @param conn_cnt 总连接数量
 */
mysql_pool::mysql_pool(const std::string &pool_name, const std::string &db_server_ip, unsigned int db_server_port,
                       const std::string &username, const std::string &password,
                       const std::string &db_name, int conn_cnt) {
    m_pool_name = pool_name;
    m_host = db_server_ip;
    m_port = db_server_port;
    m_user = username;
    m_passwd = password;
    m_db = db_name;
    m_conn_cnt = conn_cnt;

    for (int i = 0; i < m_conn_cnt; ++i) {
        m_free_list.emplace_back(new odb::mysql::database(m_user, m_passwd, m_db, m_host, m_port));
    }
    printf("mysql连接池: %s, size: %ld\n", m_pool_name.c_str(), m_free_list.size());
}

mysql_pool::~mysql_pool() {
    printf("~CDBPool\n");
    for (auto db: m_free_list) {
        printf("delete %p\n", db);
        delete db;
    }
    m_free_list.clear();
}

/**
 * @brief 从连接池中获取一个连接
 */
odb::database *mysql_pool::get_connection() {
    odb::database *db = nullptr;
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_cond.wait(lck, [this] { return !m_free_list.empty(); });
        db = m_free_list.front(); // 获取连接
        m_free_list.pop_front();  // STL吐出连接，从空闲队列删除
    }
    return db;
}

/**
 * @brief 放回连接池，同一个连接不允许多次放回。
 */
void mysql_pool::revert(odb::database *db) {
    std::unique_lock<std::mutex> lck(m_mutex);
    m_free_list.emplace_back(db);
    m_cond.notify_one();
}