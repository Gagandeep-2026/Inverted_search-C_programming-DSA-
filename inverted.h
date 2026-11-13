
#ifndef INVERTED_H
#define INVERTED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 0
#define FAILURE -1

// Linked list to store filenames
typedef struct node
{
    char filename[20];
    struct node *link;
} Slist;

// Sub node
typedef struct sub_node
{
    int wordcount;
    char filename[20];
    struct sub_node *link;
} snode;

// Main node
typedef struct main_node
{
    char word[20];
    int file_count;
    snode *slink;
    struct main_node *mlink;
} mnode;

// Hash table
typedef struct hash
{
    int index;
    mnode *hlink;
} hash_t;

// Function declarations
int file_insert(Slist **head, char *file_name);
int print_list(Slist *head);
void initialize_htable(hash_t *ptr, int size);
void create_database(Slist *head, hash_t *ptr, int size);
void display_database(hash_t *arr, int size);
void search_database(hash_t *ptr,char *word);
int save_database(hash_t *ptr, char *filename,int size);
int update_database(hash_t *ptr, int size);

#endif
