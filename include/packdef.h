#ifndef _PACKDEF_H
#define _PACKDEF_H
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include <list>
#include <malloc.h>
#include <map>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// server
extern const char *_SERVER_IP;
extern const int _SERVER_PORT;
// mysql
extern const char *_MYSQL_SERVER_IP;
extern const int _MYSQL_SERVER_PORT;
extern const char *_MYSQL_USER;
extern const char *_MYSQL_PASSWORD;
extern const char *_MYSQL_DB;
extern const int _MYSQL_POOL_NUM;
// thread_pool
extern const int _THREAD_POOL_SIZE;

extern const int _LISTEN;
extern const int _IPSIZE;
extern const int _COUNT;
extern const int _TIMEOUT;
extern const int _SQLIEN;
#define _EPOLLSIZE 4096

enum class PackType : char { // 协议包类型
  NONE,
  REGISTER_RQ,
  REGISTER_RS,
  LOGIN_RQ,
  LOGIN_RS,
};

enum class Result_Register : char { // 注册请求结果
  userid_is_exist,
  register_sucess,
};

enum class Result_Login : char { // 登录请求结果
  userid_no_exist,
  password_error,
  login_sucess,
  user_online,
};

////////////////////////////////////// 网络 //////////////////////////////////////
#define MAX_SIZE (60)

//登录请求
struct STRU_LOGIN_RQ {
  STRU_LOGIN_RQ() {
    m_nType = PackType::REGISTER_RQ;
    memset(m_szUser, 0, MAX_SIZE);
    memset(m_szPassword, 0, MAX_SIZE);
  }

  PackType m_nType;            //包类型
  char m_szUser[MAX_SIZE];     //用户ID
  char m_szPassword[MAX_SIZE]; //密码
  friend std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RQ &rq);
};

//登录回复
struct STRU_LOGIN_RS {
  STRU_LOGIN_RS() {
    m_nType = PackType::LOGIN_RS;
    m_UserID = 0;
    m_lResult = Result_Login::login_sucess;
  }
  PackType m_nType; //包类型
  int m_UserID;
  Result_Login m_lResult; //注册结果
  friend std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RS &rs);
};

//注册请求
struct STRU_REGISTER_RQ {
  STRU_REGISTER_RQ() {
    m_nType = PackType::REGISTER_RQ;
    memset(m_szUser, 0, MAX_SIZE);
    memset(m_szPassword, 0, MAX_SIZE);
  }

  PackType m_nType;            //包类型
  char m_szUser[MAX_SIZE];     //用户名
  char m_szPassword[MAX_SIZE]; //密码

  friend std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RQ &rq);
};

//注册回复
struct STRU_REGISTER_RS {
  STRU_REGISTER_RS() {
    m_nType = PackType::REGISTER_RS;
    m_lResult = Result_Register::register_sucess;
  }
  PackType m_nType;          //包类型
  Result_Register m_lResult; //注册结果
  friend std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RS &rs);
};

#endif
