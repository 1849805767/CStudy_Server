#include "TCPKernel.h"

#include <cstdio>
#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "packdef.h"
#include "User.h"
#include "User-odb.hxx"

using namespace std;

static const ProtocolMap m_ProtocolMapEntries[] = {{PackType::REGISTER_RQ, &TcpKernel::RegisterRq},
                                                   {PackType::LOGIN_RQ,    &TcpKernel::LoginRq},
                                                   {PackType::NONE,        0}};

TcpKernel::TcpKernel() : m_mysql_pool("mysql_pool", _MYSQL_SERVER_IP, _MYSQL_SERVER_PORT, _MYSQL_USER, _MYSQL_PASSWORD,
                                      _MYSQL_DB, _MYSQL_POOL_NUM) {}

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
    PackType *pType = (PackType *) szbuf;
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

    STRU_REGISTER_RQ *rq = (STRU_REGISTER_RQ *) szbuf;
    STRU_REGISTER_RS rs;

    cout << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        if (db->query<User>(odb::query<User>::account == rq->m_szAccount).size() == 0) {
            User user(rq->m_szAccount, rq->m_szPasswd, rq->m_szName, rq->m_szClassName);
            db->persist(user);
            rs.m_lResult = Result_Register::register_success;
        } else {
            rs.m_lResult = Result_Register::account_is_exist;
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    m_tcp->SendData(clientfd, (char *) &rs, sizeof(rs));
}

// 登录
void TcpKernel::LoginRq(int clientfd, char *szbuf, int nlen) {
    printf("clientfd:%d LoginRq\n", clientfd);

    STRU_LOGIN_RQ *rq = (STRU_LOGIN_RQ *) szbuf;
    STRU_LOGIN_RS rs;
    cout << "rq:" << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        auto resultUser = db->query<User>(odb::query<User>::account == rq->m_szAccount);
        if (resultUser.size()) {
            cout << "res.pass:" << resultUser.begin()->getPasswd() << ", rq.m_szPass:" << rq->m_szPassword << endl;
            cout << "==:" << (resultUser.begin()->getPasswd() == rq->m_szPassword) << endl;
            cout << "==:" << (resultUser.begin()->getPasswd().compare(rq->m_szPassword)) << endl;
        } else {
            cout << "empty" << endl;
        }

        if (resultUser.size() == 0) {     /// ! 不要用empty，始终返回true
            rs.m_lResult = Result_Login::account_no_exist;
        } else if (resultUser.begin()->getPasswd() == rq->m_szPassword) {

            rs.m_lResult = Result_Login::login_success;
        } else {
            rs.m_lResult = Result_Login::password_error;
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    cout << rs << endl;
    m_tcp->SendData(clientfd, (char *) &rs, sizeof(rs));
}
