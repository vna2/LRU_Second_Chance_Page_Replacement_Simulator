#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "list.hpp"

using namespace std;

class Page{
public:
    Page(int _address, string _role, clock_t _time);
    void print();
    int address;
    string role;
    clock_t t;
};

struct node{
    Page *r;
    node *next;

    node();
    ~node();

};

class listPg{
    public:
    node *head, *tail;
    int length;// num of items
    listPg();
    ~listPg();
    void push_back(Page *value);
    void print();
    void delete_first();
    void delete_last();
    node * return_first_item();
    int find_replace(Page *pg);
    void replace_lru(Page *pg);
    int test();

};
#endif
