//
// Created by Snoopy on 2021/2/10.
//

#include "User.h"

User::User(const string &id, const string &passwd, const string &name, int l) : level(l){
    strncpy(this->userID, id.c_str(), ID_maxLength);
    strncpy(this->passWord, passwd.c_str(), passwd_maxLength);
    strncpy(this->name, name.c_str(), name_maxLength);
}

string User::getID() {
    return string(this->userID);
}

string User::getName() {
    return string(this->name);
}

string User::getPassWord() {
    return string(this->passWord);
}

int User::getLevel() {
    return this->level;
}

void User::modify_passwd(const char *newPasswd) {
    strncpy(this->passWord, newPasswd, passwd_maxLength);
}