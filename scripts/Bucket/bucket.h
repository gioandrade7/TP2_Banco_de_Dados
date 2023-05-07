#ifndef BUCKET_H
#define BUCKET_H

#include "../Constantes/cte.h"
#include <bits/stdc++.h>
using namespace std;

typedef struct Bucket{
    unsigned int blocos[bucket_size];
} Bucket;

Bucket createBucket(){
    Bucket bucket;
    for(int i = 0; i < bucket_size; i++){
        bucket.blocos[i] = i;
    }
    return bucket;
}

#endif