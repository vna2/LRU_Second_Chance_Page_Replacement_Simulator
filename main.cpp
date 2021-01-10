#include <iostream>
#include "hash_table.hpp"
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
    int q=100;
    int frames=1000;
    int max_q=1000000;
    int bucket=5;
    int algorithm=0;
    for (int i = 1; i < argc; i+=2)
        if(!strcmp(argv[i],"-A"))
            algorithm=atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-K"))
            max_q=atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-Q"))
            q=atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-F"))
            frames=atoi(argv[i+1]);
        else if(!strcmp(argv[i],"-B"))
            bucket=atoi(argv[i+1]);
    if(argc==1){
        cout << "algorithm run with default values\n";
        cout << "Algorithm: LRU\n";
        cout << "Max Page read: "<<max_q<<endl;
        cout << "Page read per file: "<<q<<endl;
        cout << "Frames: "<<frames<<endl;
        cout << "Buckets: "<<bucket<<endl<<endl;

    }
    else{
        cout << "Algorithm: ";
        if(algorithm==0)
            cout << "LRU\n";
        else
            cout << "SECOND_CHANCE\n";
        cout << "Max Page read: "<<max_q<<endl;
        cout << "Page read per file: "<<q<<endl;
        cout << "Frames: "<<frames<<endl;
        cout << "Buckets: "<<bucket<<endl<<endl;
    }

    if(max_q % q != 0){
        cout<< "Max q cant divided \n";
        //return 0;
    }
    if(max_q>1000000){
        cout<< "Read max = 1000000\nExit\n";
        return 0;
    }

    if (algorithm==0) {
        cout << "LRU\n";
        lru(q,bucket,frames,max_q);
    }else{
        cout << "SECOND_CHANCE\n";
        Second_chance(q,bucket,frames,max_q);
    }

    return 0;
}


