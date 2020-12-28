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
    page= new listPg;

}

int bucket::find_replace(Page *item,listPg *oldest_page,int alg){

    return page->find_replace(item,oldest_page,alg);
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
