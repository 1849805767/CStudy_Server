#include <TCPKernel.h>
#include <TCPNet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

TcpNet *TcpNet::m_pThis = 0;

TcpNet::TcpNet(IKernel *kernel) {
    m_kernel = kernel;
    m_pThis = this;
}

TcpNet::~TcpNet() { delete m_threadpool; }

int TcpNet::InitNetWork() {
    m_threadpool = new ThreadPool(_THREAD_POOL_SIZE);
    bzero(&m_sockaddr_server, sizeof(m_sockaddr_server));
    m_sockaddr_server.sin_family = AF_INET;
    if (inet_pton(AF_INET, _SERVER_IP, &m_sockaddr_server.sin_addr.s_addr) == -1) {
        perror("Init Ip Error:");
        return false;
    }
    m_sockaddr_server.sin_port = htons(_SERVER_PORT);
    //创建Socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Create Socket Error:");
        return false;
    }
    int mw_optval;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *) &mw_optval, sizeof(mw_optval));
    // 设置非阻塞
    setnonblocking(listenfd);
    //绑定端口号
    if (bind(listenfd, (struct sockaddr *) &m_sockaddr_server, sizeof(m_sockaddr_server)) == -1) {
        perror("Bind Socket Error:");
        return false;
    }
    //监听socket
    if (listen(listenfd, _LISTEN) == -1) {
        perror("Listen Error:");
        return false;
    }
    epfd = epoll_create(_EPOLLSIZE); //创建epoll的句柄，可以监听的文件描述符为_DEF_EPOLLSIZE

    Addfd(listenfd, true);
    m_threadpool->init();
    m_threadpool->submit(EPOLL_Jobs);
    return true;
}

void TcpNet::Addfd(int fd, int enable_et /*是否为边缘触发*/) {
    struct epoll_event eptemp;
    eptemp.events = EPOLLIN;
    eptemp.data.fd = fd;
    if (enable_et)
        eptemp.events |= EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &eptemp);
}

void TcpNet::Deletefd(int fd) { epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0); }

void TcpNet::EPOLL_Jobs() {
    int ready;
    int i = 0;
    while (1) {
        // 阻塞-1监听socket
        printf("%d\n", i++);
        if ((ready = epoll_wait(m_pThis->epfd, m_pThis->epollarr, _EPOLLSIZE, -1)) == -1) {
            perror("Epoll Call Failed:"); //出错
            return;
        }
        m_pThis->Epoll_Deal(ready);
        bzero(m_pThis->epollarr, sizeof(epollarr));
    }
}

void TcpNet::Epoll_Deal(int ready) {
    for (int i = 0; i < ready; i++) {
        int fd = epollarr[i].data.fd;
        if (listenfd == fd) { // 客户端建立链接
            m_pThis->m_threadpool->submit(Accept_Deal);
            // todo: 加入心跳池
        } else if (epollarr[i].events & EPOLLIN) {
            Deletefd(fd);
            m_pThis->m_threadpool->submit(Info_Recv, fd);
        }
    }
}

void TcpNet::Accept_Deal() {
    sockaddr_in clientaddr;
    socklen_t clientsize = sizeof(clientaddr);
    int clientfd;
    char ipstr[_IPSIZE];
    while (true) {
        clientfd = accept(m_pThis->listenfd, (sockaddr *) &clientaddr, &clientsize);
        if (clientfd == -1 && errno == EAGAIN) {
            printf("0x%x accept没了\n", std::this_thread::get_id());
            break;
        } else if (clientfd == -1) {
            perror("accept failed!");
            return;
        }
        m_pThis->Addfd(clientfd, true);
        printf("Custom Thread TID:0x%x\tClient Ip:%s\tClient PORT:%d\t\n", std::this_thread::get_id(),
               inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipstr, sizeof(ipstr)),
               ntohs(clientaddr.sin_port));
    }
}

void TcpNet::Info_Recv(int clientfd) {
    int nPackSize = 0;
    int nRelReadNum = recv(clientfd, &nPackSize, sizeof(nPackSize), 0);
    if (nRelReadNum <= 0) {
        close(clientfd);
        return;
    }
    char *pSzBuf = new char[nPackSize];
    int nOffSet = 0;
    nRelReadNum = 0;
    //接收包的数据
    while (nPackSize) {
        nRelReadNum = recv(clientfd, pSzBuf + nOffSet, nPackSize, 0);
        if (nRelReadNum > 0) {
            nOffSet += nRelReadNum;
            nPackSize -= nRelReadNum;
        }
    }
//    printf("pszbuf = %p \n", pSzBuf);
    m_pThis->m_kernel->DealData(clientfd, pSzBuf, nOffSet);
    m_pThis->Addfd(clientfd, true);
    delete[]pSzBuf;
}

int TcpNet::SendData(int clientfd, char *szbuf, int nlen) {
    if (send(clientfd, (const char *) &nlen, sizeof(int), 0) < 0)
        return false;
    if (send(clientfd, szbuf, nlen, 0) < 0)
        return false;
    return true;
}

void TcpNet::UnInitNetWork() { close(listenfd); }

IKernel::~IKernel() {

}
