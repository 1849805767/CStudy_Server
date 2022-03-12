//
// Created by q on 2021/12/30.
//

#ifndef CSTUDY_SERVER_CONTEST_H
#define CSTUDY_SERVER_CONTEST_H

#include <odb/core.hxx>
#include <string>
#include <ostream>

#pragma db object

#pragma db table("contests")
class contest {
    contest() {}

public:
    int getId() const {
        return _id;
    }

    void setId(int id) {
        _id = id;
    }

    const std::string &getName() const {
        return _name;
    }

    void setName(const std::string &name) {
        _name = name;
    }

    const std::string &getCreator() const {
        return _creator;
    }

    void setCreator(const std::string &creator) {
        _creator = creator;
    }

    friend std::ostream &operator<<(std::ostream &os, const contest &contest) {
        os << "_id: " << contest._id << " _name: " << contest._name << " _creator: " << contest._creator;
        return os;
    }

private:
    friend odb::access;

#pragma db id auto
    int _id{};
#pragma db column("name") type("varchar(30)")
    std::string _name{};   //
#pragma db column("creator") type("varchar(30)")
    std::string _creator{};    //
// path = /static/contest/creator/name
};

#endif //CSTUDY_SERVER_CONTEST_H

/**
 *  odb -d mysql --generate-query --generate-schema --output-dir odb user.h
 *  mysql --user=root --password=200076 --database=CStudy < odb/contest.sql
 */