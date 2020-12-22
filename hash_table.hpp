#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

#include "hash_function.hpp"

class Page{
public:
    Page(int _address, string _role, clock_t _time);
    void print();
    int address;
    string role;
    clock_t t;
};

class bucket{
public:
    bool full;
    bucket();
    list<Page*> page;        //pointer gia selida
    int find(int address);
    void print();
};

class hash_table{
public:
    bucket **table;
    int write_counter;
    int read_counter;
    hash_table(int bucketsNo);
    int find_position(int hash_num,Page* item);
};


#endif
