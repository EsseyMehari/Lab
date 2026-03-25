/*------------------------------------------
 *
 * Written by: Essey Mehari
 * For: CMPT 201, X02L, Assignment A2
 * Lab instructor: Ardy RD
 * Lecture instructor: Dhara Wagh
 * Description: Implementation of hash table functions using
 * linear probing.
 * Filename: ht.c
 * 
 *-------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht.h"
#include "ht_impl.h"

/* make a copy of a string */
static char *copy_string(const char *s)
{
    char *new_str = malloc(strlen(s) + 1);

    if (new_str != NULL) {
        strcpy(new_str, s);
    }

    return new_str;
}

/* hash function */
static size_t hash(const char *s, size_t capacity)
{
    size_t value = 5381;
    unsigned char c;

    while ((c = (unsigned char)*s++) != '\0') {
        value = value * 33 + c;
    }

    return value % capacity;
}

/* resize table */
static void resize_table(hashtable ht)
{
    entry *old_table;
    size_t old_capacity;
    size_t i;

    old_table = ht->table;
    old_capacity = ht->capacity;

    ht->capacity = ht->capacity * 2 + 1;
    ht->table = calloc(ht->capacity, sizeof(entry));

    if (ht->table == NULL) {
        ht->table = old_table;
        ht->capacity = old_capacity;
        return;
    }

    ht->size = 0;

    for (i = 0; i < old_capacity; i++) {
        if (old_table[i].in_use == 1) {
            ht_insert(ht, old_table[i].key, old_table[i].value);
            free(old_table[i].key);
        }
    }

    free(old_table);
}

hashtable ht_create(void)
{
    hashtable ht;

    ht = malloc(sizeof(struct ht));
    if (ht == NULL) {
        return NULL;
    }

    ht->capacity = START_SIZE;
    ht->size = 0;
    ht->table = calloc(ht->capacity, sizeof(entry));

    if (ht->table == NULL) {
        free(ht);
        return NULL;
    }

    return ht;
}

void ht_free(hashtable ht)
{
    size_t i;

    if (ht == NULL) {
        return;
    }

    for (i = 0; i < ht->capacity; i++) {
        if (ht->table[i].in_use == 1) {
            free(ht->table[i].key);
        }
    }

    free(ht->table);
    free(ht);
}

void ht_insert(hashtable ht, char *key, void *value)
{
    size_t index;

    if (ht == NULL || key == NULL) {
        return;
    }

    if ((ht->size + 1) * 3 >= ht->capacity * 2) {
        resize_table(ht);
    }

    index = hash(key, ht->capacity);

    while (ht->table[index].in_use == 1) {
        if (strcmp(ht->table[index].key, key) == 0) {
            ht->table[index].value = value;
            return;
        }

        index = (index + 1) % ht->capacity;
    }

    while (ht->table[index].deleted == 1) {
        index = (index + 1) % ht->capacity;
    }

    ht->table[index].key = copy_string(key);
    ht->table[index].value = value;
    ht->table[index].in_use = 1;
    ht->table[index].deleted = 0;
    ht->size++;
}

void *ht_lookup(const hashtable ht, const char *key)
{
    size_t index;
    size_t start;

    if (ht == NULL || key == NULL) {
        return NULL;
    }

    index = hash(key, ht->capacity);
    start = index;

    while (ht->table[index].in_use == 1 || ht->table[index].deleted == 1) {
        if (ht->table[index].in_use == 1 &&
            strcmp(ht->table[index].key, key) == 0) {
            return ht->table[index].value;
        }

        index = (index + 1) % ht->capacity;

        if (index == start) {
            break;
        }
    }

    return NULL;
}

void ht_remove(hashtable ht, const char *key)
{
    size_t index;
    size_t start;

    if (ht == NULL || key == NULL) {
        return;
    }

    index = hash(key, ht->capacity);
    start = index;

    while (ht->table[index].in_use == 1 || ht->table[index].deleted == 1) {
        if (ht->table[index].in_use == 1 &&
            strcmp(ht->table[index].key, key) == 0) {
            free(ht->table[index].key);
            ht->table[index].key = NULL;
            ht->table[index].value = NULL;
            ht->table[index].in_use = 0;
            ht->table[index].deleted = 1;
            ht->size--;
            return;
        }

        index = (index + 1) % ht->capacity;

        if (index == start) {
            break;
        }
    }
}

void ht_print_dist(const hashtable ht)
{
    size_t i;

    if (ht == NULL) {
        return;
    }

    for (i = 0; i < ht->capacity; i++) {
        if (ht->table[i].in_use == 1) {
            printf("%s\n", ht->table[i].key);
        } else {
            printf("NULL\n");
        }
    }
}

void ht_foreach(const hashtable ht, ht_iter f)
{
    size_t i;

    if (ht == NULL || f == NULL) {
        return;
    }

    for (i = 0; i < ht->capacity; i++) {
        if (ht->table[i].in_use == 1) {
            f(ht->table[i].key, ht->table[i].value);
        }
    }
}
