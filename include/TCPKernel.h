#ifndef TCPKERNEL_H
#define TCPKERNEL_H

#include <algorithm>

#include "TCPNet.h"
#include "mysql_pool.h"

class TcpKernel;

typedef void (TcpKernel::*PFUN)(int, char *);

typedef struct {
    PackType m_type;
    PFUN m_pfun;
} ProtocolMap;

class TcpKernel : public IKernel {
public:
    TcpKernel();

    int Open();

    void Close();

    void DealData(int clientfd, char *szbuf, int nlen) override;

    void RegisterRq(int client, char *buf);   // 注册
    void LoginRq(int client, char *buf);      // 登录
    void AllUserRq(int client, char *buf);    // 获取全部用户
    void UserGet(int client, char *buf);      // 获取某用户信息
    void UserCtl(int client, char *buf);
    void ContestAllGet(int client, char *buf);

private:
    TcpNet *m_tcp{};
    mysql_pool m_mysql_pool;
};

#endif // TCPKERNEL_H
