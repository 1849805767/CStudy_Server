#ifndef _PACKDEF_H
#define _PACKDEF_H

#include <arpa/inet.h>
#include <cctype>
#include <cerrno>
#include <iostream>
#include <list>
#include <malloc.h>
#include <map>
#include <pthread.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "user.h"
#include "jwt-cpp/base.h"
#include "jwt-cpp/jwt.h"

// server
#define _SERVER_IP "0.0.0.0"
#define _SERVER_PORT 12345
// file
#define _FILE_BASE_PATH "static/contest"
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
#define _TIMEOUT 50
#define _EPOLLSIZE 4096

#define MAX_SIZE (30) // 协议包each字段大小，默认60
#define MIN_SIZE (10)

enum class PackType : char { // 协议包类型
    NONE,
    REGISTER_RQ, REGISTER_RS,   // 注册
    LOGIN_RQ, LOGIN_RS,         // 登录
    USER_ALL_GET_RQ, USER_ALL_GET_RS,   // 获取全部用户
    USER_GET_RQ, USER_GET_RS,           // 获取某用户信息
    USER_CTL_RQ, USER_CTL_RS,           // 删、改 某用户
    CONTEST_ALL_GET_RQ, CONTEST_ALL_GET_RS,         // 获取全部考试
    CONTEST_CTL_RQ, CONTEST_CTL_RS,                 // 获取某考试信息
    CONTEST_PROBLEM_GET_RQ, CONTEST_PROBLEM_GET_RS, // 增、删、改 某场考试

};

struct STRU_BASE {
    STRU_BASE() {
        m_Type = PackType::NONE;
    }

    PackType m_Type; // 包类型
    char m_Token[256]{}; // Token, 除了注册 login, 全都需要token进行验证
    friend std::ostream &operator<<(std::ostream &os, const STRU_BASE &base);
};

// REGISTER_RQ, REGISTER_RS,   // 注册
struct STRU_REGISTER_RQ : STRU_BASE { //注册请求
    STRU_REGISTER_RQ() {
        m_Type = PackType::REGISTER_RQ;
    }

    char m_Account[MAX_SIZE]{};   // 账户
    char m_Passwd[MAX_SIZE]{};    // 密码
    char m_Name[MAX_SIZE]{};      // 名字
    char m_ClassName[MAX_SIZE]{};   // 班级
    friend std::ostream &operator<<(std::ostream &os, const STRU_REGISTER_RQ &rq);
};

enum class Result_Register : char { // 注册请求结果
    NONE, register_success, account_is_exist,
};

struct STRU_REGISTER_RS : STRU_BASE { //注册回复
    STRU_REGISTER_RS() {
        m_Type = PackType::REGISTER_RS;
        m_Result = Result_Register::NONE;
    }

    Result_Register m_Result; //注册结果
    friend std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RS &rs);
};

// LOGIN_RQ, LOGIN_RS,         // 登录
struct STRU_LOGIN_RQ : STRU_BASE { //登录请求
    STRU_LOGIN_RQ() {
        m_Type = PackType::LOGIN_RQ;
    }

    char m_Account[MAX_SIZE]{};     // 用户ID
    char m_Passwd[MAX_SIZE]{};    // 密码

    friend std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RQ &rq);
};

enum class Result_Login : char { // 登录请求结果
    NONE, login_success, account_no_exist, password_error, user_online,
};

struct STRU_LOGIN_RS : STRU_BASE { //登录回复
    STRU_LOGIN_RS() {
        m_Type = PackType::LOGIN_RS;
        m_Result = Result_Login::NONE;
    }

    Result_Login m_Result; //注册结果
    int m_UserID{};

//    friend std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RS &rs);
    friend std::ostream &operator<<(std::ostream &os, const STRU_LOGIN_RS &rs);
};

// USER_ALL_GET_RQ, USER_ALL_GET_RS,   // 获取全部用户
struct STRU_USER_ALL_GET_RQ : STRU_BASE { // 获取全部用户请求
    STRU_USER_ALL_GET_RQ() {
        m_Type = PackType::USER_ALL_GET_RQ;
    }

    int m_Size{}; // 请求数量上限
    friend std::ostream &operator<<(std::ostream &os, const STRU_USER_ALL_GET_RQ &rq);
};

