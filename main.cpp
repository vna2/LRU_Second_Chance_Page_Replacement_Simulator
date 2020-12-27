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
int hash_index(int s,int MAX_LEN);
unsigned int hex2int(char *hex);

void lru(int q,int bucketsno,int frames,string file_);
void Second_chance(int q,int bucketsNo,int frames,string file_);
void cut_hex(char* mem);
int main(int argc, char const *argv[]) {
    int q=50;
    int frames=10; //
    P1(1,q,4,frames);
    //P2(1,q,10,frames);

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
    hash_table *table= new hash_table(bucketsNo);
    listPg *oldest_page=new listPg;
    unsigned int address_num=0 ;
    ifstream file;
    file.open(file_);
    int hash_num;
    int frame_counter=0;
    for (int i = 0; i < q; i++) {

        char address[10];
        address[5]='\0';
        char role[2];
        role[1]='\0';


        file >> address;
        file >> role;
        cut_hex(address);

        address_num= hex2int(address);

        Page* page= new Page(address,address_num,role,i);


        if(strcmp(role,"W")==0)
            table->write_counter++;
        else
            table->read_counter++;
        hash_num =hash_index(page->address_num,bucketsNo);
        //hash_num=returnHash(address,bucketsNo);

        #if DEBUG>=3
            oldest_page->print();
        #endif
        if(frame_counter<frames){
            if(table->table[hash_num]->find_replace(page)==0){
                table->table[hash_num]->page->push_back(page);
                table->page_faults++;
                frame_counter++;
            }
        }else{
            if(table->table[hash_num]->find_replace(page)==0){//If find the same page we will replace
                //Else we replace the oldest page
                #if DEBUG>=3
                    cout<< "old page address: " << oldest_page->head->r->address<<endl;
                #endif
                replace_lru(oldest_page->head->r,page);
                table->page_faults++;
                oldest_page->delete_first();


            }
        }
        #if DEBUG>=1
        cout << "frames: "<<frame_counter;
        oldest_page->print();
        //table->print();

        #endif
    }
    #if DEBUG>=1
        table->print();
    #endif

    cout << "~~~~~~~~~~~Stats~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< table->write_counter<<endl;
    cout << "Page read: "<< table->read_counter<<endl;
    cout << "Page faults: "<< table->page_faults<<endl;
    cout << "frames: "   << frame_counter<<endl;
    delete oldest_page;
    delete table;
}

void Second_chance(int q,int bucketsNo,int frames,string file_){
    hash_table table(bucketsNo);
    listPg *oldest_page=new listPg;

    // cout << "~~~~~~~~~~~Stats~~~~~~~~~~~~~~\n";
    // cout << "Page write: "<< table.write_counter<<endl;
    // cout << "Page read: "<< table.read_counter<<endl;
    // cout << "Page faults: "<< table.page_faults<<endl;
    // cout << "frames: "   << frame_counter<<endl;
    delete oldest_page;

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

void cut_hex(char* mem){
  char temp[100];

  memcpy( temp,mem , 5 );
  mem[5]='\0';
}
