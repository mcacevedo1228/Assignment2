// CS366 Assignment 2
// Marc Acevedo
//Accomplish low-level FileIO in C


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <utime.h>
#include <sys/stat.h>

// Declarations
void permission_time(char *read_in_file);
void get_last(char *read_in_file);
void append(char *file, char *new_file); 
void zero_out(char *read_in_file);
void delete_file(char *read_in_file);
void copy(char *read_in_file, char *new_file);
void cat(char *read_in_file);
int file_process(int argc, char *argv[], int placer);
int command_compare(char *seeker, char *actual);
void command_menu();


/**
 * Purpose: Main Method
 * Input: arg count and array or args
 * Return: int
 **/
int main(int argc, char *argv[])
{
	int i = 1;  // Starts at 1 to avoid main call arg
	int file_status = 0;

	if (argc < 2)
	{
		printf("Not enough arguments!");
		return 1;
	}

	command_menu();

	for (; i < argc; i++)
	{
		file_status = file_process(argc, argv, i);
        
		if (file_status == -1)
		{
			return 0;
		}
	}
    
	return 0;
}

/**
 * Purpose: process each file passed in
 * Input: arg count, array of passed in args, and a place holder
 *
 **/
int file_process(int argc, char *argv[], int placer)
{

	char *fileName[] = {argv[placer]};

	char user_in;

	while (!command_compare(&user_in, "q"))		// '&' references location
	{
        printf("File %s: command: ", *fileName);
        scanf("%s", &user_in);

        if (command_compare(&user_in, "c"))
        {
            cat(*fileName);
        }

        else if (command_compare(&user_in, "n"))
        {
            return 0;
        }

        else if (command_compare(&user_in, "d"))
        {
            printf("Enter file to copy to:\n");
            char str4[256];
            scanf("%s", str4);
            copy(*fileName, str4);
        }
        
        else if (command_compare(&user_in, "r"))
        {
            int ret;
            printf("Enter new name: ");
            char new_name[256];
            scanf("%s", new_name);
            
            ret = rename(*fileName, new_name);
            
            if (ret == 0)
            {
                printf("File renamed successfully! ");
            }
        }
        
        else if (command_compare(&user_in, "u"))
        {
            delete_file(*fileName);			
        }
        
        else if (command_compare(&user_in, "t"))
        {
            zero_out(*fileName);
        }
        
        else if (command_compare(&user_in, "a"))
        {
            printf("Enter file to append: \n");
            char newSt[256];
            scanf("%s", newSt);
            
            append(*fileName, newSt);
        }
        
        else if (command_compare(&user_in, "l"))
        {
            get_last(*fileName);
        }
        
        else if (command_compare(&user_in, "x"))
        {
            permission_time(*fileName);
        }
        
        else if (command_compare(&user_in, "m"))
        {
            int modNum;
            printf("Enter the desired permission number: ");
            scanf("%d", &modNum);
            chmod(*fileName, modNum);
        }
        
        else
        {
            printf("Invalid argument. Try again. \n");
            continue;
        }
    
}

printf("q has been entered. Program is now exiting.\n\n");
return -1;

}

/**
 * Purpose: compare two characters passed in
 * Input: a character to be sosught for and a charcacter that is to be found
 * Returns: 1 for found, 0 for not found
 **/
int command_compare (char *seeker, char *actual)
{
	char *string1 = seeker;
	char *string2 = actual;
	int ret;
	
	ret = strcmp(string1, string2);

	if (ret == 0)
	{
		return 1;
	}

	return 0;
}



/**
 * Purpose: Display list of viable commands
 * Input: N/A
 * Returns: N/A
 **/
void command_menu()
{
	printf("Menu:\n");
   	printf("Press 'c' to list the current file\n");
    printf("Press 'd' to copy the current file to another file\n");
	printf("Press 'r' to rename the current file \n");
	printf("Press 't' to zero the current file \n");
	printf("Press 'a' to append the current file to another file\n");
	printf("Press 'l' to display the last 100 bytes of the current file\n");
	printf("Press 'x' to change the current file's access time to the current time\n");
    printf("Press 'm' to change the current file's access permission\n");
	printf("Press 'n' to goto the next file \n");
	printf("Press 'q' to quit \n");
	printf("\n");
}


/**
 * Purpose: Lists currrent file
 * Input: Read-in file
 * Returns: N/A
 **/
void cat(char *read_in_file)
{
	char c;
    
    FILE *write = fopen("/dev/tty", "w");
	FILE *file = fopen(read_in_file, "r");

	c = fgetc(file);

	while (c != EOF)
	{		
		fputc(c, write);
		c = fgetc(file);
	}

	fclose(write);
	fclose(file);
}

/**
 * Purpose: Copy file
 * Input: Read-in file
 * Returns: N/A
 **/
void copy(char *read_in_file, char *new_file)
{
	char c;

	
	FILE *file = fopen(read_in_file, "r");
	FILE *write = fopen(new_file, "w");

	c = fgetc(file);
	while (c != EOF)
	{
		fputc(c, write);
		c = fgetc(file);
	}
	
	fclose(write);
	fclose(file);

}


/**
 * Purpose: Deletes passed in file
 * Input: Read-ion file
 * Returns: N/A
 **/
void delete_file(char *read_in_file)
{
	int ret;

	ret = unlink(read_in_file);

	if (ret == 0)
	{
		printf("File deleted Successfully\n");
	}

}

/**
 * Purpose: Clears passed in file
 * Input: Read-in file
 * Returns: N/A
 **/
void zero_out(char *read_in_file)
{
	FILE *file = fopen(read_in_file, "w");
	fclose(file);
}

/**
 * Purpose: Append Read in file with other
 * Input: Read-in file and file to append
 * Returns: N/A
 **/
void append(char *file, char *new_file) 
{
	char buffer[256];

    	FILE *old_file = fopen(file, "r");
    	FILE *append_file = fopen(new_file, "a");
    
    	if (old_file == NULL) 
    	{
    		perror("Error opening file.");
    	}
	else 
	{
		while (fgets(buffer, sizeof(buffer), old_file)) 
		{
			fprintf(append_file, "%s", buffer);
		}
	}

	fclose(old_file);
	fclose(append_file);

}

/**
 * Purpose: get last 100bytes of file
 * Input: Read-in file
 * Returns: N/A
 **/
void get_last(char *read_in_file)
{
    int f, data;
    char tail[101];

    f = open(read_in_file, O_RDONLY);

    if (f == -1) 
    {
        printf("Error opening file\n");
    }

    lseek(f, -100L, SEEK_END);
    data = read(f, tail, 100);
    tail[data] = '\0';

    printf("%s\n", tail);

    close(f);
}

/**
 * Purpose: Changes permission time to current time
 * Input: Read-in file
 * Returns: N/A
 **/
void permission_time(char *read_in_file)
{
	struct utimbuf *current_time = NULL;
	utime(read_in_file, current_time);
}

