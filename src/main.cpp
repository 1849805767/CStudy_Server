#include "TCPKernel.h"

// server
const char *_SERVER_IP = "0.0.0.0";
const int _SERVER_PORT = 12346;
// mysql
const char *_MYSQL_SERVER_IP = "127.0.0.1";
const int _MYSQL_SERVER_PORT = 3306;
const char *_MYSQL_USER = "root";
const char *_MYSQL_PASSWORD = "200076";
const char *_MYSQL_DB = "cstudy";
const int _MYSQL_POOL_NUM = 12;
// thread_pool
const int _THREAD_POOL_SIZE = 10;

const int _LISTEN = 128; // 监听数量
const int _IPSIZE = 16;
const int _COUNT = 10;
const int _TIMEOUT = 10;
const int _SQLIEN = 400;

int main(int argc, char *argv[]) {
  TcpKernel *pKernel = new TcpKernel;

  if (pKernel->Open()) {
    while (1) {
      printf("Server Running...\n");
      sleep(_TIMEOUT * 3);
    }
  }
  pKernel->Close();
  delete pKernel;
  return 0;
}
