/*------------------------------------------
 *
 * Written by: Essey Mehari
 * For: CMPT 201, X02L, Assignment A2
 * Lab instructor: Ardy RD
 * Lecture instructor: Dhara Wagh
 * Description: Program to reading postal codes and storing
 * them in a hash tabl
 * Filename: pcode.c
 * 
 *-------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht.h"

#define MAX_LINE 100

/* linked list node */
typedef struct node {
    char *code;
    struct node *next;
} node;

/* copy string */
char *copy_string(const char *s)
{
    char *new = malloc(strlen(s) + 1);
    if (new != NULL) {
        strcpy(new, s);
    }
    return new;
}

/* add postal code to list */
void add_code(node **head, const char *code)
{
    node *new_node = malloc(sizeof(node));
    node *curr;

    if (new_node == NULL) return;

    new_node->code = copy_string(code);
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    curr = *head;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = new_node;
}

/* print codes (10 per line max) */
void print_codes(node *head)
{
    int count = 0;
    node *curr = head;

    while (curr != NULL) {
        printf("%s", curr->code);
        count++;

        if (curr->next != NULL) {
            if (count % 10 == 0) {
                printf("\n");
            } else {
                printf(",");
            }
        }

        curr = curr->next;
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    FILE *fp;
    hashtable ht;
    char line[MAX_LINE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <postalcodes_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    ht = ht_create();
    if (ht == NULL) {
        fprintf(stderr, "Failed to create hash table\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    /* read file */
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *comma;
        char *newline;
        char *city;
        char *postal;
        node *list;

        newline = strchr(line, '\n');
        if (newline != NULL) *newline = '\0';

        comma = strchr(line, ',');
        if (comma == NULL) continue;

        *comma = '\0';
        city = line;
        postal = comma + 1;

        list = (node *) ht_lookup(ht, city);

        if (list == NULL) {
            list = NULL;
            add_code(&list, postal);
            ht_insert(ht, city, list);
        } else {
            add_code(&list, postal);
        }
    }

    fclose(fp);

    /* read cities from stdin */
    while (fgets(line, sizeof(line), stdin) != NULL) {
        char *newline;
        node *list;

        newline = strchr(line, '\n');
        if (newline != NULL) *newline = '\0';

        list = (node *) ht_lookup(ht, line);

        if (list != NULL) {
            print_codes(list);
        }
    }

    ht_free(ht);

    return EXIT_SUCCESS;
}