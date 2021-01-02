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

#define DEBUG 2

class bucket{
public:
    int bucketsz;
    bucket();
    listPg *page;
    int find_replace(Page *item,listPg *oldest_page,int alg,listPg *write_back);
    void print();
};

class hash_table{
public:
    bucket **table;
    listPg *write_back;
    int page_faults;
    int write_counter;
    int read_counter;
    int bucketsNo;
    hash_table(int bucketsNo);

    void print();
};

#endif
