#include "inverted.h"
int file_insert(Slist **head, char *file_name)
{
    Slist *temp;
    temp=*head;

    Slist *ptr=malloc(sizeof(Slist));
    strcpy(ptr->filename, file_name);   //reason is this 'filename' is array.
    ptr->link=NULL;

    if(*head==NULL)
    {
        *head=ptr;
        return SUCCESS;
    }
    while(temp!=NULL)
    {
        if(temp->link==NULL)
        {
            temp->link=ptr;
            return SUCCESS;
        }
        temp=temp->link;
    }
    return FAILURE;
}


//this is for print the stored files.
int print_list(Slist *head)
{
    Slist *temp;
    temp=head;

    if(head==NULL)
    {
        printf("no files existed !\n");
        return FAILURE;
    }
    while(temp!=NULL)
    {
        printf("%s\n",temp->filename);
        temp=temp->link;
    }
    return FAILURE;
}


//create the database with the help of the main_node and sub_node.
void initialize_htable(hash_t *ptr ,int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        ptr[i].index=i;
        ptr[i].hlink=NULL;
    }
}


void create_database(Slist *head, hash_t *ptr, int size)
{
    Slist *temp = head;
    char word[20];

    while (temp != NULL)
    {
        FILE *fp = fopen(temp->filename, "r");

        if (fp == NULL)
        {
            printf("Unable to open %s\n", temp->filename);
            temp = temp->link;
            continue;
        }

        while (fscanf(fp, "%19s", word) == 1)
        {
            int index = word[0] - 'a'; // calculated index.

            if (index < 0 || index >= size)
                continue;

            // Search for the word in the main-node list
            mnode *main_temp = ptr[index].hlink;

            while (main_temp != NULL)
            {
                if (strcmp(main_temp->word, word) == 0)
                {
                    break;
                }  
                main_temp = main_temp->mlink;
            }

            // CASE 1: Word does not exist
            if (main_temp == NULL)
            {
                mnode *new_main = malloc(sizeof(mnode));
                snode *new_sub = malloc(sizeof(snode));

                if (new_main == NULL || new_sub == NULL)
                {
                    free(new_main);
                    free(new_sub);
                    fclose(fp);
                    return;
                }

                strcpy(new_main->word, word);
                new_main->file_count = 1;
                new_main->mlink=NULL;

                strcpy(new_sub->filename, temp->filename);
                new_sub->wordcount = 1;
                new_sub->link = NULL;

                new_main->slink = new_sub;

                // Connect the new main node to the hash table
                mnode *temph;
                temph=ptr[index].hlink;
                if(temph==NULL)
                {
                    ptr[index].hlink=new_main;
                }
                else
                {
                    while(temph->mlink!=NULL)
                    {
                        temph=temph->mlink;
                    }
                    temph->mlink=new_main;
                }
                //above is for printing one by one.
            }
            else
            {
                // Search for the current filename
                snode *sub_temp = main_temp->slink;

                while (sub_temp != NULL)
                {
                    if (strcmp(sub_temp->filename, temp->filename) == 0)
                    {
                        break;
                    }
                    sub_temp = sub_temp->link; //if file name is not same it will not enter the loop sub_temp=NULL;
                }

                // CASE 2: Same word and same file
                if (sub_temp != NULL)
                {
                    sub_temp->wordcount++;
                }
                else
                {
                    // CASE 3: Same word, different file
                    snode *new_sub = malloc(sizeof(snode));

                    if (new_sub == NULL)
                    {
                        fclose(fp);
                        return;
                    }

                    strcpy(new_sub->filename, temp->filename);
                    new_sub->wordcount = 1;
                
                    //updating to the main_node
                    snode *temps=main_temp->slink; // Adding the subnode 1 by one.
                    if(temps==NULL)
                    {
                        main_temp->slink=new_sub;
                    }
                    else
                    {
                        while(temps->link!=NULL)
                        {
                            temps=temps->link;
                        }
                        temps->link=new_sub;
                        //above printing in order.
                    }
                    main_temp->file_count++;
                }
            }
        }

        fclose(fp);
        temp = temp->link;
    }

    printf("Database created successfully!\n");
}


void display_database(hash_t *ptr, int size)
{
    mnode *mtemp;
    snode *stemp;

    printf("%-10s %-15s %-15s %-15s %-15s\n","Index", "Word", "File_count","Word_count", "File_name");

    for(int i = 0; i < size; i++)
    {
        mtemp = ptr[i].hlink;
        while(mtemp != NULL)
        {
            stemp = mtemp->slink;
            int first = 1;

            while(stemp != NULL)
            {
                if(first)
                {
                    printf("%-10d %-15s %-15d %-15d %-15s\n",i, mtemp->word,mtemp->file_count,stemp->wordcount,stemp->filename);
                    first = 0;
                }
                else
                {
                    printf("%-10s %-15s %-15s %-15d %-15s\n","", "", "",stemp->wordcount,stemp->filename);
                }
                stemp = stemp->link;
            }
            mtemp = mtemp->mlink;
        }
    }
}

