#include "hash_function.hpp"

uint32_t SuperFastHash (const char * data, int len) {
uint32_t hashh = len, tmp;
int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hashh  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hashh;
        hashh   = (hashh << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hashh  += hashh >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hashh += get16bits (data);
                hashh ^= hashh << 16;
                hashh ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hashh += hashh >> 11;
                break;
        case 2: hashh += get16bits (data);
                hashh ^= hashh << 11;
                hashh += hashh >> 17;
                break;
        case 1: hashh += (signed char)*data;
                hashh ^= hashh << 10;
                hashh += hashh >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hashh ^= hashh << 3;
    hashh += hashh >> 5;
    hashh ^= hashh << 4;
    hashh += hashh >> 17;
    hashh ^= hashh << 25;
    hashh += hashh >> 6;


    return hashh;
}

int hash_index(const char * data, int len, int bucketsno){
    int hashh=SuperFastHash(data,len);

    if((int)hashh<0)
        hashh*=-1;

    hashh=hashh%bucketsno+1;
    return hashh;
}
