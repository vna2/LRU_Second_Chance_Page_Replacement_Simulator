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

using namespace std;

class Page{
public:
    Page(char* _address, char* _role, clock_t _time);
    void print();
    char address[6];
    int address_num;
    char role[2];
    clock_t t;
    bool Second_chance;

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
    void delete_item(Page *value);
    node * return_first_item();
    int find_replace(Page *pg,listPg *oldest_page);
    int find(Page *pg);
    void replace_lru(Page *pg_old,Page *pg_new,listPg *oldest_page);

};
#endif
