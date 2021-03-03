//
// Created by Snoopy on 2021/3/1.
//

#ifndef BOOKSTORE_2021_BLOCKLIST_H
#define BOOKSTORE_2021_BLOCKLIST_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

class node{
private:
    char key[100];
    int offset;
public:
    node() = default;

    node(const string &_key, int _offset){
        strncpy(key, _key.c_str(),100); offset = _offset;
    }

    node(const node &other){
        strcpy(key, other.key); offset = other.offset;
    }

    node &operator=(const node &other){
        strcpy(key, other.key);
        offset = other.offset;
        return *this;
    }

    const int &get_offset() const{
        return offset;
    }
    int &get_offset(){
        return offset;
    }

    string get_key() const{
        string s_key(key);
        return s_key;
    }

    void print_node() const{cout << this->key << ' ' << this->offset << endl;}
};

class BlockList;
class Block{
    friend BlockList;
private:
    node data[300];
    int size, position, next;
public:
    Block(){
        size = 0;
        next = -1;
    }

    void setSize(int _size){size = _size;}
    void setNext(int _next){next = _next;}
    void setPosition(long _position) {position = _position;}

    const string front() const{
        if (size == 0) {cerr << "nothing in this block\n";}
        return data[0].get_key();
    }
    const string back() const{
        if (size == 0) {cerr << "nothing in this block\n";}
        return data[size - 1].get_key();
    }

    vector<int> find(const string &_key) const{
        vector<int> All_offsets;
        for (int i = 0; i < size; ++i) {
            if (data[i].get_key() == _key) {
                All_offsets.push_back(data[i].get_offset());
            }
        }
        return All_offsets;
    }

    void insert(const node &value){
        if (size == 0){
            data[0] = value;
        }
        else {
            int index = 0;
            while (value.get_key() > data[index].get_key() && index < size) ++index;
            for (int i = size; i > index; --i) {
                data[i] = data[i - 1];
            }
            data[index] = value;
        }
        ++size;
    }

    void insert_savagely(const node &value, int index){
        data[index] = value;
    }

    void erase(const node &value){
        for (int i = 0; i < size; ++i) {
            if (data[i].get_key() == value.get_key() && data[i].get_offset() == value.get_offset()){
                for (int j = i; j < size - 1; ++j) {
                    data[i] = data[i + 1];
                }
                --size;
                return;
            }
        }
    }

    bool isExist(const string &_key) const{
        for (int i = 0; i < size; ++i) {
            if (data[i].get_key() == _key) return true;
        }
        return false;
    }

    void printBlock(){
        for (int i = 0; i < size; ++i){
            data[i].print_node();
        }
    }
};

class BlockList {
private:
    string fileName;

    void splitBlock(int _position);
//    void mergeBlock();
public:
    BlockList(const string &_file_name){
        fileName = _file_name;
        ifstream infile(fileName, ios::in | ios::binary);
        if (!infile){cerr << "open file error\n";}
        infile.close();
    }
    vector<int> find(const string &_key) const;
    void insert(const node &value);
    void erase(const node &value);
    bool isExist(const string &_key) const;

    void printBlockList() const;
};

#endif //BOOKSTORE_2021_BLOCKLIST_H
