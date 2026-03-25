/*------------------------------------------
 *
 * Written by: Essey Mehari
 * For: CMPT 201, X02L, Assignment A2
 * Lab instructor: Ardy RD
 * Lecture instructor: Dhara Wagh
 * Description: Private structures for hash table implementation
 * Filename: ht_impl.h
 * 
 *-------------------------------------------------------------*/

#ifndef _HT_IMPL_H_
#define _HT_IMPL_H_

#include <stddef.h>

typedef struct entry {
    char *key;
    void *value;
    int in_use;
    int deleted;
} entry;

struct ht {
    entry *table;
    size_t capacity;
    size_t size;
};

#endif
