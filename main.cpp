#include <iostream>
#include "hash_table.hpp"
#include "semaphores_sheared_mem.hpp"
#include "list.hpp"
//#include "hash_function.hpp"

using namespace std;

#define sh_mem_p1 sh_mem_p1
#define sh_mem_p2 sh_mem_p2

void P1(int algorithm,int q,int bucketsNo);
void P2(int algorithm,int q,int bucketsno,hash_table *table);
int returnHash(string s,int MAX_LEN);

void lru(int q,int bucketsno);

int main(int argc, char const *argv[]) {
    int q=1000000;

    P1(1,q,100);

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

void P1(int algorithm,int q,int bucketsNo){
    if(algorithm==1)
        lru(q,bucketsNo);
}


void P2(int algorithm,int q,int bucketsno,hash_table *table){
    ifstream gcc_file("gcc.trace");

}

void lru(int q,int bucketsNo){
    hash_table table(bucketsNo,10);
    listPg *saved_pages=new listPg;
    string address_num ;
    ifstream bzip_file("bzip.trace");
    int hash_num;
    stringstream hex_num;
    string line, addrStr, role;
    clock_t timer;

    for (int i = 0; i < q; i++) {
        getline(bzip_file, line);
        address_num.assign(line.begin(),line.begin()+5);
        role.assign(line.end()-1,line.end());
        timer=clock();

        //hex_num<<addrStr;
        // hash_index_num>>hex>>address_num;
        Page* page= new Page(address_num,role,timer);


        if(role=="W")
            table.write_counter++;
        else
            table.read_counter++;
        hash_num=returnHash(address_num,bucketsNo);
        //hash_num=hash_index(address_num.c_str(),address_num.length(),bucketsno);


        if(table.table[hash_num]->full==true){
            if(table.table[hash_num]->find_replace(page)==0){
                table.table[hash_num]->replace_lru(page);
                table.page_faults++;
            }
        }
        else{
            table.table[hash_num]->empty=false;
            table.table[hash_num]->page->push_back(page);
            if(table.table[hash_num]->bucketsz==table.table[hash_num]->page->length)
                table.table[hash_num]->full=true;
            table.page_faults++;
        }

    }
    cout << "~~~~~~~~~~~Stats~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< table.write_counter<<endl;
    cout << "Page read: "<< table.read_counter<<endl;
    cout << "Page faults: "<< table.page_faults<<endl;

    //table.print();


}

int returnHash(string s,int MAX_LEN)
{
    // A simple hashing, no collision handled
    int sum=0,index=0;
    for(string::size_type i=0; i < s.length(); i++)
    {
        sum += s[i];
    }
    index = sum % MAX_LEN;
    return index;
}
