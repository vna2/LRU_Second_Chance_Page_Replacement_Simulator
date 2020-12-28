#include "list.hpp"

using namespace std;

Page::Page(char* _address,unsigned int _address_num,char* _role,clock_t _time){
    strcpy(this->address,_address);
    strcpy(this->role,_role);
    address_num=_address_num;
    this->t=_time;
}

void Page::print()
{
    cout<<"Address: "<<address<<endl;
    cout<<"Address_num: "<<address_num<<endl;
    cout<<"Role: "<<role<<endl<<endl;
}

int listPg::find_replace(Page *pg,listPg *oldest_page){
    node *temp = new node;
    temp =head;

    while(temp!=NULL){
        if(temp->r->address_num==pg->address_num){
            //replace
            oldest_page->delete_item(pg);
            oldest_page->push_back(pg);
            temp->r=pg;
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

int listPg::find(Page *pg){
    node *temp = head;
    while(temp!=NULL){
        if(temp->r->address_num==pg->address_num){
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

void listPg::replace_lru(Page *pg_old,Page *pg_new,listPg *oldest_page){
    node *temp = new node;
    temp =head;
    #if DEBUG>=0
        cout<< "new page address: " << pg_old->address<<endl;
        //cout<< "new page address: " << oldest_page->head->r->address<<endl;
        cout << "new page address: "<< pg_new->address<<endl;

    #endif
//    oldest_page->delete_item(pg_old);
    while(temp!=NULL){
        if(temp->r->address_num==pg_old->address_num){
            oldest_page->delete_item(pg_old);
            oldest_page->push_back(pg_new);
            temp->r= pg_new;
            #if DEBUG>=3
                cout<<"replace_lru \n";
            #endif
            return;
        }
        temp=temp->next;
    }
        cout<< "aaaaaaaaalgorithm has problem\n";

}

void listPg::push_back(Page *value){
    node *temp;
    temp=head;
    if(head==NULL){
        head=new node;
        head->r=value;
        length++;
        tail=head;

    }
    else{
        // tail->next=new node;
        // tail->next->r=value;
        // length++;
        // tail=tail->next;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=new node;
        temp=temp->next;
        temp->r=value;
        temp->next=NULL;

    }
}

listPg::listPg(){
  head=NULL;
  tail=NULL;
  length=0;
}

listPg::~listPg(){
  if(head!=NULL)
    delete head;
}

void listPg::print(){
    node *temp;
    temp=head;
    cout<< "~~~~~~~~~~~~Print List~~~~~~~~~~~~~\n";
    while(temp!=NULL){
      temp->r->print();
      temp=temp->next;
    }
}

void listPg::delete_item(Page *value){
    node *temp;
    node *prev;
    temp=head;
    prev=temp;
    if(temp->r->address_num==value->address_num){
        cout << "arxi listas\n";
        delete_first();
        length--;
        return;
    }
    while( temp!=NULL){
        if(temp->r->address_num==value->address_num){
            if(temp->next!=NULL){
                prev->next=temp->next;
                cout << "endiameso stixio stixio\n";
                length--;
                delete temp;
                return;
            }
            else{
                cout << "telefteo stixio\n";
                tail=prev;
                tail->next=NULL;
                length--;
                delete temp;
                return;
            }
        }
        prev=temp;
        temp=temp->next;
    }
    cout << "problem on list \n";
}

void listPg::delete_first(){
    node *temp=new node;
    temp=head;
    if(temp!=NULL){
        head=head->next;
        delete temp;
        length--;
    }
    else{
        delete temp;
    }
}

node::node(){
    r=new Page;
    next=NULL;
}

node::~node(){
  // if(next!=NULL)
  //   delete next;
}
