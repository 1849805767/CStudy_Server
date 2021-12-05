#include "TCPKernel.h"

#include "packdef.h"
#include <stdio.h>

using namespace std;

static const ProtocolMap m_ProtocolMapEntries[] = {{PackType::REGISTER_RQ, &TcpKernel::RegisterRq}, {PackType::LOGIN_RQ, &TcpKernel::LoginRq}, {PackType::NONE, 0}};
TcpKernel::TcpKernel() : m_mysql_pool("mysql_pool", _MYSQL_SERVER_IP, MYSQL_PORT, _MYSQL_USER, _MYSQL_PASSWORD, _MYSQL_DB, _MYSQL_POOL_NUM) {}
int TcpKernel::Open() {
  m_tcp = new TcpNet(this);
  if (!m_tcp->InitNetWork()) {
    printf("InitNetWork Failed...\n");
    return false;
  } else {
    printf("Init Net Success...\n");
  }

  return true;
}

void TcpKernel::Close() { m_tcp->UnInitNetWork(); }

void TcpKernel::DealData(int clientfd, char *szbuf, int nlen) {
  PackType *pType = (PackType *)szbuf;
  int i = 0;
  while (1) {
    if (*pType == m_ProtocolMapEntries[i].m_type) {
      PFUN fun = m_ProtocolMapEntries[i].m_pfun;
      (this->*fun)(clientfd, szbuf, nlen);
    } else if (m_ProtocolMapEntries[i].m_type == PackType::NONE && m_ProtocolMapEntries[i].m_pfun == 0)
      return;
    ++i;
  }
  return;
}

// 注册
void TcpKernel::RegisterRq(int clientfd, char *szbuf, int nlen) {
  printf("clientfd:%d RegisterRq\n", clientfd);

  STRU_REGISTER_RQ *rq = (STRU_REGISTER_RQ *)szbuf;
  STRU_REGISTER_RS rs;

  cout << *rq << endl;

  m_tcp->SendData(clientfd, (char *)&rs, sizeof(rs));
}

// 登录
void TcpKernel::LoginRq(int clientfd, char *szbuf, int nlen) {
  printf("clientfd:%d LoginRq\n", clientfd);

  STRU_LOGIN_RQ *rq = (STRU_LOGIN_RQ *)szbuf;
  STRU_LOGIN_RS rs;

  cout << *rq << endl;

  m_tcp->SendData(clientfd, (char *)&rs, sizeof(rs));
}
