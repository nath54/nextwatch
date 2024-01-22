#include <string.h>
#include <stdio.h>

#include "../include/utils.h"

//file_exists take input and verify if the file named in the input exist if yess it return 1 else it returns 0.
int file_exists(char* input)
{
    //file_name stores the name of the file given by input.
    const char* file_name = (const char*)input;
    //file_path is the path to the given file and will permit us to know if this file exits.
    FILE* file_path = fopen(file_name, "r");
    //exist is our truth indicator.
    int exist = 0;

    //if the file path exists then he isn't NULL so it exists else the file doesn't exist.
    if (file_path != NULL)
    {
        exist = 1;
        fclose(file_path);
    }

    return exist;
}

//occurrence_of_movies take a string of movie ids seperated by a ',' and its length and return the number of movie id.
int occurrence_of_movies(char* input, int length)
{
    //nb_movies will be used to count the number of movies it start at one because the number of movies is equal to the number of quotes plus 1.
    int nb_movies = 1;

    //this condition enable us to verify if there are movies.
    if (length == 0){
        fprintf(stderr, "ERROR NO MOVIE\n");
        exit(42);
        //return ERROR_NO_MOVIE;
    }

    //this loop add 1 to the number of movies for each quotes because the quotes seperates the id of each movie
    for (int i = 0; i < length; i++)
    {
        if (input[i]==',')
            nb_movies += 1;
    }

    return nb_movies;
}

/*
entry_type is an integer pointer which contain 0 if `input` is a file path, and 1 if it's an list of movies ids.

id_movies is a Array* pointer, which contain the movies ids if `input` is a list of movie ids.
The function : 

    read the end of `input` and compare it to ".txt" :
        -> True, if the file exists it gives entry_type the value 0.
        -> False, put in id_movies the id of each movie.

*/
int get_list_of_liked_movies(char* input, int* entry_type, int id_movies[], int* nb_movies)
{
    //

    //length stores the length of `input`
    int length = (int)strlen(input);
    //compare_test will be our comparator to know if `input` is a file
    const char* compare_test = ".txt";
    //compare_input is the part of the input which will be compared, it's its last 4 char
    char* compare_input = input + length - 4;
    
    //we compare the two to know if input is a ".txt" file
    if (strcmp((const char*)compare_test, (const char*)compare_input)==0)
    {
        //we verify if this file exists
        if (file_exists(input)){
            //if it exists then we set the value pointed by `entry_type` to 0 for a file
            *entry_type = 0;
        }
        else{
            //else we return an error
            fprintf(stderr, "FILE DOESN'T EXISTS\n");
            exit(42);
        }

        //`it` is used to not trespass the upper limit of NUMBER_OF_MOVIES movies
        int it = 0;
        //`buffer` will stores an id in char
        char buffer[10];
        //`int_buffer` will stores the `buffer` char id in type int
        int int_buffer = -1;
        //`file_path` is the file path.
        FILE* file_path = fopen((const char*)input, "r");
        //we see if the file is reachable with the given file path
        if (file_path == NULL){
            fprintf(stderr, "ERROR, FILE UNREACHABLE\n");
            exit(42);
        }

        //we take the id of each line and stores id in `id_movies` in type int
        while (! feof(file_path) && it != 17769){
            fgets(buffer, 8, file_path);
            int_buffer = atoi((const char*)buffer);
            id_movies[it] = int_buffer;
            it += 1;
        }

        //we give the nb of movies
        (*nb_movies) = it;

        //we check if there are not to many movies
        if (it == 17769){
            printf("You got too many movies check if you have reappearance\n");
        }

        //we close the file path
        fclose(file_path);
    }

    //if `input` isn't a file then it's a list of movie ids
    else
    {
        //str_id_movies will store the string which contain the movie id in type char
        char str_id_movies[20]="";
        //int_id_movies will store the movie id in type int.
        int int_id_movies = -1;

        //if it exists then we set the value pointed by `entry_type` to 1 for a list
        *entry_type = 1;

        // We count the number of occurences of movies there will be
        int nb_movies_with_doublons = occurrence_of_movies(input, length);
        int crt_movies = 0;

        /*
        we do a loop to get the ids of each movie (in type int) by using the fact that each movie
        id is separated by a ','
        */
        for (int i = 0; i <= length ; i++)
        {
            //if `input[i]` is a ',' then we are at the end of an id
            if (input[i] == ',' || input[i] == '\0')
            {
                //so we need to change the type to int by using the `atoi` function
                int_id_movies = atoi((const char*)str_id_movies);
                //and we append it to `id_movies`.

                if(crt_movies >= (nb_movies_with_doublons)){
                    fprintf(stderr, "ERROR, there are more movies that we counted!\n");
                    exit(42);
                }
                // printf("crt_movies = %d, int_id_movies = %d, nb_movies = %d, *id_movie == NULL = %d\n", crt_movies, int_id_movies, *nb_movies, *id_movies==NULL);
                id_movies[crt_movies] = int_id_movies;
                crt_movies++;
                *nb_movies = crt_movies;
                
                //we clear the `str_id_movies`
                str_id_movies[0] = '\0';
            }
            
            //we are not at the end of an id
            else
            {
                //so we put the end of the string 1 memory case farther
                str_id_movies[strlen(str_id_movies)+1] = '\0';
                //and we add `input[i]` at the old end
                str_id_movies[strlen(str_id_movies)] = input[i];
            }
        }

    }

    return ERROR_OK;
}
