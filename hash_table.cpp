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


bucket::bucket(){
    this->empty=true;
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
hash_table::hash_table(int bucketsNo_){
    this->write_counter=0;
    this->read_counter=0;
    this->page_faults=0;
    this->bucketsNo=bucketsNo_; //how many buckets we have
    this->table = new bucket*[bucketsNo_];
    for (int i = 0; i < bucketsNo; i++) {
        this->table[i]=new bucket;
    }

}


void bucket::replace_lru(Page *pg_old,Page *pg_new){
    page->replace_lru(pg_old,pg_new);
}
