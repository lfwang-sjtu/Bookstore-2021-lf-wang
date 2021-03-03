#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cassert>
#include "User.h"
#include "Book.h"
#include "BlockList.h"

using namespace std;

vector<User> loginSystem;

void initialize(){
    fstream myFile("_user.index", ios::in | ios::binary);
    if (!myFile){

        myFile.open("_user.data", ios::out | ios::binary);
        myFile.seekp(0, ios::beg);
        node boss_node("root", myFile.tellp());
        User boss_user("root", "sjtu", "root", 7);
        myFile.write(reinterpret_cast<char *>(&boss_user), sizeof(User));
        myFile.close();

        myFile.open("_user.index", ios::out | ios::binary);
        Block block;
        block.insert_savagely(boss_node,0);
        block.setSize(1);
        myFile.seekp(0, ios::beg);
        block.setPosition(myFile.tellp());
        myFile.write(reinterpret_cast<char *>(&block), sizeof(Block));
        myFile.close();
    }
    myFile.close();
}

//todo checked
void su_(stringstream &sstream, vector<User> &loginSystem){
    string id, passwd;
    sstream >> id >> passwd;

    //todo 读取ID为id的user
    BlockList user_index("_user.index");
    vector<int> user_offset = user_index.find(id);
    if (user_offset.size() != 1) {cerr << "different people using the same ID\n";}

    User user;
    ifstream user_data("_user.data", ios::in | ios::binary);
    user_data.seekg(user_offset[0], ios::beg);
    user_data.read(reinterpret_cast<char *>(&user), sizeof(User));

    if (passwd == "") {
        //todo 检查ID为id的用户的等级，比较当前登录的用户的等级，决定是否可以登录
        if (!loginSystem.empty() && user.getLevel() < loginSystem.back().getLevel()) {
            loginSystem.push_back(user);
            user_data.close();
            return;
        }
        else {user_data.close(); cerr << "can't login without password\n";}
    }

    //todo 检查id与passwd是否匹配正确
    if (passwd == user.getPassWord()) {
        loginSystem.push_back(user);
        user_data.close();
        return;
    }
    else {user_data.close(); cerr << "wrong password\n"; return;}
}

void useradd_(stringstream &sstream, vector<User> &loginSystem){
    string id, passwd, name;
    int level;
    sstream >> id >> passwd >> level >> name;
    if (loginSystem.empty() || loginSystem.back().getLevel() <= level){
        cerr<< "have no right to add user of this level\n";
        return;
    }
    else{
        //todo 如果ID为id的用户已经存在，不可创建，否则建立新的用户
        BlockList user_index("_user.index");
        if (user_index.isExist(id)){
            cerr << "this ID has been signed in\n";
            return;
        }
        else{
            fstream user_data("_user.data", ios::in | ios::out | ios::binary);
            user_data.seekp(0, ios::end);
            User user(id, passwd, name, level);
            node newNode(id, user_data.tellp());
            user_data.write(reinterpret_cast<char *>(&user), sizeof(User));
            user_data.close();
            user_index.insert(newNode);
            return;
        }
    }
}

//register 110110110 000 snoopy
void register_(stringstream &sstream, vector<User> &loginSystem){
    string id, passwd, name;
    sstream >> id >> passwd >> name;
    BlockList user_index("_user.index");
    if (user_index.isExist(id)){
        cerr << "this ID has been signed in\n";
        return;
    }
    else{
        fstream user_data("_user.data", ios::in | ios::out | ios::binary);
        user_data.seekp(0, ios::end);
        User user(id, passwd, name, 1);
        node newNode(id, user_data.tellp());
        user_data.write(reinterpret_cast<char *>(&user), sizeof(User));
        user_data.close();
        user_index.insert(newNode);
        return;
    }
}

void delete_(stringstream &sstream, vector<User> &loginSystem){
    string id;
    sstream >> id;
    if (!loginSystem.empty() && loginSystem[loginSystem.size() - 1].getLevel() == 7){
        BlockList user_index("_user.index");
        vector<int> single_offset = user_index.find(id);
        if (single_offset.size() != 1) {cerr << "different people using the same ID\n";}
        node toDelete(id, single_offset[0]);
        user_index.erase(toDelete);
        User user;
        fstream file("_user.data", ios::in | ios::out | ios::binary);
        file.seekp(toDelete.get_offset(), ios::beg);
        file.write(reinterpret_cast<char *>(&user), sizeof(User));
        file.close();
    }
    else throw "nope";
}

void passwd_(stringstream &sstream, vector<User> &loginSystem){
    string id, old_passwd, new_passwd;
    if (!loginSystem.empty() && loginSystem[loginSystem.size() - 1].getLevel() == 7){
        sstream >> id >> new_passwd;
        BlockList user_index("_user.index");
        User user;
        vector<int> single_offset = user_index.find(id);
        if (single_offset.size() != 1) {cerr << "different people using the same ID\n";}

        fstream file("_user.data", ios::in | ios::out | ios::binary);
        file.seekg(single_offset[0], ios::beg);
        file.read(reinterpret_cast<char *>(&user), sizeof(User));
    }
    else{
        sstream >> id >> old_passwd >> new_passwd;
    }
}

void print_user_data(){
    ifstream user_data("_user.data", ios::in | ios::binary);
    User user;
    user_data.read(reinterpret_cast<char *>(&user), sizeof(User));
    while (!user_data.eof()){
        user.show();
        user_data.read(reinterpret_cast<char *>(&user), sizeof(User));
    }
}

void print_user_index(){
    BlockList toPrint("_user.index");
    toPrint.printBlockList();
}

void execute(stringstream &sstream){
    string command;
    sstream >> command;
    if (command == "su"){su_(sstream, loginSystem);}
    if (command == "logout"){loginSystem.pop_back();}
    if (command == "useradd"){useradd_(sstream, loginSystem);}
    if (command == "register"){register_(sstream, loginSystem);}
    if (command == "delete"){delete_(sstream, loginSystem);}
    if (command == "passwd"){passwd_(sstream, loginSystem);}
    if (command == "select"){}
    if (command == "modify"){}
    if (command == "print_user_data"){print_user_data();}
    if (command == "print_user_index"){print_user_index();}
    if (command == "exit" || command == "quit") {exit(0);}
}

int main() {
    initialize();
    string cmd;
    stringstream sstream;
    while (getline(cin,cmd)){
        try {
            sstream.clear();
            sstream << cmd;
            execute(sstream);
        }
        catch (...) {
            cout << "Invalid\n";
        }
    }
    return 0;
}