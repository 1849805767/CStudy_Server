#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H

#include "TCPNet.h"
#include "mysql_pool.h"

class TcpKernel;
typedef void (TcpKernel::*PFUN)(int, char *, int);

typedef struct {
  PackType m_type;
  PFUN m_pfun;
} ProtocolMap;

class TcpKernel : public IKernel {
public:
  TcpKernel();
  int Open();
  void Close();
  void DealData(int clientfd, char *szbuf, int nlen);

  //注册
  void RegisterRq(int clientfd, char *szbuf, int nlen);
  //登录
  void LoginRq(int clientfd, char *szbuf, int nlen);

private:
  // CMysql *m_sql;
  TcpNet *m_tcp;
  mysql_pool m_mysql_pool;
};

#endif // _TCPKERNEL_H
