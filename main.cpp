#include <iostream>
#include "hash_table.hpp"
#include "semaphores_sheared_mem.hpp"
#include "list.hpp"
//#include "hash_function.hpp"

using namespace std;

#define sh_mem_p1 sh_mem_p1
#define sh_mem_p2 sh_mem_p2

void P1(int algorithm,int q,int bucketsNo,int frames);
void P2(int algorithm,int q,int bucketsno,hash_table *table);
int returnHash(const string s,int MAX_LEN);

void lru(int q,int bucketsno,int frames);

int main(int argc, char const *argv[]) {
    int q=1000000;
    int frames=10; //
    P1(1,q,20,frames);

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

void P1(int algorithm,int q,int bucketsNo,int frames){
    if(algorithm==1)
        lru(q,bucketsNo,frames/2);
}


void P2(int algorithm,int q,int bucketsno,hash_table *table){
    ifstream gcc_file("gcc.trace");

}

void lru(int q,int bucketsNo,int frames){
    hash_table table(bucketsNo);
    listPg *oldest_page=new listPg;
    string address_num ;
    ifstream bzip_file("bzip.trace");
    int hash_num;
    stringstream hex_num;
    string line, addrStr, role;
    clock_t timer;
    int frame_counter=0;
    for (int i = 0; i < q; i++) {
        getline(bzip_file, line);
        address_num.assign(line.begin(),line.begin()+5);
        role.assign(line.end()-1,line.end());
        timer=clock();

        //hex_num<<addrStr;
        // hash_index_num>>hex>>address_num;
        Page* page= new Page(address_num,role,timer);
        oldest_page->push_back(page);

        if(role=="W")
            table.write_counter++;
        else
            table.read_counter++;
        hash_num=returnHash(address_num,bucketsNo);

        if(frame_counter<frames){
            if(table.table[hash_num]->find_replace(page)==0){
                table.table[hash_num]->page->push_back(page);
                table.page_faults++;
                frame_counter++;
            }

        }else{
            if(table.table[hash_num]->find_replace(page)==0){//If find the same page we will replace
                //Else we replace the oldest page
                int hash=0;
                //string temp_address=(string)oldest_page->head->r->address;
                cout <<  oldest_page->return_address();
                //hash= returnHash(temp_address,bucketsNo);
                table.table[hash]->replace_lru(oldest_page->head->r,page);
                table.page_faults++;
                oldest_page->delete_first();

            }
        }

    }
    cout << "~~~~~~~~~~~Stats~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< table.write_counter<<endl;
    cout << "Page read: "<< table.read_counter<<endl;
    cout << "Page faults: "<< table.page_faults<<endl;
    cout << "frames: "   << frame_counter<<endl;

    //table.print();

}

int returnHash(const string s,int MAX_LEN)
{
    // A simple hashing, no collision handled
    int sum=0,index=0;
    for(unsigned long int i=0; i < s.length(); i++)
    {
        sum += s[i];
    }
    index = sum % MAX_LEN;
    return index;
}
