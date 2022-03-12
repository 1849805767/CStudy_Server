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
        case PackType::USER_ALL_GET_RQ:
            out << "USER_ALL_GET_RQ";
            break;
        case PackType::USER_ALL_GET_RS:
            out << "USER_ALL_GET_RS";
            break;
        case PackType::USER_GET_RQ:
            out << "USER_GET_RQ";
            break;
        case PackType::USER_GET_RS:
            out << "USER_GET_RS";
            break;
        case PackType::USER_CTL_RQ:
            out << "USER_CTL_RQ";
            break;
        case PackType::USER_CTL_RS:
            out << "USER_CTL_RS";
            break;
        case PackType::CONTEST_ALL_GET_RQ:
            out << "CONTEST_ALL_GET_RQ";
            break;
        case PackType::CONTEST_ALL_GET_RS:
            out << "CONTEST_ALL_GET_RS";
            break;
        default:;
    }
    return out;
}

std::ostream &operator<<(std::ostream &os, const STRU_BASE &base) {
    os << "m_Type: " << base.m_Type << " m_Token: " << base.m_Token;
    return os;
}

/// STRU_LOGIN
std::ostream &operator<<(std::ostream &out, const STRU_LOGIN_RQ &rq) {
    out << "m_Type:" << rq.m_Type << ",m_Account:" << rq.m_Account << ",m_Passwd:" << rq.m_Passwd;
    return out;
}

std::ostream &operator<<(std::ostream &out, Result_Login pt) {
    switch (pt) {
        case Result_Login::NONE:
            out << "NONE";
            break;
        case Result_Login::account_no_exist:
            out << "account_no_exist";
            break;
        case Result_Login::password_error:
            out << "password_error";
            break;
        case Result_Login::login_success:
            out << "login_success";
            break;
        case Result_Login::user_online:
            out << "user_online";
            break;
        default:;
    }
    return out;
}

std::ostream &operator<<(std::ostream &os, const STRU_LOGIN_RS &rs) {
    os << static_cast<const STRU_BASE &>(rs) << " m_Result: " << rs.m_Result << " m_UserID: " << rs.m_UserID;
    return os;
}

/// STRU_REGISTER
std::ostream &operator<<(std::ostream &os, const STRU_REGISTER_RQ &rq) {
    os << "m_Type: " << rq.m_Type << " m_Account: " << rq.m_Account << " m_Passwd: " << rq.m_Passwd
       << " m_Name: " << rq.m_Name << " m_ClassName: " << rq.m_ClassName;
    return os;
}

std::ostream &operator<<(std::ostream &out, Result_Register pt) {
    switch (pt) {
        case Result_Register::NONE:
            out << "NONE";
            break;
        case Result_Register::register_success:
            out << "register_success";
            break;
        case Result_Register::account_is_exist:
            out << "account_is_exist";
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const STRU_REGISTER_RS &rs) {
    out << "m_Type:" << rs.m_Type << ",m_Result:" << rs.m_Result;
    return out;
}

/// STRU_USER_ALL_GET
std::ostream &operator<<(std::ostream &os, const STRU_USER_ALL_GET_RQ &rq) {
    os << "m_Type: " << rq.m_Type << " m_Size: " << rq.m_Size;
    return os;
}

std::ostream &operator<<(std::ostream &out, Result_STRU_USER_ALL pt) {
    switch (pt) {
        case Result_STRU_USER_ALL::NONE:
            out << "NONE";
            break;
        case Result_STRU_USER_ALL::success:
            out << "success";
            break;
        case Result_STRU_USER_ALL::error:
            out << "error";
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &os, const STRU_USER_ALL_GET_RS &rs) {
    os << "m_Type: " << rs.m_Type << " m_Result: " << rs.m_Result << " m_Size: " << rs.m_Size << "{\n";
    for (int i = 0; i < rs.m_Size; ++i) {
        os << "\tm_User_account: " << rs.m_User_account[i] << " m_User_name: " << rs.m_User_name[i]
           << " m_User_classname: "
           << rs.m_User_classname[i] << "\n";
    }
    os << "}";
    return os;
}

/// USER_GET_RQ, USER_GET_RS,           // 获取某用户信息
std::ostream &operator<<(std::ostream &os, const STRU_USER_GET_RQ &rq) {
    os << "m_Type: " << rq.m_Type << " m_account: " << rq.m_account;
    return os;
}

std::ostream &operator<<(std::ostream &out, Result_USER_GET pt) {
    switch (pt) {
        case Result_USER_GET::NONE:
            out << "NONE";
            break;
        case Result_USER_GET::success:
            out << "success";
            break;
        case Result_USER_GET::error:
            out << "error";
            break;
        case Result_USER_GET::empty:
            out << "empty";
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &os, const STRU_USER_GET_RS &rs) {
    os << "m_Type: " << rs.m_Type << " m_Result: " << rs.m_Result << " m_account: " << rs.m_account << " m_passwd: "
       << rs.m_passwd << " m_name: " << rs.m_name << " m_classname: " << rs.m_classname;
    return os;
}

/// USER_CTL_RQ, USER_CTL_RS,           // 删、改 某用户
std::ostream &operator<<(std::ostream &out, STRU_USER_CTL_RQ::Op op) {
    switch (op) {
        case STRU_USER_CTL_RQ::Op::NONE:
            out << "NONE";
            break;
        case STRU_USER_CTL_RQ::Op::add:
            out << "add";
            break;
        case STRU_USER_CTL_RQ::Op::update:
            out << "update";
            break;
        case STRU_USER_CTL_RQ::Op::del:
            out << "del";
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &os, const STRU_USER_CTL_RQ &rq) {
    os << static_cast<const STRU_BASE &>(rq) << " m_Op: " << rq.m_Op << " m_account: " << rq.m_account << " m_passwd: "
       << rq.m_passwd << " m_name: " << rq.m_name << " m_classname: " << rq.m_classname;
    return os;
}

std::ostream &operator<<(std::ostream &out, Result_USER_CTL pt) {
    switch (pt) {
        case Result_USER_CTL::NONE:
            out << "NONE";
            break;
        case Result_USER_CTL::success:
            out << "success";
            break;
        case Result_USER_CTL::error:
            out << "error";
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &os, const STRU_USER_CTL_RS &rs) {
    os << static_cast<const STRU_BASE &>(rs) << " m_Result: " << rs.m_Result;
    return os;
}


/// CONTEST_ALL_GET_RQ, CONTEST_ALL_GET_RS,         // 获取全部考试
std::ostream &operator<<(std::ostream &os, const STRU_CONTEST_ALL_GET_RQ &rq) {
    os << static_cast<const STRU_BASE &>(rq) << " m_size: " << rq.m_size << " m_offset: " << rq.m_offset;
    return os;
}

std::ostream &operator<<(std::ostream &os, const STRU_CONTEST_ALL_GET_RS &rs) {
    os << static_cast<const STRU_BASE &>(rs) << " m_size: " << rs.m_size << " m_name: " << rs.m_name << " m_creator: "
       << rs.m_creator;
    return os;
}
