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
    write_back= 0;
    for (int i = 0; i < bucketsNo; i++) {
        this->table[i]=new bucket;
    }

}

int returnHash(char *s,int MAX_LEN)
{
    // A simple hashing, no collision handled
    int sum=0,index=0;

    for(unsigned long int i=0; i < strlen(s); i++)
    {
        sum += s[i];
    }
    index = sum % MAX_LEN;
    return index;
}

int hash_index(int s,int MAX_LEN){
    return s % MAX_LEN;
}

unsigned int hex2int(char *hex) {
    uint32_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}