void lru(int q,int bucketsNo,int frames,int MAX_Q){
//~~~~~~~~~~~~~~~~bzip.trace~~~~~~~~~~~~~~~~~//
    hash_table *P1table= new hash_table(bucketsNo);
    listPg *P1oldest_page=new listPg();
    string file_P1 ="bzip.trace";
    ifstream fileP1;
    fileP1.open(file_P1);
    int frame_counterP1=0;
//~~~~~~~~~~~~~~~~gcc.trace~~~~~~~~~~~~~~~~~//

    hash_table *P2table= new hash_table(bucketsNo);
    listPg *P2oldest_page=new listPg();
    //P2oldest_page->head=NULL;
    string file_P2 ="gcc.trace";
    ifstream fileP2;
    fileP2.open(file_P2);
    int frame_counterP2=0;

    unsigned int t =0;
    int write_back=0;
    int max= MAX_Q/q;
    int i;
    int y;
    for (i = 0; i < max; i++){
        for (y = 0; y < q; y++) {
            if(i % 2 == 0){//bzip.trace file
                unsigned int address_numP1=0 ;

                char address[10];
                address[5]='\0';
                char role[2];
                role[1]='\0';
                fileP1 >> address;
                fileP1 >> role;
                cut_hex(address);

                address_numP1= hex2int(address);

                Page* page= new Page(address,address_numP1,role,t++);


                if(strcmp(role,"W")==0){
                    P1table->write_counter++;
                    page->dirty=true;
                }
                else
                    P1table->read_counter++;

                int hash_num =hash_index(page->address_num,bucketsNo);

                if(frame_counterP1+frame_counterP2<frames){
                    if(P1table->table[hash_num]->find_replace(page,P1oldest_page,1)==0){
                        P1table->table[hash_num]->page->push_back(page);
                        P1oldest_page->push_back(page);
                        P1table->page_faults++;
                        frame_counterP1++;
                    }
                }else{
                    if(P1table->table[hash_num]->find_replace(page,P1oldest_page,1)==0){//If find the same page we will replace
                        //Else we replace the P1oldest page
                        //LRU
                        if(  P1oldest_page->length!=0 && P2oldest_page->length!=0 ){
                            if(P1oldest_page->head->r->t < P2oldest_page->head->r->t){//replace from p1 table
                                int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                                if(P1oldest_page->head->r->dirty){
                                    write_back++;
                                }
                                P1table->table[hash]->page->delete_item(P1oldest_page->head->r);
                                P1table->table[hash_num]->page->push_back(page);
                                P1oldest_page->delete_first();
                                P1oldest_page->push_back(page);
                                P1table->page_faults++;
                            }
                            else if(P1oldest_page->head->r->t > P2oldest_page->head->r->t) {//replace from p2 table
                                int hash= hash_index(P2oldest_page->head->r->address_num,bucketsNo);
                                if(P2oldest_page->head->r->dirty){
                                    write_back++;
                                }
                                P2table->table[hash]->page->delete_item(P2oldest_page->head->r);
                                P1table->table[hash_num]->page->push_back(page);
                                frame_counterP2--;
                                frame_counterP1++;
                                P2oldest_page->delete_first();
                                P1oldest_page->push_back(page);
                                P1table->page_faults++;
                            }
                        }else if(P1oldest_page->length==0){
                            int hash= hash_index(P2oldest_page->head->r->address_num,bucketsNo);
                            if(P2oldest_page->head->r->dirty){
                                write_back++;
                            }
                            P2table->table[hash]->page->delete_item(P2oldest_page->head->r);
                            P1table->table[hash_num]->page->push_back(page);
                            frame_counterP2--;
                            frame_counterP1++;
                            P2oldest_page->delete_first();
                            P1oldest_page->push_back(page);
                            P1table->page_faults++;
                        }else if(P2oldest_page->length==0){
                            int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                            if(P1oldest_page->head->r->dirty){
                                write_back++;
                            }
                            P1table->table[hash]->page->delete_item(P1oldest_page->head->r);
                            P1table->table[hash_num]->page->push_back(page);
                            P1oldest_page->delete_first();
                            P1oldest_page->push_back(page);
                            P1table->page_faults++;
                        }
                    }

                }
                #if DEBUG>=1
                // cout << "\n\n P1 \n\n";
                // cout << "frames: "<<frame_counterP1<<endl;
                // cout << "OLD PAGES\n";
                // P1oldest_page->print();
                // cout << "BUCKET\n";
                // P1table->print();

                #endif
            }
            else{//gcc.trace file
                unsigned int address_numP2=0 ;
                char address[10];
                address[5]='\0';
                char role[2];
                role[1]='\0';
                fileP2 >> address;
                fileP2 >> role;
                cut_hex(address);

                address_numP2= hex2int(address);

                Page* page= new Page(address,address_numP2,role,t++);

                if(strcmp(role,"W")==0){
                    P2table->write_counter++;
                    page->dirty=true;
                }
                else
                    P2table->read_counter++;

                int hash_num =hash_index(page->address_num,bucketsNo);
                if(frame_counterP1+frame_counterP2<frames){
                    if(P2table->table[hash_num]->find_replace(page,P2oldest_page,1)==0){
                        P2table->table[hash_num]->page->push_back(page);
                        P2oldest_page->push_back(page);
                        P2table->page_faults++;
                        frame_counterP2++;
                    }
                }else{
                    if(P2table->table[hash_num]->find_replace(page,P2oldest_page,1)==0){//If find the same page we will replace
                            //Else we replace the P2oldest page
                            //LRU
                        if(  P1oldest_page->length!=0 && P2oldest_page->length!=0 ){
                            if(P2oldest_page->head->r->t < P1oldest_page->head->r->t){//replace from p2 table
                                int hash= hash_index(P2oldest_page->head->r->address_num,bucketsNo);
                                if(P2oldest_page->head->r->dirty){
                                    write_back++;
                                }
                                P2table->table[hash]->page->delete_item(P2oldest_page->head->r);
                                P2table->table[hash_num]->page->push_back(page);
                                P2oldest_page->delete_first();
                                P2oldest_page->push_back(page);
                                P2table->page_faults++;
                            }
                            else if(P2oldest_page->head->r->t > P1oldest_page->head->r->t){//replace from p1 table
                                int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                                if(P1oldest_page->head->r->dirty){
                                    write_back++;
                                }
                                P1table->table[hash]->page->delete_item(P1oldest_page->head->r);
                                P2table->table[hash_num]->page->push_back(page);
                                P1oldest_page->delete_first();
                                P2oldest_page->push_back(page);
                                frame_counterP1--;
                                frame_counterP2++;
                                P2table->page_faults++;
                            }
                        }else if(P2oldest_page->length==0){
                            int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                            if(P1oldest_page->head->r->dirty){
                                write_back++;
                            }
                            P1table->table[hash]->page->delete_item(P1oldest_page->head->r);
                            P2table->table[hash_num]->page->push_back(page);
                            P1oldest_page->delete_first();
                            P2oldest_page->push_back(page);
                            frame_counterP1--;
                            frame_counterP2++;
                            P2table->page_faults++;
                        }else if(P1oldest_page->length==0){
                            int hash= hash_index(P1oldest_page->head->r->address_num,bucketsNo);
                            if(P2oldest_page->head->r->dirty){
                                write_back++;
                            }
                            P2table->table[hash]->page->delete_item(P2oldest_page->head->r);
                            P2table->table[hash_num]->page->push_back(page);
                            P2oldest_page->delete_first();
                            P2oldest_page->push_back(page);
                            P2table->page_faults++;
                        }
                }
                    //cout << "P2oldest_page->head->r->t: "<<P2oldest_page->head->r->t<<endl;
            }

                #if DEBUG>=1
                // cout << "\n\n P2 \n\n";
                // cout << "frames: "<<frame_counterP2<<endl;
                // cout << "OLD PAGES\n";
                // P2oldest_page->print();
                // cout << "BUCKET\n";
                // P2table->print();
                #endif
            }
        }
    }

    cout << "~~~~~~~~~~~Stats-for-bzip.trace~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< P1table->write_counter<<endl;
    cout << "Page read: "<< P1table->read_counter<<endl;
    cout << "Page faults: "<< P1table->page_faults<<endl;
    cout << "Frames: "   << frame_counterP1<<endl<<endl;
    cout << "~~~~~~~~~~~Stats-for-gcc.trace~~~~~~~~~~~~~~~\n";
    cout << "Page write: "<< P2table->write_counter<<endl;
    cout << "Page read: "<< P2table->read_counter<<endl;
    cout << "Page faults: "<< P2table->page_faults<<endl;
    cout << "Frames: "   << frame_counterP2<<endl;
    cout<<"~~~~~~~~~~~~~Execution-stats~~~~~~~~~~~~~~~~~~"<<endl;
    cout << "Page writes: "<< P1table->write_counter+P2table->write_counter<<endl;
    cout << "Page reads: "<< P1table->read_counter+P2table->read_counter<<endl;
    cout << "Page faults: "<< P1table->page_faults+P2table->page_faults<<endl;
    cout << "Frames: "   <<frame_counterP1+frame_counterP2<<endl;
    cout << "Write back: "<< write_back<<endl;


    delete P1oldest_page;
    delete P1table;
    delete P2oldest_page;
    delete P2table;
}

