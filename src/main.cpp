#include "TCPKernel.h"

int main(int argc, char *argv[]) {
    TcpKernel *pKernel = new TcpKernel;

    if (pKernel->Open()) {
        while (1) {
            printf("Server Running...\n");
            sleep(_TIMEOUT * 30);
        }
    }
    pKernel->Close();
    delete pKernel;
    return 0;
}
