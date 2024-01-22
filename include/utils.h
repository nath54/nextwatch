#pragma once

#include "args_parser.h"
#include "lib.h"

enum Errors{
    //ERROR_OK is returned if there is no error.
    ERROR_OK,
    //ERROR_FILE_DOESNT_EXIST is returned if the file doesn't exist.
    ERROR_FILE_DOESNT_EXIST,
    //ERROR_NO_MOVIE is returned when there is an error due to the lack of movie.
    ERROR_NO_MOVIE,
    //ERROR_REALLOC is returned if the realloc didn't worked.
    ERROR_REALLOC
};

//file_exists take input and verify if the file named in the input exist if yess it return 1 else it returns 0.
int file_exists(char* input);

//occurrence_of_movies take a string of movie ids seperated by a ',' and its length and return the number of movie id.
int occurrence_of_movie(char* input, int lenght);

/*
entry_type is an integer pointer which contains 0 if `input` is a file path, and 1 if it's an list of movie ids.

id_movies is an Array* pointer, which contains the movie ids if `input` is a list of movie ids.
The function : 

    read the end of `input` and compare it to ".txt" :
        -> True, if the file exists it gives entry_type the value 0.
        -> False, put in id_movies the id of each movie.

*/
int get_list_of_liked_movies(char* input, int* entry_type, int id_movies[], int* nb_movies);


