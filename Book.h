//
// Created by Snoopy on 2021/2/10.
//

#ifndef BOOKSTORE_2021_BOOK_H
#define BOOKSTORE_2021_BOOK_H

#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

const int ISBN_maxLength = 20;
const int bookName_maxlength = 60;
const int author_maxlength = 60;
const int keyword_maxlength = 60;

class Book{
private:
    char ISBN[ISBN_maxLength + 1], name[bookName_maxlength + 1], author[author_maxlength + 1], keyword[keyword_maxlength + 1];
    int quantity;
    double price;
public:
    Book(char *isbn);

    void modifyISBN(char *update);
    void modifyName(char *update);
    void modifyAuthor(char *update);
    void modifyKeyword(char *update);
    void modifyPrice(double update);

    void import(int qImport);

    void show();

    void sell(int qSell);
};

#endif //BOOKSTORE_2021_BOOK_H
