#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include "hash_table.hpp"
#include "algorithm"
#include <vector>

using namespace std;


bucket::bucket(int sz){
    this->empty=true;
    this->full=false;
    this->bucketsz=sz;
    page= new listPg;

}

int bucket::find_replace(Page *item){
    return page->find_replace(item);
}

void hash_table::print(){
    cout << bucketsNo<<endl;
    cout << "~~~~~~PRINT BUCKET~~~~~~~\n";
    for(int a = 0; a < bucketsNo; a ++){
            cout<< "bucket: "<<a<<endl;
            table[a]->print();
    }
}

void bucket::print(){
    page->print();
}
class bucket;
hash_table::hash_table(int bucketsNo_,int bucketsz){
    this->write_counter=0;
    this->read_counter=0;
    this->page_faults=0;
    this->bucketsNo=bucketsNo_; //how many buckets we have
    cout<< this->bucketsNo;
    this->table = new bucket*[bucketsNo_];
    for (int i = 0; i < bucketsNo; i++) {
        this->table[i]=new bucket(bucketsz);
    }

}

int hash_table::find_replace(int hash_num,Page *pg){
    return table[hash_num]->find_replace(pg);
}

void bucket::replace_lru(Page *pg){
    page->replace_lru(pg);
}

void hash_table::replace_lru(int hash_num,Page *pg){
    table[hash_num]->replace_lru(pg);
}
