#include <iostream>
#include "hash_table.hpp"
#include "semaphores_sheared_mem.hpp"
#include "list.hpp"
//#include "hash_function.hpp"

using namespace std;

void P1(int algorithm,int q,int bucketsNo,int frames,int MAX_Q);
void P2(int algorithm,int q,int bucketsNo,int frames,int MAX_Q);
int returnHash(char* s,int MAX_LEN);
int hash_index(int s,int MAX_LEN);
unsigned int hex2int(char *hex);

void lru(int q,int bucketsno,int frames,int MAX_Q);
void Second_chance(int q,int bucketsNo,int frames,int MAX_Q);
void cut_hex(char* mem);
int main(int argc, char const *argv[]) {
    int q=10;
    int frames=10; //
    int max_q=1000;
    if(max_q % q != 0){
        cout<< "Max q cant divide \n";
    }
    lru(q,4,frames,max_q);
    //initialized_all_shared_memmory_semaphores();
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


void lru(int q,int bucketsNo,int frames,int MAX_Q){
//~~~~~~~~~~~~~~~~bzip.trace~~~~~~~~~~~~~~~~~//
    hash_table *P1table= new hash_table(bucketsNo);
    listPg *P1oldest_page=new listPg;
    unsigned int address_numP1=0 ;
    string file_P1 ="bzip.trace";
    ifstream fileP1;
    fileP1.open(file_P1);
    int frame_counterP1=0;
//~~~~~~~~~~~~~~~~gcc.trace~~~~~~~~~~~~~~~~~//

    hash_table *P2table= new hash_table(bucketsNo);
    listPg *P2oldest_page=new listPg;
    unsigned int address_numP2=0 ;
    string file_P2 ="gcc.trace";
    ifstream fileP2;
    fileP2.open(file_P2);
    int frame_counterP2=0;

    int max= MAX_Q/q;
    for (int i = 0; i < max; i++)
        for (int i = 0; i < q; i++) {
            if(i % 2 ==0){//bzip.trace file
                char address[10];
                address[5]='\0';
                char role[2];
                role[1]='\0';
                fileP1 >> address;
                fileP1 >> role;
                cut_hex(address);

                address_numP1= hex2int(address);

                Page* page= new Page(address,address_numP1,role,i);

                if(strcmp(role,"W")==0){
                    P1table->write_counter++;
                    page->dirty=true;
                }
                else
                    P1table->read_counter++;
                int hash_num =hash_index(page->address_num,bucketsNo);
                //hash_num=returnHash(address,bucketsNo

                if(frame_counterP1<frames){
                    if(P1table->table[hash_num]->find_replace(page,P1oldest_page,1)==0){
                        P1table->table[hash_num]->page->push_back(page);
                        P1oldest_page->push_back(page);
                        P1table->page_faults++;

                        frame_counterP1++;
                    }
                }else{
                    //int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                    if(P1table->table[hash_num]->find_replace(page,P1oldest_page,1)==0){//If find the same page we will replace
                        //Else we replace the P1oldest page
                        //LRU
                        int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                        if(P1oldest_page->head->r->dirty){
                            P1table->write_back++;
                        }
                        P1table->table[hash]->page->delete_item(P1oldest_page->head->r);
                        P1table->table[hash_num]->page->push_back(page);
                        P1oldest_page->delete_first();
                        P1oldest_page->push_back(page);
                        P1table->page_faults++;
                    }
                }
                #if DEBUG>=1
                //cout << "frames: "<<frame_counterP1<<endl;
                //P1oldest_page->print();
                //P1table->print();
                #endif
            }
            else{//gcc.trace file
                char address[10];
                address[5]='\0';
                char role[2];
                role[1]='\0';
                fileP2 >> address;
                fileP2 >> role;
                cut_hex(address);

                address_numP2= hex2int(address);

                Page* page= new Page(address,address_numP2,role,i);

                if(strcmp(role,"W")==0){
                    P2table->write_counter++;
                    page->dirty=true;
                }
                else
                    P2table->read_counter++;
                int hash_num =hash_index(page->address_num,bucketsNo);
                //hash_num=returnHash(address,bucketsNo

                if(frame_counterP2<frames){
                    if(P2table->table[hash_num]->find_replace(page,P2oldest_page,1)==0){
                        P2table->table[hash_num]->page->push_back(page);
                        P2oldest_page->push_back(page);
                        P2table->page_faults++;

                        frame_counterP2++;
                    }
                }else{
                    //int hash= hash_index(P2oldest_page->head->r->address_num,bucketsNo);
                    if(P2table->table[hash_num]->find_replace(page,P2oldest_page,1)==0){//If find the same page we will replace
                        //Else we replace the P2oldest page
                        //LRU
                        int hash= hash_index(P2oldest_page->head->r->address_num,bucketsNo);
                        if(P2oldest_page->head->r->dirty){
                            P2table->write_back++;
                        }
                        P2table->table[hash]->page->delete_item(P2oldest_page->head->r);
                        P2table->table[hash_num]->page->push_back(page);
                        P2oldest_page->delete_first();
                        P2oldest_page->push_back(page);
                        P2table->page_faults++;
                    }
                }
                #if DEBUG>=1
                //cout << "frames: "<<frame_counterP2<<endl;
                //P2oldest_page->print();
                //P2table->print();
                #endif
            }
        }

    cout << "~~~~~~~~~~~Stats-for-bzip.trace~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< P1table->write_counter<<endl;
    cout << "Page read: "<< P1table->read_counter<<endl;
    cout << "Write back: "<< P1table->write_back<<endl;
    cout << "Page faults: "<< P1table->page_faults<<endl;
    cout << "Frames: "   << frame_counterP1<<endl<<endl;

    cout << "~~~~~~~~~~~Stats-for-gcc.trace~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< P2table->write_counter<<endl;
    cout << "Page read: "<< P2table->read_counter<<endl;
    cout << "Write back: "<< P2table->write_back<<endl;
    cout << "Page faults: "<< P2table->page_faults<<endl;
    cout << "Frames: "   << frame_counterP2<<endl;
    delete P1oldest_page;
    delete P1table;
    delete P2oldest_page;
    delete P2table;
}

