#include "list.hpp"

using namespace std;

Page::Page(string _address,string _role,clock_t _time){
    this->address=_address;
    this->role=_role;
    this->t=_time;
}

void Page::print()
{
    cout<<"Address: "<<address<<endl;
    cout<<"Role: "<<role<<endl;
}

int listPg::find_replace(Page *pg){
    node *temp = head;
    for (int i = 0; i < length; i++) {
        if(temp->r->address==pg->address){
            //replace
            temp->r->role=pg->role;
            temp->r->t=pg->t;
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

int listPg::find(Page *pg){
    node *temp = head;
    for (int i = 0; i < length; i++) {
        if(temp->r->address==pg->address){
            return 1;
        }
        temp=temp->next;
    }
    return 0;
}

void listPg::replace_lru(Page *pg_old,Page *pg_new){
    node *temp = head;
    for (int i = 0; i < length; i++) {
        if(temp->r->address == pg_old->address){
            temp->r=pg_new;
            return;
        }
        temp=temp->next;
    }

    cout<< "algorithm has problem\n";
}

void listPg::push_back(Page *value){
    if(head==NULL){
        head=new node;
        head->r=value;
        length++;
        tail=head;

    }
    else{
        tail->next=new node;
        tail->next->r=value;
        length++;
        tail=tail->next;

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
    while(temp!=NULL){
      temp->r->print();
      temp=temp->next;
    }
}


void listPg::delete_first(){
    node *temp=new node;
    temp=head;
    head=head->next;
    delete temp;
    length--;
}

node::node(){
  next=NULL;
}

node::~node(){
  if(next!=NULL)
    delete next;
}
