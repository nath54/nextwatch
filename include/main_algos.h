#pragma once

#include "lib.h"
#include "data.h"
#include "parser.h"
#include "utils.h"
#include "constants.h"
#include "args_parser.h"
#include "algo0.h"
#include "algo1.h"
#include "algo2.h"

typedef struct {
    int* predictions;
    int nb_predictions_faites;
    int nb_predictions_total;
} Predictions;

/*
Algorithms : 

 * Algorithm 0 : random
 * Algorithm 1 : partial clustering
 * Algorithm 2 : Random Indexing (v10)
 * Algorithm 3 : Random Indexing (v100)
 * Algorithm 4 : Random Good Movies
 * Algorithm 5 : Movie from Movie

*/

Predictions* main_algos(const args_t* args, MainData* main_data);

void static_print_algos(const args_t* args, MainData* main_data);
