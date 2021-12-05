#include "packdef.h"

std::ostream &operator<<(std::ostream &out, PackType pt) {
    switch (pt) {
    case PackType::NONE:
        out << "NONE";
        break;
    case PackType::REGISTER_RQ:
        out << "REGISTER_RQ";
        break;
    case PackType::REGISTER_RS:
        out << "REGISTER_RS";
        break;
    case PackType::LOGIN_RQ:
        out << "LOGIN_RQ";
        break;
    case PackType::LOGIN_RS:
        out << "LOGIN_RS";
        break;
    default:;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, Result_Register pt) {
    switch (pt) {
    case Result_Register::userid_is_exist:
        out << "userid_is_exist";
        break;
    case Result_Register::register_sucess:
        out << "register_sucess";
        break;
    default:;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, Result_Login pt) {
    switch (pt) {
    case Result_Login::userid_no_exist:
        out << "userid_no_exist";
        break;
    case Result_Login::password_error:
        out << "password_error";
        break;
    case Result_Login::login_sucess:
        out << "login_sucess";
        break;
    case Result_Login::user_online:
        out << "user_online";
        break;
    default:;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RQ &rq) {
    out << "m_nType:" << rq.m_nType << ",m_szUser:" << rq.m_szUser << ",m_szPassword:" << rq.m_szPassword;
    return out;
}
std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RS &rs) {
    out << "m_nType:" << rs.m_nType << ",m_UserID:" << rs.m_UserID << ",m_lResult:" << rs.m_lResult;
    return out;
}
std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RQ &rq) {
    out << "m_nType:" << rq.m_nType << ",m_szUser:" << rq.m_szUser << ",m_szPassword:" << rq.m_szPassword;
    return out;
}
std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RS &rs) {
    out << "m_nType:" << rs.m_nType << ",m_lResult:" << rs.m_lResult;
    return out;
}