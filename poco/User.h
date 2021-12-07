//
// Created by q on 2021/12/6.
//

#ifndef CSTUDY_SERVER_USER_H
#define CSTUDY_SERVER_USER_H

#include <odb/core.hxx>
#include <string>
#include <ostream>

#pragma db object

class User {
public:
    User(const std::string &account, const std::string &passwd, const std::string &name, const std::string &classname)
            : _account(account), _passwd(passwd), _name(name), _classname(classname) {}

    int getId() const {
        return _id;
    }

    void setId(int id) {
        _id = id;
    }

    const std::string &getAccount() const {
        return _account;
    }

    void setAccount(const std::string &account) {
        _account = account;
    }

    const std::string &getPasswd() const {
        return _passwd;
    }

    void setPasswd(const std::string &passwd) {
        _passwd = passwd;
    }

    const std::string &getName() const {
        return _name;
    }

    void setName(const std::string &name) {
        _name = name;
    }

    const std::string &getClassname() const {
        return _classname;
    }

    void setClassname(const std::string &classname) {
        _classname = classname;
    }

    friend std::ostream &operator<<(std::ostream &os, const User &user) {
        os << "_id: " << user._id << " _account: " << user._account << " _passwd: " << user._passwd << " _name: "
           << user._name << " _classname: " << user._classname;
        return os;
    }

private:
    friend odb::access;

    User() {}

#pragma db id auto
    int _id;
#pragma db column("account") type("varchar(30)")
    std::string _account;   // 账户
#pragma db column("passwd") type("varchar(30)")
    std::string _passwd;    // 密码
#pragma db column("name") type("varchar(30)")
    std::string _name;      // 名字
#pragma db column("classname") type("varchar(30)")
    std::string _classname;     // 班级
};

#endif //CSTUDY_SERVER_USER_H

/**
 *  odb -d mysql --generate-query --generate-schema --output-dir odb User.h
 *  mysql --user=root --password=200076 --database=CStudy < odb/User.sql
 */