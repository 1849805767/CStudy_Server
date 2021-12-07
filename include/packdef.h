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
#define _SERVER_IP "0.0.0.0"
#define _SERVER_PORT 12346
// mysql
#define _MYSQL_SERVER_IP "127.0.0.1"
#define _MYSQL_SERVER_PORT 3306
#define _MYSQL_USER "root"
#define _MYSQL_PASSWORD "200076"
#define _MYSQL_DB "CStudy"
#define _MYSQL_POOL_NUM 13
// thread_pool
#define _THREAD_POOL_SIZE 12

#define _LISTEN 128
#define _IPSIZE 16
#define _COUNT 10
#define _TIMEOUT 10
#define _SQLIEN 400
#define _EPOLLSIZE 4096

enum class PackType : char { // 协议包类型
    NONE,
    REGISTER_RQ,
    REGISTER_RS,
    LOGIN_RQ,
    LOGIN_RS,
};

enum class Result_Register : char { // 注册请求结果
    NONE,
    register_success,
    account_is_exist,
};

enum class Result_Login : char { // 登录请求结果
    NONE,
    login_success,
    account_no_exist,
    password_error,
    user_online,
};

////////////////////////////////////// 网络 //////////////////////////////////////
#define MAX_SIZE (60)

//登录请求
struct STRU_LOGIN_RQ {
    STRU_LOGIN_RQ() {
        m_nType = PackType::REGISTER_RQ;
        memset(m_szAccount, 0, MAX_SIZE);
        memset(m_szPassword, 0, MAX_SIZE);
    }

    PackType m_nType;            //包类型
    char m_szAccount[MAX_SIZE];     //用户ID
    char m_szPassword[MAX_SIZE]; //密码
    friend std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RQ &rq);
};

//登录回复
struct STRU_LOGIN_RS {
    STRU_LOGIN_RS() {
        m_nType = PackType::LOGIN_RS;
        m_UserID = 0;
        m_lResult = Result_Login::NONE;
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
        memset(m_szAccount, 0, MAX_SIZE);
        memset(m_szPasswd, 0, MAX_SIZE);
        memset(m_szName, 0, MAX_SIZE);
        memset(m_szClassName, 0, MAX_SIZE);
    }

    PackType m_nType;             // 包类型
    char m_szAccount[MAX_SIZE];   // 账户
    char m_szPasswd[MAX_SIZE];    // 密码
    char m_szName[MAX_SIZE];      // 名字
    char m_szClassName[MAX_SIZE];   // 班级
    friend std::ostream &operator<<(std::ostream &os, const STRU_REGISTER_RQ &rq);
};

//注册回复
struct STRU_REGISTER_RS {
    STRU_REGISTER_RS() {
        m_nType = PackType::REGISTER_RS;
        m_lResult = Result_Register::NONE;
    }

    PackType m_nType;          //包类型
    Result_Register m_lResult; //注册结果
    friend std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RS &rs);
};

#endif
