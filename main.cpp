#include <iostream>
#include "hash_table.hpp"
#include "semaphores_sheared_mem.hpp"
#include "list.hpp"
//#include "hash_function.hpp"

using namespace std;

#define sh_mem_p1 sh_mem_p1
#define sh_mem_p2 sh_mem_p2

void P1(int algorithm,int q,int bucketsNo,int frames);
void P2(int algorithm,int q,int bucketsNo,int frames);
int returnHash(char* s,int MAX_LEN);

void lru(int q,int bucketsno,int frames,string file_);

int main(int argc, char const *argv[]) {
    int q=100000;
    int frames=100; //
    P1(1,q,1000,frames);
    P2(1,q,1000,frames);

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
    string bzip_file="bzip.trace";
    if(algorithm==1)
        lru(q,bucketsNo,frames/2,bzip_file);
}


void P2(int algorithm,int q,int bucketsNo,int frames){
    string gcc_file="gcc.trace";
    if(algorithm==1)
        lru(q,bucketsNo,frames/2,gcc_file);

}

void lru(int q,int bucketsNo,int frames,string file_){
    hash_table table(bucketsNo);
    listPg *oldest_page=new listPg;
    string address_num ;
    ifstream file(file_);
    int hash_num;
    stringstream hex_num;
    string line, addrStr, rl;
    clock_t timer;
    int frame_counter=0;
    for (int i = 0; i < q; i++) {

        char address[10];
        address[6]='\0';
        char role[2];
        role[1]='\0';

        getline(file, line);
        address_num.assign(line.begin(),line.begin()+5);
        rl.assign(line.end()-1,line.end());
        timer=clock();

        strcpy(address,address_num.c_str());
        strcpy(role,rl.c_str());

        Page* page= new Page(address,role,timer);
        oldest_page->push_back(page);
        #if DEBUG>=3
            oldest_page->print();
        #endif
        if(rl=="W")
            table.write_counter++;
        else
            table.read_counter++;
        hash_num=returnHash(address,bucketsNo);

        if(frame_counter<frames){
            if(table.table[hash_num]->find_replace(page)==0){
                table.table[hash_num]->page->push_back(page);
                table.page_faults++;
                frame_counter++;
            }
        }else{
            if(table.table[hash_num]->find_replace(page)==0){//If find the same page we will replace
                //Else we replace the oldest page
                #if DEBUG>=3
                    cout<< "old page address: " << oldest_page->head->r->address<<endl;
                #endif
                int hash= returnHash(oldest_page->head->r->address,bucketsNo);
                table.table[hash]->replace_lru(oldest_page->head->r,page);
                table.page_faults++;
                oldest_page->delete_first();

            }
        }

    }
    #if DEBUG>=1
        table.print();
    #endif

    cout << "~~~~~~~~~~~Stats~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< table.write_counter<<endl;
    cout << "Page read: "<< table.read_counter<<endl;
    cout << "Page faults: "<< table.page_faults<<endl;
    cout << "frames: "   << frame_counter<<endl;

}

int returnHash(char *s,int MAX_LEN)
{
    // A simple hashing, no collision handled
    int sum=0,index=0;
    if(strlen(s)>6){
        std::cout << "Error on char* " << '\n';
    }
    for(unsigned long int i=0; i < strlen(s); i++)
    {
        sum += s[i];
    }
    index = sum % MAX_LEN;
    return index;
}