void Second_chance(int q,int bucketsNo,int frames,int MAX_Q){
    //~~~~~~~~~~~~~~~~bzip.trace~~~~~~~~~~~~~~~~~//
        hash_table *P1table= new hash_table(bucketsNo);
        listPg *P1oldest_page=new listPg();
        string file_P1 ="bzip.trace";
        ifstream fileP1;
        fileP1.open(file_P1);
        int frame_counterP1=0;
    //~~~~~~~~~~~~~~~~gcc.trace~~~~~~~~~~~~~~~~~//

        hash_table *P2table= new hash_table(bucketsNo);
        listPg *P2oldest_page=new listPg();
        //P2oldest_page->head=NULL;
        string file_P2 ="gcc.trace";
        ifstream fileP2;
        fileP2.open(file_P2);
        int frame_counterP2=0;

        unsigned int t =0;
        int write_back=0;
        int max= MAX_Q/q;
        int i;
        int y;
        for (i = 0; i < max; i++){
            for (y = 0; y < q; y++) {
                if(i % 2 == 0){//bzip.trace file
                    unsigned int address_numP1=0 ;
                    char address[10];
                    address[5]='\0';
                    char role[2];
                    role[1]='\0';
                    fileP1 >> address;
                    fileP1 >> role;
                    cut_hex(address);

                    address_numP1= hex2int(address);

                    Page* page= new Page(address,address_numP1,role,t++);


                    if(strcmp(role,"W")==0){
                        P1table->write_counter++;
                        page->dirty=true;
                    }
                    else
                        P1table->read_counter++;

                    int hash_num =hash_index(page->address_num,bucketsNo);

                    if(frame_counterP1+frame_counterP2<frames){
                        if(P1table->table[hash_num]->find_replace(page,P1oldest_page,2)==0){
                            P1table->table[hash_num]->page->push_back(page);
                            P1oldest_page->push_back(page);
                            P1table->page_faults++;
                            frame_counterP1++;
                        }
                    }else{
                        if(P1table->table[hash_num]->find_replace(page,P1oldest_page,2)==0){//If find the same page we will replace
                            //Else we replace the P1oldest page(chance)
                            //Second_chance
                            if(  P1oldest_page->length!=0 && P2oldest_page->length!=0 ){//if both frames has pages
                                ////////
                                Page *oldestP1=P1oldest_page->head->r;
                                Page *oldestP2=P2oldest_page->head->r;
                                node *temp;
                                temp=P1oldest_page->head;
                                while(temp!=NULL){//find oldest page without second chance
                                    if(temp->r->Second_chance==false){
                                        oldestP1=temp->r;
                                        break;
                                    }
                                    int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                    P1table->table[hash]->page->find_change_second_chance(temp->r);
                                    temp->r->Second_chance=false;
                                    temp=temp->next;
                                }
                                temp=P2oldest_page->head;
                                while(temp!=NULL){
                                    if(temp->r->Second_chance==false){
                                        oldestP2=temp->r;
                                        break;
                                    }
                                    int hash=hash_index(temp->r->address_num,P2table->bucketsNo);
                                    P2table->table[hash]->page->find_change_second_chance(temp->r);
                                    temp->r->Second_chance=false;
                                    temp=temp->next;
                                }

                                ///////////
                                if(oldestP1->t < oldestP2->t){//replace from p1 table
                                    if(oldestP1==P1oldest_page->head->r){// if all buckets has 1 we done them 0 and take the older
                                        temp=P1oldest_page->head;
                                        while(temp!=NULL){
                                            int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                            P1table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }else{//we put 0 untill we find the page we wont
                                        while(temp!=NULL){
                                            if(temp->r->Second_chance==false){
                                                break;
                                            }
                                            int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                            P1table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }

                                    int hash= hash_index(oldestP1->address_num,bucketsNo);
                                    if(oldestP1->dirty){
                                        write_back++;
                                    }
                                    P1oldest_page->delete_item_second_chance(oldestP1,P1table);
                                    P1table->table[hash]->page->delete_item(oldestP1);
                                    P1table->table[hash_num]->page->push_back(page);
                                    P1oldest_page->push_back(page);
                                    P1table->page_faults++;
                                }
                                else if(oldestP1->t > oldestP2->t) {//replace from p2 table

                                    if(oldestP2==P2oldest_page->head->r){// if all buckets has 1(second chance) we done them 0 and take the older
                                        temp=P2oldest_page->head;
                                        while(temp!=NULL){//find oldest page without second chance
                                            int hash=hash_index(temp->r->address_num,P2table->bucketsNo);
                                            P2table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }else{//we do 0 until we find the page we wont
                                        while(temp!=NULL){
                                            if(temp->r->Second_chance==false){
                                                break;
                                            }
                                            int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                            P1table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }
                                    int hash= hash_index(oldestP2->address_num,bucketsNo);
                                    if(oldestP2->dirty){
                                        write_back++;
                                    }
                                    frame_counterP2--;
                                    frame_counterP1++;
                                    P2oldest_page->delete_item_second_chance(oldestP2,P2table);
                                    P2table->table[hash]->page->delete_item(oldestP2);
                                    P1table->table[hash_num]->page->push_back(page);
                                    P1oldest_page->push_back(page);
                                    P1table->page_faults++;
                                }
                            }else if(P1oldest_page->length==0){
                                //////
                                Page *oldestP2=P2oldest_page->head->r;
                                node *temp;
                                temp=P2oldest_page->head;
                                while(temp!=NULL){
                                    if(temp->r->Second_chance==false){
                                        oldestP2=temp->r;
                                        break;
                                    }
                                    int hash=hash_index(temp->r->address_num,P2table->bucketsNo);
                                    P2table->table[hash]->page->find_change_second_chance(temp->r);
                                    temp->r->Second_chance=false;
                                    temp=temp->next;
                                }
                                /////
                                int hash= hash_index(oldestP2->address_num,bucketsNo);
                                if(oldestP2->dirty){
                                    write_back++;
                                }
                                P2table->table[hash]->page->delete_item(oldestP2);
                                P1table->table[hash_num]->page->push_back(page);
                                frame_counterP2--;
                                frame_counterP1++;
                                P2oldest_page->delete_item_second_chance(oldestP2,P2table);
                                P1oldest_page->push_back(page);
                                P1table->page_faults++;
                            }else if(P2oldest_page->length==0){
                                Page *oldestP1=P1oldest_page->head->r;
                                node *temp;
                                temp=P2oldest_page->head;
                                if(oldestP1==P1oldest_page->head->r){// if all buckets has 1 we done them 0 and take the older
                                    temp=P1oldest_page->head;
                                    while(temp!=NULL){
                                        int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                        P1table->table[hash]->page->find_change_second_chance(temp->r);
                                        temp->r->Second_chance=false;
                                        temp=temp->next;
                                    }
                                }else{//we do 0 untill we find the page we wont
                                    while(temp!=NULL){
                                        if(temp->r->Second_chance==false){
                                            break;
                                        }
                                        int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                        P1table->table[hash]->page->find_change_second_chance(temp->r);
                                        temp->r->Second_chance=false;
                                        temp=temp->next;
                                    }
                                }

                                int hash= hash_index(oldestP1->address_num,bucketsNo);
                                if(oldestP1->dirty){
                                    write_back++;
                                }
                                P1oldest_page->delete_item_second_chance(oldestP1,P1table);
                                P1table->table[hash]->page->delete_item(oldestP1);
                                P1table->table[hash_num]->page->push_back(page);
                                P1oldest_page->push_back(page);
                                P1table->page_faults++;
                            }
                        }

                    }
                    #if DEBUG>=1
                    // cout << "\n\n P1 \n\n";
                    // cout << "frames: "<<frame_counterP1<<endl;
                    // cout << "OLD PAGES\n";
                    // P1oldest_page->print();
                    // cout << "BUCKET\n";
                    // P1table->print();

                    #endif
                }
                else{//gcc.trace file
                    unsigned int address_numP2=0 ;
                    char address[10];
                    address[5]='\0';
                    char role[2];
                    role[1]='\0';
                    fileP2 >> address;
                    fileP2 >> role;
                    cut_hex(address);

                    address_numP2= hex2int(address);

                    Page* page= new Page(address,address_numP2,role,t++);

                    if(strcmp(role,"W")==0){
                        P2table->write_counter++;
                        page->dirty=true;
                    }
                    else
                        P2table->read_counter++;

                    int hash_num =hash_index(page->address_num,bucketsNo);
                    if(frame_counterP1+frame_counterP2<frames){
                        if(P2table->table[hash_num]->find_replace(page,P2oldest_page,2)==0){
                            P2table->table[hash_num]->page->push_back(page);
                            P2oldest_page->push_back(page);
                            P2table->page_faults++;
                            frame_counterP2++;
                        }
                    }else{
                        if(P2table->table[hash_num]->find_replace(page,P2oldest_page,2)==0){//If find the same page we will replace
                            //Else we replace the P2 oldest page(chance)
                            //Second_chance

                            if(  P1oldest_page->length!=0 && P2oldest_page->length!=0 ){
                                /////
                                Page *oldestP1=P1oldest_page->head->r;//temp init
                                Page *oldestP2=P2oldest_page->head->r;
                                node *temp;
                                temp=P1oldest_page->head;
                                while(temp!=NULL){//find oldest page without second chance
                                    if(temp->r->Second_chance==false){
                                        oldestP1=temp->r;
                                        break;
                                    }
                                    temp=temp->next;
                                }
                                temp=P2oldest_page->head;
                                while(temp!=NULL){
                                    if(temp->r->Second_chance==false){
                                        oldestP2=temp->r;
                                        break;
                                    }
                                    temp=temp->next;
                                }
                                //////
                                if(oldestP2->t < oldestP1->t){//replace from p2 table
                                    if(oldestP2==P2oldest_page->head->r){// if all buckets has 1(second chance) we done them 0 and take the older
                                        temp=P2oldest_page->head;
                                        while(temp!=NULL){//find oldest page without second chance
                                            int hash=hash_index(temp->r->address_num,P2table->bucketsNo);
                                            P2table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }else{//we do 0 until we find the page we wont
                                        while(temp!=NULL){
                                            if(temp->r->Second_chance==false){
                                                break;
                                            }
                                            int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                            P1table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }
                                    int hash= hash_index(oldestP2->address_num,bucketsNo);
                                    if(oldestP2->dirty){
                                        write_back++;
                                    }
                                    P2oldest_page->delete_item_second_chance(oldestP2,P2table);
                                    P2table->table[hash]->page->delete_item(oldestP2);
                                    P2table->table[hash_num]->page->push_back(page);
                                    P2oldest_page->push_back(page);
                                    P2table->page_faults++;
                                }
                                else if(oldestP2->t > oldestP1->t){//replace from p1 table
                                    if(oldestP1==P1oldest_page->head->r){// if all buckets has 1 we done them 0 and take the older
                                        temp=P1oldest_page->head;
                                        while(temp!=NULL){
                                            int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                            P1table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }else{//we do 0 untill we find the page we wont
                                        while(temp!=NULL){
                                            if(temp->r->Second_chance==false){
                                                break;
                                            }
                                            int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                            P1table->table[hash]->page->find_change_second_chance(temp->r);
                                            temp->r->Second_chance=false;
                                            temp=temp->next;
                                        }
                                    }
                                    int hash= hash_index(oldestP1->address_num,bucketsNo);
                                    if(oldestP1->dirty){
                                        write_back++;
                                    }
                                    P1oldest_page->delete_item_second_chance(oldestP1,P1table);
                                    P1table->table[hash]->page->delete_item(oldestP1);
                                    P2table->table[hash_num]->page->push_back(page);
                                    P2oldest_page->push_back(page);
                                    frame_counterP1--;
                                    frame_counterP2++;
                                    P2table->page_faults++;
                                }
                            }else if(P2oldest_page->length==0){
                                Page *oldestP1=P1oldest_page->head->r;
                                node *temp;
                                temp=P1oldest_page->head;
                                while(temp!=NULL){
                                    if(temp->r->Second_chance==false){
                                        oldestP1=temp->r;
                                        break;
                                    }
                                    int hash=hash_index(temp->r->address_num,P1table->bucketsNo);
                                    P1table->table[hash]->page->find_change_second_chance(temp->r);
                                    temp->r->Second_chance=false;
                                    temp=temp->next;
                                }
                                int hash= hash_index(oldestP1->address_num,bucketsNo);
                                if(oldestP1->dirty){
                                    write_back++;
                                }
                                P1oldest_page->delete_item_second_chance(oldestP1,P1table);
                                P1table->table[hash]->page->delete_item(oldestP1);
                                P2table->table[hash_num]->page->push_back(page);
                                P2oldest_page->push_back(page);
                                frame_counterP1--;
                                frame_counterP2++;
                                P2table->page_faults++;
                        }
                    }
                        //cout << "P2oldest_page->head->r->t: "<<P2oldest_page->head->r->t<<endl;
                }

                    #if DEBUG>=1
                    // cout << "\n\n P2 \n\n";
                    // cout << "frames: "<<frame_counterP2<<endl;
                    // cout << "OLD PAGES\n";
                    // P2oldest_page->print();
                    // cout << "BUCKET\n";
                    // P2table->print();
                    #endif
                }
            }
        }

        cout << "~~~~~~~~~~~Stats-for-bzip.trace~~~~~~~~~~~~~~\n";
        cout << "Page write: "<< P1table->write_counter<<endl;
        cout << "Page read: "<< P1table->read_counter<<endl;
        cout << "Page faults: "<< P1table->page_faults<<endl;
        cout << "Frames: "   << frame_counterP1<<endl<<endl;
        cout << "~~~~~~~~~~~Stats-for-gcc.trace~~~~~~~~~~~~~~~\n";
        cout << "Page write: "<< P2table->write_counter<<endl;
        cout << "Page read: "<< P2table->read_counter<<endl;
        cout << "Page faults: "<< P2table->page_faults<<endl;
        cout << "Frames: "   << frame_counterP2<<endl;
        cout<<"~~~~~~~~~~~~~Execution-stats~~~~~~~~~~~~~~~~~~"<<endl;
        cout << "Page writes: "<< P1table->write_counter+P2table->write_counter<<endl;
        cout << "Page reads: "<< P1table->read_counter+P2table->read_counter<<endl;
        cout << "Page faults: "<< P1table->page_faults+P2table->page_faults<<endl;
        cout << "Frames: "   <<frame_counterP1+frame_counterP2<<endl;
        cout << "Write back: "<< write_back<<endl;


        delete P1oldest_page;
        delete P1table;
        delete P2oldest_page;
        delete P2table;

}



void cut_hex(char* mem){
  char temp[100];

  memcpy( temp,mem , 5 );
  mem[5]='\0';
}
