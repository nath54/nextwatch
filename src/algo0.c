#include "algo0.h"

#include <stdlib.h>
#include <time.h>



/*

preds_id_movies contains the **id** of the movies from 1 to NUMBER_OF_MOVIES, and not their indexes of an array which go from 0 to 17769

nb_preds_asked contains the number of predictions asked

nb_predis contains the real number of predictions the algorithm did

*/
void algo0_random_predictions(int* preds_id_movies, int nb_preds_asked, int* nb_predis){
    (*nb_predis) = 0;
    // srand((unsigned) time(NULL));
    for(int i=0; i<nb_preds_asked; i++){
        preds_id_movies[i] = 1 + (rand() % NB_MOVIES);
        (*nb_predis)++;
    }
}



