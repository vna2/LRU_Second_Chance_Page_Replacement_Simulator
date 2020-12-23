#include <iostream>
#include "hash_table.hpp"
#include "semaphores_sheared_mem.hpp"
#include "list.hpp"

using namespace std;

#define sh_mem_p1 sh_mem_p1
#define sh_mem_p2 sh_mem_p2

void P1(int algorithm,int q,int bucketsno);
void P2(int algorithm,int q,int bucketsno,hash_table *table);
int hash_index(int data, int bucketsno);

void lru(int q,int bucketsno);

int main(int argc, char const *argv[]) {
    int q=10000;

    P1(1,q,5);

    // int pid = fork();
    // switch(pid){
    //     case -1:
    //         perror("fork faild \n");
    //         break;
    //     case 0://this is child
    //         P1(1,q,table);
    //         exit(0);
    // }

    return 0;
}

void P1(int algorithm,int q,int bucketsno){
    if(algorithm==1)
        lru(q,bucketsno);
}


void P2(int algorithm,int q,int bucketsno,hash_table *table){
    ifstream gcc_file("gcc.trace");

}

void lru(int q,int bucketsno){
    hash_table *table =new hash_table(5,10);
    listPg *saved_page=new listPg;
    int address_num ;
    ifstream bzip_file("bzip.trace");
    int hash_num;
    stringstream hex_num;
    string line, addrStr, role;
    clock_t timer;
    clock_t min;

    for (int i = 0; i < q; i++) {
        getline(bzip_file, line);
        addrStr.assign(line.begin(),line.begin()+5);
        role.assign(line.end()-1,line.end());
        timer=clock();

        hex_num<<addrStr;
        hex_num>>hex>>address_num;
        Page* page= new Page(address_num,role,timer);

  
        if(role=="W")
            table->write_counter++;
        else
            table->read_counter++;
        hash_num=hash_index(address_num,bucketsno);

        if(table->table[hash_num]->full){
            if(table->table[hash_num]->find_replace(page)){
                table->table[hash_num]->replace_lru(page);
                table->page_faults++;
            }
        }
        else{
            table->table[hash_num]->empty=false;
            table->table[hash_num]->page->push_back(page);
            if(table->table[hash_num]->bucketsz==table->table[hash_num]->page->length)
                table->table[hash_num]->full=true;
            table->page_faults++;
        }

    }

}

int hash_index(int data, int bucketsno){
    int hash;
    hash=data%bucketsno;
    return hash;
}
