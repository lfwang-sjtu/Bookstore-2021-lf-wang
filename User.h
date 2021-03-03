//
// Created by Snoopy on 2021/2/10.
//

#ifndef BOOKSTORE_2021_USER_H
#define BOOKSTORE_2021_USER_H

#include <iostream>
#include <cstring>
using namespace std;

const int ID_maxLength = 30;
const int passwd_maxLength = 30;
const int name_maxLength = 30;

class User {
private:
    char userID[ID_maxLength + 1], passWord[passwd_maxLength + 1], name[name_maxLength + 1];
    const int level;
public:
    User(const string &id = "", const string &passwd = "", const string &name = "", int l = 1);

    string getID();
    string getPassWord();
    string getName();
    int getLevel();

    void modify_passwd(const char *newPasswd);

    void show(){cout << this->userID << ' ' << this->name << ' ' << this->passWord << ' ' << this->level << endl;}
};

#endif //BOOKSTORE_2021_USER_H