void search_database(hash_t *ptr, char *word)
{
    int index = word[0] - 'a';

    mnode *mtemp;
    snode *stemp;

    mtemp=ptr[index].hlink;

    if(mtemp==NULL)
    {
        printf("The word is not present");
    }
    while(mtemp!=NULL)
    {
        if(strcmp(mtemp->word,word)==0)
        {
            printf("word %s is present in %d files\n",mtemp->word,mtemp->file_count);

            stemp=mtemp->slink;
            while(stemp!=NULL)
            {
                printf("In %s => %d times\n",stemp->filename,stemp->wordcount);
                stemp=stemp->link;
            }
        }
        mtemp=mtemp->mlink;
    }
}


int save_database(hash_t *ptr, char *filename, int size)
{
    char *backup = strrchr(filename, '.');
    if (backup == NULL || strcmp(backup, ".txt") != 0)
    {
        printf("The file is not valid!\n");
        return FAILURE;
    }
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Unable to open the file!\n");
        return FAILURE;
    }
    mnode *mtemp;
    snode *stemp;
    for (int i = 0; i < size; i++)
    {
        mtemp = ptr[i].hlink;
        while (mtemp != NULL)
        {
            fprintf(fp, "#%d;%s;%d;", i,
                    mtemp->word, mtemp->file_count);

            stemp = mtemp->slink;

            while (stemp != NULL)
            {
                fprintf(fp, "%s;%d;",stemp->filename, stemp->wordcount);
                stemp = stemp->link;
            }
            fprintf(fp, "\n");
            mtemp = mtemp->mlink;
        }
    }

    fclose(fp);

    printf("Database saved successfully!\n");

    return SUCCESS;
}

int update_database(hash_t *ptr, int size)
{
    char filename[50];
    char line[4096];

    printf("Enter the saved database file name: ");
    scanf("%49s", filename);

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open %s\n", filename);
        return FAILURE;
    }

    // Check whether hash table is empty
    for (int i = 0; i < size; i++)
    {
        if (ptr[i].hlink != NULL)
        {
            printf("Hash table already contains data\n");
            fclose(fp);
            return FAILURE;
        }
    }

    // Read database file line by line
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *token = strtok(line, ";\n\r");

        if (token == NULL)
            continue;

        // Extract hash table index
        if (token[0] != '#')
        {
            fclose(fp);
            return FAILURE;
        }

        int index = atoi(token + 1);

        if (index < 0 || index >= size)
        {
            fclose(fp);
            return FAILURE;
        }

        // Extract word
        char *word_token = strtok(NULL, ";\n\r");

        // Extract file count
        char *count_token = strtok(NULL, ";\n\r");

        if (word_token == NULL || count_token == NULL)
        {
            fclose(fp);
            return FAILURE;
        }

        // Create main node
        mnode *new_main = malloc(sizeof(mnode));

        if (new_main == NULL)
        {
            fclose(fp);
            return FAILURE;
        }

        if (strlen(word_token) >= sizeof(new_main->word))
        {
            free(new_main);
            fclose(fp);
            return FAILURE;
        }

        strcpy(new_main->word, word_token);

        new_main->file_count = atoi(count_token);
        new_main->slink = NULL;
        new_main->mlink = NULL;

        // Create subnodes
        for (int j = 0; j < new_main->file_count; j++)
        {
            char *file_token = strtok(NULL, ";\n\r");
            char *wordcount_token = strtok(NULL, ";\n\r");

            if (file_token == NULL || wordcount_token == NULL)
            {
                snode *del = new_main->slink;

                while (del != NULL)
                {
                    snode *next = del->link;
                    free(del);
                    del = next;
                }

                free(new_main);
                fclose(fp);
                return FAILURE;
            }

            snode *new_sub = malloc(sizeof(snode));

            if (new_sub == NULL)
            {
                snode *del = new_main->slink;

                while (del != NULL)
                {
                    snode *next = del->link;
                    free(del);
                    del = next;
                }

                free(new_main);
                fclose(fp);
                return FAILURE;
            }

            if (strlen(file_token) >= sizeof(new_sub->filename))
            {
                free(new_sub);

                snode *del = new_main->slink;

                while (del != NULL)
                {
                    snode *next = del->link;
                    free(del);
                    del = next;
                }

                free(new_main);
                fclose(fp);
                return FAILURE;
            }

            strcpy(new_sub->filename, file_token);
            new_sub->wordcount = atoi(wordcount_token);
            new_sub->link = NULL;

            // Insert subnode at the end
            snode *stemp = new_main->slink;

            if (stemp == NULL)
            {
                new_main->slink = new_sub;
            }
            else
            {
                while (stemp->link != NULL)
                {
                    stemp = stemp->link;
                }

                stemp->link = new_sub;
            }
        }

        // Insert main node at the end
        mnode *mtemp = ptr[index].hlink;

        if (mtemp == NULL)
        {
            ptr[index].hlink = new_main;
        }
        else
        {
            while (mtemp->mlink != NULL)
            {
                mtemp = mtemp->mlink;
            }

            mtemp->mlink = new_main;
        }
    }

    fclose(fp);

    printf("Database updated successfully!\n");

    return SUCCESS;
}