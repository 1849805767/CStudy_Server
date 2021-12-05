#ifndef MYSQL_POOL_H
#define MYSQL_POOL_H

#include <algorithm>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <string>

#include <odb/core.hxx>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>

class mysql_pool {
  class A {};

private:
  std::string m_pool_name; // 连接池名称
  std::string m_host;      // 数据库ip
  unsigned int m_port;     // 数据库端口
  std::string m_user;      // 用户名
  std::string m_passwd;    // 用户密码
  std::string m_db;        // db名称
  int m_conn_cnt;          // 连接数量

  std::list<odb::database *> m_free_list; // 空闲的连接
  std::mutex m_mutex;
  std::condition_variable m_cond;

public:
  mysql_pool() = delete;
  mysql_pool(const mysql_pool &) = delete;
  mysql_pool &operator=(const mysql_pool &buffer) = delete;
  mysql_pool(mysql_pool &&) = delete;
  mysql_pool &operator=(mysql_pool &&buffer) = delete;

  mysql_pool(const std::string &pool_name, const std::string &db_server_ip, unsigned int db_server_port, const std::string &user, const std::string &password,
             const std::string &db_name, int conn_cnt);
  virtual ~mysql_pool();
  odb::database *get_connection(); // 获取连接资源
  void revert(odb::database *db);  // 归还连接资源

  const char *get_pool_name() { return m_pool_name.c_str(); }
  const char *get_host() { return m_host.c_str(); }
  uint16_t get_port() { return m_port; }
  const char *get_user() { return m_user.c_str(); }
  const char *get_passwd() { return m_passwd.c_str(); }
  const char *get_db() { return m_db.c_str(); }
};

#endif