void Second_chance(int q,int bucketsNo,int frames,string file_){
    hash_table *table= new hash_table(bucketsNo);
    listPg *P1oldest_page=new listPg;
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
        //Page* pageL= new Page(address,address_num,role,i);


        if(strcmp(role,"W")==0){
            table->write_counter++;
            page->dirty=true;
        }
        else
            table->read_counter++;
        hash_num =hash_index(page->address_num,bucketsNo);

        if(frame_counter<frames){
            if(table->table[hash_num]->find_replace(page,P1oldest_page,2)==0){
                table->table[hash_num]->page->push_back(page);
                P1oldest_page->push_back(page);
                table->page_faults++;

                frame_counter++;
            }
        }else{
            if(table->table[hash_num]->find_replace(page,P1oldest_page,2)==0){//If find the same page we will replace
                //Else we replace the P1oldest page
                //SECOND CHANCE
                int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                if(P1oldest_page->head->r->dirty){
                    table->write_back++;
                }
                table->table[hash]->page->delete_item(P1oldest_page->head->r);
                table->table[hash_num]->page->push_back(page);
                P1oldest_page->delete_first();
                P1oldest_page->push_back(page);
                table->page_faults++;



            }
        }
        #if DEBUG>=1
        cout << "frames: "<<frame_counter<<endl;
        //P1oldest_page->print();
        table->print();
        #endif
    }
    #if DEBUG>=1
        //table->print();
    #endif

    cout << "~~~~~~~~~~~Stats~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< table->write_counter<<endl;
    cout << "Page read: "<< table->read_counter<<endl;
    cout << "Write back: "<< table->write_back<<endl;
    cout << "Page faults: "<< table->page_faults<<endl;
    cout << "frames: "   << frame_counter<<endl;
    delete P1oldest_page;
    delete table;

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
