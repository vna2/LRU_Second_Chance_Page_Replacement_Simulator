#include <iostream>
#include "hash_table.hpp"
#include "semaphores_sheared_mem.hpp"
using namespace std;

#define sh_mem_p1 sh_mem_p1
#define sh_mem_p2 sh_mem_p2

void P1(int algorithm,int q,hash_table *table);

int main(int argc, char const *argv[]) {
    int q=4;
    hash_table *table =new hash_table(5);
    int pid = fork();
    switch(pid){
        case -1:
            perror("fork faild \n");
            break;
        case 0://this is child
            P1(1,q,table);
            exit(0);
    }

    return 0;
}

void P1(int algorithm,int q,int bucketsno,hash_table *table){
    int address_num ;
    ifstream bzip_file("bzip.trace");
    int hash_num;
    stringstream hex_num;
    string line, addrStr, role;
    for (int i = 0; i < q; i++) {
        getline(bzip_file, line);
        addrStr.assign(line.begin(),line.begin()+5);
        role.assign(line.end()-1,line.end());
        hex_num<<addrStr;
        hex_num>>hex>>address_num;
        Page* page= new Page(address_num,role,1);
        if(role=="W")
            table->write_counter++;
        else
            table->read_counter++;
        hash_num=hash_index(address_num,bucketsno);
        if(table->find_position(hash_num,page)==-1){
            table->table[hash_num]->page.push_front(page);
        }



    }

}

void P2(int algorithm){
    ifstream gcc_file("gcc.trace");

}
