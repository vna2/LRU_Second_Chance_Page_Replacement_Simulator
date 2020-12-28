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
    cout<<"Role: "<<role<<endl;
    cout<< "Second chance: " <<Second_chance<<endl<<endl;
}

int listPg::find_replace(Page *pg,listPg *oldest_page,int alg){
    node *temp = new node;
    temp =head;
if(alg==1){
    //LRU
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
}else{
    //SECOND CHANCE
    while(temp!=NULL){
        if(temp->r->address_num==pg->address_num){
            if(temp->r->Second_chance==false){
                temp->r->Second_chance=true;
                return 1;
            }else{
                //replace
                temp->r=pg;
                temp->r->Second_chance=false;
                oldest_page->delete_item(pg);
                oldest_page->push_back(pg);
                temp->r=pg;
                return 1;
            }
        }
        temp=temp->next;
    }
    return 0;

}
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
        delete_first();
        length--;
        return;
    }
    while( temp!=NULL){
        if(temp->r->address_num==value->address_num){
            if(temp->next!=NULL){
                prev->next=temp->next;
                length--;
                delete temp;
                return;
            }
            else{
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
