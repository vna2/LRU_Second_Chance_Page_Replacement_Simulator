#include <iostream>

int hash_index(const int data, int bucketsno){
    int hash;
    hash=data%bucketsno;
    return hash;
}
