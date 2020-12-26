#include "list.hpp"

using namespace std;

Page::Page(char* _address,char* _role,clock_t _time){
    strcpy(this->address,_address);
    strcpy(this->role,_role);
    this->t=_time;
}

void Page::print()
{
    cout<<"Address: "<<address<<endl;
    cout<<"Role: "<<role<<endl;
}

int listPg::find_replace(Page *pg,listPg *oldest_page){
    node *temp = head;
    for (int i = 0; i < length; i++) {
        if(strcmp(temp->r->address,pg->address)==0){
            //replace
            oldest_page->delete_item(pg);
            strcpy(temp->r->role,pg->role);
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

void listPg::replace_lru(Page *pg_old,Page *pg_new,listPg *oldest_page){
    node *temp = head;
    #if DEBUG>=3
        cout<< "new page address: " << pg_new->address<<endl;
    #endif
    oldest_page->delete_item(pg_old);
    for (int i = 0; i < length; i++) {
        if(strcmp(temp->r->address ,pg_old->address)==0){
            temp->r=pg_new;
            #if DEBUG>=3
                cout<<"replace_lru \n";
            #endif
            return;
        }
        temp=temp->next;
    }
    #if DEBUG>=3
        cout<< "algorithm has problem\n";
    #endif
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
    cout<< "~~~~~~~~~~~~Print List~~~~~~~~~~~~~\n";
    while(temp!=NULL){
      temp->r->print();
      temp=temp->next;
    }
}

void listPg::delete_item(Page *value){
    node *temp=new node;
    temp=head;
    if(strcmp(temp->r->address,value->address)==0){
        delete_first();
        return;
    }
    node *temp_last=new node;
    temp_last=tail;
    if(strcmp(temp_last->r->address,value->address)==0){
        delete temp_last;
        length--;
        return;
    }
    while(temp!=NULL || temp->next!=NULL){
        if(strcmp(temp->next->r->address ,value->address)==0){
            temp->next=temp->next->next;
            delete temp->next;
            return;
        }
        temp=temp->next;
    }



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
  next=NULL;
}

node::~node(){
  // if(next!=NULL)
  //   delete next;
}
