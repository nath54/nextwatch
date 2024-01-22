#include "data.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Load processed bin data
MainData* load_data(){
    printf("Loading data has begun.");
    MainData* data = (MainData*)calloc(sizeof(MainData), 1);
    // Opening Movies
    printf(" .Opening movies...\n");
    time_t t1 = time(NULL);
    FILE* f = fopen("processed_bin_data/parser_data_movies.bin", "rb");
    if(f == NULL){
        fprintf(stderr, "\e[31mERROR: FILE IS NULL\e[m\n");
        exit(42);
    }
    for(size_t i=0; i<NUMBER_OF_MOVIES; i++){
        MovieParser mp;
        fread(&mp, sizeof(MovieParser), 1, f);
        data->movies[i].id = mp.id;
        strcpy(data->movies[i].name, mp.name);
        data->movies[i].year = mp.year;
        data->movies[i].avg_rating = 0;
        data->movies[i].nb_ratings = 0;
        data->movies[i].ratings = (unsigned int*)calloc(arr_length_movie_ratings[i], sizeof(unsigned int));
    }
    fclose(f);
    time_t t2 = time(NULL);
    printf("Took %ld sec\n", t2-t1);
    // Opening Movies Rating
    printf(" .Opening movies rating...\n");
    t1 = time(NULL);
    f = fopen("processed_bin_data/parser_data_movies_ratings.bin", "rb");
    if(f == NULL){
        printf("ERROR: FILE IS NULL\n");
        exit(42);
    }
    fread(data->ratings, sizeof(MovieRating), NUMBER_OF_RATINGS, f);
    fclose(f);
    t2 = time(NULL);
    printf("Took %ld sec\n", t2-t1);
    // Initialising users
    printf(" .Initialising users...\n");
    t1 = time(NULL);
    for(int i=0; i<2649430; i++){
        if(arr_length_user_ratings[i] > 0){
            data->users[i].id = i;
            data->users[i].nb_ratings = 0;
            data->users[i].ratings = (unsigned int*)calloc(sizeof(int), arr_length_user_ratings[i]);
        }
        else{
            data->users[i].id = 0;
            data->users[i].nb_ratings = 0;
            data->users[i].ratings = NULL;
        }
    }
    t2 = time(NULL);
    printf("Took %ld sec\n", t2-t1);
    
    // For all ratings, adding them to the movies and users
    printf(" .Adding all the ratings to the users and movies...\n");
    t1 = time(NULL);
    for(size_t i=0; i<NUMBER_OF_RATINGS; i++){
        
        // Adding to the movie
        if(data->movies[data->ratings[i].id_movie-1].nb_ratings >= arr_length_movie_ratings[data->ratings[i].id_movie-1]){
            printf("ERROR, MORE THAN SHOULD MOVIES (%d > %d)!\n", data->movies[data->ratings[i].id_movie-1].nb_ratings, arr_length_movie_ratings[i]);
            exit(42);
        }
        data->movies[data->ratings[i].id_movie-1].ratings[data->movies[data->ratings[i].id_movie-1].nb_ratings] = i;
        data->movies[data->ratings[i].id_movie-1].nb_ratings++;
        
        // Adding to the user
        if(data->users[data->ratings[i].id_user].nb_ratings >= arr_length_user_ratings[data->ratings[i].id_user]){
            printf("ERROR, MORE THAN SHOULD USERS (%d > %d)!\n", data->users[data->ratings[i].id_user].nb_ratings, arr_length_user_ratings[i]);
            exit(42);
        }
        data->users[data->ratings[i].id_user].ratings[data->users[data->ratings[i].id_user].nb_ratings] = i;
        data->users[data->ratings[i].id_user].nb_ratings++;        
    }
    t2 = time(NULL);
    printf("Took %ld sec\n", t2-t1);
    // 
    printf(" .Calculating the average rating of all movies...");
    t1 = time(NULL);
    for(size_t i=0; i<NUMBER_OF_MOVIES; i++){
        float divi = 0;
        for(unsigned int j=0; j<data->movies[i].nb_ratings; j++){
            int idx_rating = data->movies[i].ratings[j];
            data->movies[i].avg_rating += data->ratings[idx_rating].rating;
            divi+=1.0;
        }
        if(divi > 0){
            data->movies[i].avg_rating /= divi;
        }
    }
    t2 = time(NULL);
    printf("Took %ld sec\n", t2-t1);
    //
    printf("Loading data ended.\n");
    //
    return data;
}

// Free data
void free_data(MainData* data){
    // Free movies->ratings
    for(size_t i=0; i<NUMBER_OF_MOVIES; i++){
        free(data->movies[i].ratings);
    }
    // Free users->ratings
    for(size_t i=0; i<2649430; i++){
        if(data->users[i].id != 0){
            free(data->users[i].ratings);
        }
    }
    //
    free(data);
}


