#include "TCPKernel.h"
#include "packdef.h"

#include "jwt-cpp/base.h"
#include "jwt-cpp/jwt.h"

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "user.h"
#include "user-odb.hxx"
#include "contest.h"
#include "contest-odb.hxx"


using namespace std;

static const ProtocolMap m_ProtocolMapEntries[] = {{PackType::REGISTER_RQ, &TcpKernel::RegisterRq},
                                                   {PackType::LOGIN_RQ, &TcpKernel::LoginRq},
                                                   {PackType::USER_ALL_GET_RQ, &TcpKernel::AllUserRq},
                                                   {PackType::USER_GET_RQ, &TcpKernel::UserGet},
                                                   {PackType::USER_CTL_RQ, &TcpKernel::UserCtl},
                                                   {PackType::CONTEST_ALL_GET_RQ, &TcpKernel::ContestAllGet},
                                                   {PackType::NONE, nullptr}};

TcpKernel::TcpKernel() : m_mysql_pool("mysql_pool", _MYSQL_SERVER_IP, _MYSQL_SERVER_PORT, _MYSQL_USER, _MYSQL_PASSWORD,
                                      _MYSQL_DB, _MYSQL_POOL_NUM) {
    m_tcp = new TcpNet(this);
}

int TcpKernel::Open() {
    if (!m_tcp) {
        m_tcp = new TcpNet(this);
    }
    if (!m_tcp->InitNetWork()) {
        printf("InitNetWork Failed...\n");
        return false;
    } else {
        printf("Init Net Success...\n");
    }

    return true;
}

void TcpKernel::Close() {
    if (!m_tcp) return;
    m_tcp->UnInitNetWork();
}

void TcpKernel::DealData(int client, char *buf, int len) {
    if (!m_tcp) return;
    auto *pBase = (STRU_BASE *) buf;
    if (pBase->m_Type != PackType::REGISTER_RQ && pBase->m_Type != PackType::LOGIN_RQ) {
        auto decoded = jwt::decode(pBase->m_Token);
        auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{"MySecret"});
        error_code err;
        verifier.verify(decoded, err);
        if (err) {
            printf("[token] : %s\n", err.message().c_str());
            return;
        }
    }
    int i = 0;
    while (true) {
        if (pBase->m_Type == m_ProtocolMapEntries[i].m_type) {
            PFUN fun = m_ProtocolMapEntries[i].m_pfun;
            (this->*fun)(client, buf);
        } else if (m_ProtocolMapEntries[i].m_type == PackType::NONE && m_ProtocolMapEntries[i].m_pfun == nullptr)
            return;
        ++i;
    }
}

// 注册
void TcpKernel::RegisterRq(int client, char *buf) {
    if (!m_tcp) return;
    auto *rq = (STRU_REGISTER_RQ *) buf;
    STRU_REGISTER_RS rs;

    cout << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        if (db->query<user>(odb::query<user>::account == rq->m_Account).empty()) {
            user user(rq->m_Account, rq->m_Passwd, rq->m_Name, rq->m_ClassName);
            db->persist(user);
            rs.m_Result = Result_Register::register_success;
        } else {
            rs.m_Result = Result_Register::account_is_exist;
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    {
        auto token = jwt::create()
                .set_payload_claim("account", jwt::claim(string(rq->m_Account)))
                .sign(jwt::algorithm::hs256{"MySecret"});
        copy(token.begin(), token.end(), rs.m_Token);
        cout << "token:" << token << endl;
    }
    m_tcp->SendData(client, (char *) &rs, sizeof(rs));
}

// 登录
void TcpKernel::LoginRq(int client, char *buf) {
    if (!m_tcp) return;
    auto *rq = (STRU_LOGIN_RQ *) buf;
    STRU_LOGIN_RS rs;
    cout << "rq:" << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());
        printf("---------1------\n");
        auto resultUser = db->query<user>(odb::query<user>::account == rq->m_Account);
        printf("----------2-----\n");
        if (resultUser.empty()) {
            rs.m_Result = Result_Login::account_no_exist;
        } else if (resultUser.begin()->getPasswd() == rq->m_Passwd) {
            rs.m_Result = Result_Login::login_success;
            rs.m_UserID = resultUser.begin()->getId();
        } else {
            rs.m_Result = Result_Login::password_error;
        }
        ts.commit();
        m_mysql_pool.revert(db);
        printf("---------3------\n");
    }
    {
        auto token = jwt::create()
                .set_payload_claim("account", jwt::claim(string(rq->m_Account)))
                .sign(jwt::algorithm::hs256{"MySecret"});
        copy(token.begin(), token.end(), rs.m_Token);
        cout << "token:" << token << endl;
    }
    m_tcp->SendData(client, (char *) &rs, sizeof(rs));
}

