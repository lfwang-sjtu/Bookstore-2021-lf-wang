//
// Created by Snoopy on 2021/2/10.
//

#include "Book.h"

Book::Book(char *isbn)
:quantity(0), price(0)
{
    strcmp(this->ISBN, isbn);
    strcmp(this->name, "NULL");
    strcmp(this->author, "NULL");
    strcmp(this->keyword, "NULL");
}

void Book::modifyISBN(char *update) {
    strcmp(this->ISBN, update);
}
void Book::modifyName(char *update) {
    strcmp(this->name, update);
}
void Book::modifyAuthor(char *update) {
    strcmp(this->author, update);
}
void Book::modifyKeyword(char *update) {
    strcmp(this->keyword, update);
}
void Book::modifyPrice(double update) {
    this->price = update;
}

void Book::import(int qImport) {
    this->quantity += qImport;
}

void Book::show() {
    cout << this->ISBN << ' ' << this->name << ' ' << this->author << ' ' << this->keyword << ' ' << setprecision(2) << this->price << ' ' << this->quantity << endl;
}

void Book::sell(int qSell) {
    this->quantity -= qSell;
}