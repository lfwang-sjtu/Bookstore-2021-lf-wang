//
// Created by Snoopy on 2021/3/1.
//

#include "BlockList.h"

//todo checked
vector<int> BlockList::find(const string &_key) const {
    ifstream infile(fileName, ios::in | ios::binary);
    if (!infile){ cerr << "open file error\n";}
    Block block;
    vector<int> all_offsets, tmp;

    infile.seekg(0, ios::beg);
    infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
    while (true){
        if (block.size == 0){
            if (block.next == -1) break;
            else{
                infile.seekg(block.next, ios::beg);
                infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
            }
        }
        else{
            if (_key >= block.front() && _key <= block.back()) {
                tmp = block.find(_key);
                for (int i = 0; i < tmp.size(); ++i) {
                    all_offsets.push_back(tmp[i]);
                }
            }
            if (block.next == -1) break;
            infile.seekg(block.next, ios::beg);
            infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
        }
    }
    infile.close();
    return all_offsets;
}

//todo checked
void BlockList::insert(const node &value) {
    fstream file(fileName, ios::in | ios::out | ios::binary);
    if (!file) { cerr << "open file error\n"; }
    Block block;

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char *>(&block), sizeof(Block));
    while (true){
        if (block.size == 0){
            if (block.next == -1){
                block.insert_savagely(value,0);
                file.seekp(block.position, ios::beg);
                file.write(reinterpret_cast<char *>(&block), sizeof(Block));
                break;
            }
            else{
                file.seekg(block.next, ios::beg);
                file.read(reinterpret_cast<char *>(&block), sizeof(Block));
            }
        }
        else{
            if (block.next == -1){
                if (block.size >= 295) {
                    file.close();
                    splitBlock(block.position);
                    file.open(fileName, ios::in | ios::out | ios::binary);

                    Block block1, block2;
                    file.seekg(block.position, ios::beg);
                    file.read(reinterpret_cast<char *>(&block1), sizeof(Block));
                    file.seekg(block1.next, ios::beg);
                    file.read(reinterpret_cast<char *>(&block2), sizeof(Block));

                    if (block2.front() >= value.get_key()){
                        block1.insert(value);
                        file.seekp(block1.position, ios::beg);
                        file.write(reinterpret_cast<char *>(&block1), sizeof(Block));
                        break;
                    }
                    else{
                        block2.insert(value);
                        file.seekp(block2.position, ios::beg);
                        file.write(reinterpret_cast<char *>(&block2), sizeof(Block));
                        break;
                    }
                }
                else{
                    block.insert(value);
                    file.seekp(block.position, ios::beg);
                    file.write(reinterpret_cast<char *>(&block), sizeof(Block));
                    break;
                }
            }
            else{
                Block block1;
                file.seekg(block.next, ios::beg);
                file.read(reinterpret_cast<char *>(&block1), sizeof(Block));
                if (value.get_key() >= block1.front()){
                    file.seekg(block.next, ios::beg);
                    file.read(reinterpret_cast<char *>(&block), sizeof(Block));
                }
                else{
                    if (block.size >= 295){
                        file.close();
                        splitBlock(block.position);
                        file.open(fileName, ios::in | ios::out | ios::binary);

                        Block block1, block2;
                        file.seekg(block.position, ios::beg);
                        file.read(reinterpret_cast<char *>(&block1), sizeof(Block));
                        file.seekg(block1.next, ios::beg);
                        file.read(reinterpret_cast<char *>(&block2), sizeof(Block));

                        if (block2.front() >= value.get_key()){
                            block1.insert(value);
                            file.seekp(block1.position, ios::beg);
                            file.write(reinterpret_cast<char *>(&block1), sizeof(Block));
                            break;
                        }
                        else{
                            block2.insert(value);
                            file.seekp(block2.position, ios::beg);
                            file.write(reinterpret_cast<char *>(&block2), sizeof(Block));
                            break;
                        }
                    }
                    else {
                        block.insert(value);
                        file.seekp(block.position, ios::beg);
                        file.write(reinterpret_cast<char *>(&block), sizeof(Block));
                        break;
                    }
                }
            }
        }
    }
    file.close();
}

//todo checked
void BlockList::erase(const node &value) {
    fstream file(fileName, ios::in | ios::out | ios::binary);
    if (!file) {cerr << "open file error\n";}
    Block block;

    file.seekg(0, ios::beg);
    file.read(reinterpret_cast<char *>(&block), sizeof(Block));
    while (true){
        if (block.size == 0){
            if (block.next == -1) break;
            else{
                file.seekg(block.next, ios::beg);
                file.read(reinterpret_cast<char *>(&block), sizeof(Block));
            }
        }
        else{
            if (value.get_key() >= block.front() && value.get_key() <= block.back()){
                if (block.size >= 295){
                    file.close();
                    splitBlock(block.position);
                    file.open(fileName, ios::in | ios::out | ios::binary);
                }
                block.erase(value);
                file.seekp(block.position, ios::beg);
                file.write(reinterpret_cast<char *>(&block), sizeof(Block));
                if (block.next == -1) break;
                file.read(reinterpret_cast<char *>(&block), sizeof(Block));
            }
        }
    }
    ///如果什么都没有删，要不要给一个Invalid？
    file.close();
}

//todo checked
void BlockList::splitBlock(int _position) {
    fstream file(fileName, ios::in | ios::out | ios::binary);
    long position1, position2;
    Block toSplit, newBlock;

    position1 = _position;
    file.seekp(0, ios::end);
    position2 = file.tellp();

    file.seekg(_position, ios::beg);
    file.read(reinterpret_cast<char *>(&toSplit), sizeof(Block));

    for (int i = 150; i < toSplit.size; ++i) {
        newBlock.data[i - 150] = toSplit.data[i];
    }
    newBlock.size = toSplit.size - 150;
    newBlock.position = position2;
    newBlock.next = toSplit.next;

    toSplit.size = 150;
    toSplit.next = position2;

    file.seekp(position1, ios::beg);
    file.write(reinterpret_cast<char *>(&toSplit), sizeof(Block));
    file.seekp(position2, ios::beg);
    file.write(reinterpret_cast<char *>(&toSplit), sizeof(Block));

    file.close();
}

//todo checked
bool BlockList::isExist(const string &_key) const{
    ifstream infile(fileName, ios::in | ios::binary);
    Block block;

    infile.seekg(0, ios::beg);
    infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
    while (true){
        if (block.size == 0){
            if (block.next == -1) break;
            else{
                infile.seekg(block.next, ios::beg);
                infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
            }
        }
        else{
            if (_key >= block.front() && _key <= block.back()){
                if (block.isExist(_key)){
                    infile.close();
                    return true;
                }
            }
            if (block.next == -1) break;
            infile.seekg(block.next, ios::beg);
            infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
        }
    }
    infile.close();
    return false;
}

void BlockList::printBlockList() const{
    ifstream infile(fileName, ios::in | ios::binary);
    Block block;
    infile.seekg(0, ios::beg);
    infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
    while (true){
        block.printBlock();
        if (block.next == -1) break;
        infile.seekg(block.next, ios::beg);
        infile.read(reinterpret_cast<char *>(&block), sizeof(Block));
    }
    infile.close();
}