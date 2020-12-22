#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include "hash_table.hpp"
#include <vector>

using namespace std;

Page::Page(int _address,string _role,clock_t _time){
    this->address=_address;
    this->role=_role;
    this->t=_time;
}

void Page::print()
{
    cout<<"Address: "<<address<<endl;
    cout<<"Role: "<<role<<endl;
}
bucket::bucket(){
    full=false;
}

void bucket::print(){
    cout << "~~~~~~PRINT BUCKET~~~~~~~\n";
    for(int a = 0; a < page.size(); a ++){
            cout<< "bucket: "<<a<<endl;
            page[a]->print();
    }
}

hash_table::hash_table(int bucketsNo){
    this->write_counter=0;
    this->read_counter=0;
    for (int i = 0; i < bucketsNo; i++) {
        table[i]=new bucket;
    }
}

int hash_table::find_position(int hash_num,Page* pg){
    // for(int a = 0; a < table[hash_num]->page.size(); a ++){
    //     if(table[hash_num]->page[a]==pg){
    //         return a;
    //     }
    // }
    return -1;
}