enum class Result_STRU_USER_ALL : char { // 获取全部用户请求结果
    NONE, success, error,
};

struct STRU_USER_ALL_GET_RS : STRU_BASE { // 获取全部用户回复
    STRU_USER_ALL_GET_RS() {
        m_Type = PackType::USER_ALL_GET_RS;
        m_Result = Result_STRU_USER_ALL::NONE;
    }

    Result_STRU_USER_ALL m_Result;
    int m_Size{}; // 用户数量
    char m_User_account[MAX_SIZE][MAX_SIZE]{};
    char m_User_name[MAX_SIZE][MAX_SIZE]{};
    char m_User_classname[MAX_SIZE][MAX_SIZE]{};

    friend std::ostream &operator<<(std::ostream &os, const STRU_USER_ALL_GET_RS &rs);
};

// USER_GET_RQ, USER_GET_RS,           // 获取某用户信息
struct STRU_USER_GET_RQ : STRU_BASE {
    STRU_USER_GET_RQ() {
        m_Type = PackType::USER_GET_RQ;
    }

    char m_account[MAX_SIZE]{};

    friend std::ostream &operator<<(std::ostream &os, const STRU_USER_GET_RQ &rq);
};
enum class Result_USER_GET {
    NONE, success, error, empty
};
struct STRU_USER_GET_RS : STRU_BASE {
    STRU_USER_GET_RS() {
        m_Type = PackType::USER_GET_RS;
        m_Result = Result_USER_GET::NONE;
    }

    Result_USER_GET m_Result;
    char m_account[MAX_SIZE]{};
    char m_passwd[MAX_SIZE]{};
    char m_name[MAX_SIZE]{};
    char m_classname[MAX_SIZE]{};

    friend std::ostream &operator<<(std::ostream &os, const STRU_USER_GET_RS &rs);
};

// USER_CTL_RQ, USER_CTL_RS,           // 删、改 某用户
struct STRU_USER_CTL_RQ : STRU_BASE {
    STRU_USER_CTL_RQ() {
        m_Type = PackType::USER_CTL_RQ;
        m_Op = Op::NONE;
    }

    enum class Op : char {
        NONE, add, del, update
    } m_Op;
    char m_account[MAX_SIZE]{};
    char m_passwd[MAX_SIZE]{};
    char m_name[MAX_SIZE]{};
    char m_classname[MAX_SIZE]{};

    friend std::ostream &operator<<(std::ostream &os, const STRU_USER_CTL_RQ &rq);
};
enum class Result_USER_CTL {
    NONE, success, error
};
struct STRU_USER_CTL_RS : STRU_BASE {
    STRU_USER_CTL_RS() {
        m_Type = PackType::USER_CTL_RS;
        m_Result = Result_USER_CTL::NONE;
    }

    Result_USER_CTL m_Result;

    friend std::ostream &operator<<(std::ostream &os, const STRU_USER_CTL_RS &rs);
};

// CONTEST_ALL_GET_RQ, CONTEST_ALL_GET_RS,         // 获取全部考试
struct STRU_CONTEST_ALL_GET_RQ : STRU_BASE {
    STRU_CONTEST_ALL_GET_RQ() {
        m_Type = PackType::CONTEST_ALL_GET_RQ;
    }

    int m_size{};
    int m_offset{};

    friend std::ostream &operator<<(std::ostream &os, const STRU_CONTEST_ALL_GET_RQ &rq);
};
enum class Result_CONTEST_ALL_GET {
    NONE, success, error
};
struct STRU_CONTEST_ALL_GET_RS : STRU_BASE {
    STRU_CONTEST_ALL_GET_RS() {
        m_Type = PackType::CONTEST_ALL_GET_RS;
        m_Result = Result_CONTEST_ALL_GET::NONE;
    }

    Result_CONTEST_ALL_GET m_Result;
    int m_size{};  // empty:-1
    char m_name[MAX_SIZE][MAX_SIZE]{};
    char m_creator[MAX_SIZE][MAX_SIZE]{};

    friend std::ostream &operator<<(std::ostream &os, const STRU_CONTEST_ALL_GET_RS &rs);
};

#endif
