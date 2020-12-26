#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "list.hpp"

using namespace std;

#define DEBUG 3

class bucket{
public:
    bool empty;
    int bucketsz;
    bucket();
    listPg *page;
    int find_replace(Page *item);
    void replace_lru(Page *pg_old,Page *pg_new);
    void print();
};

class hash_table{
public:
    bucket **table;
    int page_faults;
    int write_counter;
    int read_counter;
    int bucketsNo;
    hash_table(int bucketsNo);

    void print();
};

#endif