void TcpKernel::AllUserRq(int client, char *buf) { // 获取全部用户
    if (!m_tcp) return;
    auto *rq = (STRU_USER_ALL_GET_RQ *) buf;
    STRU_USER_ALL_GET_RS rs;
    cout << "rq:" << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        auto resultUser = db->query<user>();
        rs.m_Result = Result_STRU_USER_ALL::success;

        rs.m_Size = std::min(MIN_SIZE, (int) resultUser.size());
        auto ite(resultUser.begin());
        for (int i = 0; i < rs.m_Size; ++i, ++ite) {
            copy(ite->getAccount().begin(), ite->getAccount().end(), rs.m_User_account[i]);
            copy(ite->getName().begin(), ite->getName().end(), rs.m_User_name[i]);
            copy(ite->getClassname().begin(), ite->getClassname().end(), rs.m_User_classname[i]);
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    m_tcp->SendData(client, (char *) &rs, sizeof(rs));
}

void TcpKernel::UserGet(int client, char *buf) {
    if (!m_tcp) return;
    auto *rq = (STRU_USER_GET_RQ *) buf;
    STRU_USER_GET_RS rs;
    cout << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        auto u = db->query_one<user>(odb::query<user>::account == rq->m_account);
        if (u) {
            rs.m_Result = Result_USER_GET::success;
            copy(u->getAccount().begin(), u->getAccount().end(), rs.m_account);
            copy(u->getPasswd().begin(), u->getPasswd().end(), rs.m_passwd);
            copy(u->getName().begin(), u->getName().end(), rs.m_name);
            copy(u->getClassname().begin(), u->getClassname().end(), rs.m_classname);
        } else {
            rs.m_Result = Result_USER_GET::empty;
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    m_tcp->SendData(client, (char *) &rs, sizeof(rs));
}

void TcpKernel::UserCtl(int client, char *buf) {
    if (!m_tcp) return;
    auto *rq = (STRU_USER_CTL_RQ *) buf;
    STRU_USER_CTL_RS rs;
    cout << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        switch (rq->m_Op) {
            case STRU_USER_CTL_RQ::Op::update:{
                auto u = db->query_one<user>(odb::query<user>::account == rq->m_account);
                if (u) {
                    cout << "user:"<<*u<<endl;
                    u->setAccount(rq->m_account);
                    u->setClassname(rq->m_classname);
                    u->setName(rq->m_name);
                    cout << "user:"<<*u<<endl;
                    db->update(u);
                    cout << "user:"<<*u<<endl;
                    rs.m_Result = Result_USER_CTL::success;
                } else {
                    rs.m_Result = Result_USER_CTL::error;
                }
                break;
            }
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    m_tcp->SendData(client, (char *) &rs, sizeof(rs));
}

void TcpKernel::ContestAllGet(int client, char *buf) {
    if (!m_tcp) return;
    auto *rq = (STRU_CONTEST_ALL_GET_RQ *) buf;
    STRU_CONTEST_ALL_GET_RS rs;
    cout << "rq:" << *rq << endl;
    {
        odb::database *db = m_mysql_pool.get_connection();
        odb::transaction ts(db->begin());

        auto resultContest = db->query<contest>();
        rs.m_Result = Result_CONTEST_ALL_GET::success;

        rs.m_size = std::min(MIN_SIZE, (int) resultContest.size());
        auto ite(resultContest.begin());
        for (int i = 0; i < rs.m_size; ++i, ++ite) {
            copy(ite->getName().begin(), ite->getName().end(), rs.m_name[i]);
            copy(ite->getCreator().begin(), ite->getCreator().end(), rs.m_creator[i]);
        }

        ts.commit();
        m_mysql_pool.revert(db);
    }
    m_tcp->SendData(client, (char *) &rs, sizeof(rs));
}
