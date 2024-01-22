#pragma once

#include "constants.h"
#define NB_MOVIES NUMBER_OF_MOVIES


/*

`preds_id_movies` contains the **id** of the movies from 1 to NUMBER_OF_MOVIES, and not their indexes of an array which go from 0 to 17769

`nb_preds_asked` contains the number of predictions asked

`nb_predis` contains the real number of predictions the algorithm did

*/
void algo0_random_predictions(int* preds_id_movies, int nb_preds_demandes, int* nb_predis);

