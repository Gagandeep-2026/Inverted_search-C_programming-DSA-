/*DOCUMENTATION

NAME : Gagandeep MT
ADMIN ID : 26012_071
START DATE : 15/10/2026
END DATE : 31/10/2026

INVERTED SEARCH:

Developed a C-based Inverted Search application that creates and maintains an inverted index for multiple text files using Hash Tables and Linked Lists. The project allows users to efficiently search for a word and identify the files in which it appears, along with its occurrence count.
The project demonstrates practical implementation of file handling, dynamic memory allocation, structures, pointers, hashing techniques, and linked list data structures in C.

KEY FEATURES
•	Accepts and validates multiple .txt files through command-line arguments.
•	Checks for invalid, empty, and duplicate input files.
•	Creates an inverted index using a hash table of size 27.
•	Uses the first character of a word to calculate its hash index.
•	Stores words in main nodes and file details in subnodes using linked lists.
•	Maintains the following information:
o	Hash Index
o	Word
o	File Count
o	File Name
o	Word Count
•	Creates a database by reading words from multiple text files.
•	Displays the complete database in a structured format.
•	Searches for a particular word and displays the files in which it is present along with its occurrence count.
•	Saves the created database into a backup .txt file.
•	Updates the database by restoring previously saved data.
•	Supports adding new files to an existing database without losing previously stored records.
•	Includes input validation and error handling.

TECHNOLOGIES USED
C | Data Structures and Algorithms | Hash Tables | Singly Linked Lists | Structures | Pointers | Dynamic Memory Allocation | File Handling | String Handling

USAGE
________________________________________
COMMAND-LINE EXECUTION:
To execute the program, pass the input text files as command-line arguments.
./a.out file1.txt file2.txt file3.txt

MENU OPTIONS:
1. Create database
2. Display database
3. Search database
4. Update database
5. Save database
6. Exit

FUNCTIONALITY:
•	Create Database: Reads the input files and constructs the inverted index.
•	Display Database: Displays the hash index, words, file count, filenames and word counts.
•	Search Database: Searches for a given word and displays its occurrence details.
•	Update Database: Restores the previously saved database from a backup file.
•	Save Database: Saves the current database into a .txt file.
•	Exit: Terminates the application.

DATABASE UPDATE EXECUTION:
First execution:
./a.out file1.txt file2.txt file3.txt
1.	Create database.
2.	Save database as b.txt.
3.	Exit.
Second execution:
./a.out file4.txt
1.	Update database using b.txt.
2.	Create database to add file4.txt.
3.	Display or search the updated database.
4.	Save the updated database as b.txt.
5.	Exit.
________________________________________
SAMPLE OUTPUT
1.	Create Database
Choose your option: 1

Database created successfully!
2.	Display Database
Index      Word            File_count      Word_count      File_name

0          are             2               1               file1.txt
                                           1               file2.txt
7          hi              2               1               file1.txt
                                           1               file2.txt
           hello           2               1               file1.txt
                                           1               file3.txt
12         man             1               1               file3.txt
24         you             2               1               file1.txt
                                           1               file2.txt
3.	Search Database
Choose your option: 3

Enter the word to search: hi

word hi is present in 2 files

In file1.txt => 1 times
In file2.txt => 1 times
4.	Update Database
Choose your option: 4

Enter the saved database file name: b.txt

Database updated successfully!
5.	Save Database
Choose your option: 5

Enter the file to save the database: b.txt

Database saved successfully!
6.	Add New File to Existing Database
Choose your option: 1

Database created successfully!

The new file records are added to the previously restored database while preserving the existing records.
*/

#include "inverted.h"

int main(int argc, char *argv[])
{
    Slist *head=NULL;
    int i;

    if(argc < 2)
    {
        printf("Pass at least 2 arguments !\n");
        return FAILURE;
    }
    for(i=1;i<argc;i++)
    {
        char *ptr=strrchr(argv[i],'.');

        if (ptr == NULL || strcmp(ptr, ".txt") != 0)
        {
            printf("this not a valid file ! %s \n",argv[i]);
            continue;
        }
        FILE *fp=fopen(argv[i],"r");
        if(fp==NULL)
        {
            printf("there is no file existing !\n");
            continue;
        }
        if(fgetc(fp)==EOF) //if file is empty it will returns EOF
        {
            printf("the file is empty !\n"); // if file not empty it wil returns the starting address of the first character.
            fclose(fp);
            continue;

        }
        fclose(fp);
        
        //checking the duplicate files.
        int duplicate=0;
        Slist *temp=head;

        if(temp==NULL)
        {
            file_insert(&head, argv[i]);
            continue;
        }
        while(temp!=NULL)
        {
            if(strcmp(temp->filename, argv[i])==0)
            {
                duplicate++;
            }
            temp=temp->link;
        }
        if(duplicate==0)
        {
            file_insert(&head, argv[i]); //we are passing the files into single linked list.
            continue;
        }
        else
        {
            printf("detecting the duplicate files !");
        }
        //validation for checking the existence of multiple file. 
    }
    print_list(head);

//this is for database sector.
    int option;
    int size = 27;
    hash_t arr[27];
    initialize_htable(arr, size);

    char word[20];
    char filename[20];

    while (1)
    {
        printf("\n1. Create database\n");
        printf("2. Display database\n");
        printf("3. Search database\n");
        printf("4. Update database\n");
        printf("5. Save database\n");
        printf("6. Exit\n");

        printf("Choose your option: ");

        if (scanf("%d", &option) != 1)
        {
            printf("Invalid input\n");
            return FAILURE;
        }

        switch (option)
        {
            case 1:
                create_database(head, arr, size);
                break;

            case 2:
                display_database(arr, size);
                break;

            case 3:
                printf("Enter the word to search : ");
                scanf("%s",word);
                search_database(arr,word);
                break;

            case 4:
                update_database(arr,size);
                break;

            case 5:
                printf("Enter the file to save the database : ");
                scanf("%s",filename);
                save_database(arr,filename,size);
                break;

            case 6:
                return SUCCESS;

            default:
                printf("Invalid option\n");
        }
    }
}
