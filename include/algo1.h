#pragma once

#include "constants.h"
#include "lib.h"

typedef struct{
    float notes[NUMBER_OF_MOVIES];
} UserMoviesVec;



/*

`preds_id_movies` contains the **id** of the movies from 1 to NUMBER_OF_MOVIES, and not their indexes of an array which go from 0 to 17769

`nb_preds_asked` contains the number of predictions asked

`nb_predis contains` the real number of predictions the algorithm did

*/
void algo1_partial_clustering(MainData* main_data, const args_t* args, int* preds_id_movies, int nb_preds_demandes, int* nb_predis);


/*
Take a random user that liked one of the movies given in input
and take a random movie that is liked by that user
*/
void algo5_select_with_films(MainData* main_data, const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis);

