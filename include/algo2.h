#pragma once

#include "constants.h"
#include "lib.h"

typedef struct {
    double vect[10];
    unsigned int dim;
} MovieVect10;

typedef struct {
    double vect[100];
    unsigned int dim;
} MovieVect100;

void prepare_algo2(MainData* data);

void algo2_random_indexing_v10(const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis);

void algo2_random_indexing_v100(const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis);
