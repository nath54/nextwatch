#include "args_parser.h"
#include "main_algos.h"
#include "lib.h"
#include "algo0.h"
#include "algo1.h"
#include "algo2.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

// The prediction 
Predictions* main_algos(const args_t* args, MainData* main_data){

    // Predictions 
    // predictions->predictions  contains the **id** of the movies from 1 to NUMBER_OF_MOVIES, and not their indexes of an array which go from 0 to 17769
    Predictions* predictions = (Predictions*)calloc(1, sizeof(Predictions));
    predictions->predictions = (int*)calloc(args->nb_predictions, sizeof(int));
    predictions->nb_predictions_faites = 0;
    int nb_preds_algo = (int)((float)args->nb_predictions * 0.7);
    //
    time_t t1 = time(NULL);
    //
    printf("\n");
    //
    switch (args->algorithm_of_prediction)
    {
        case 0: // Algorithm 0 : random
            printf("Random Algorithm : \n");
            algo0_random_predictions(predictions->predictions, nb_preds_algo, &(predictions->nb_predictions_faites));
            break;

        case 1: // Algorithm 1 : partial clustering
            printf("Partial Clustering : \n");
            algo1_partial_clustering(main_data, args, predictions->predictions, nb_preds_algo, &(predictions->nb_predictions_faites));
            break;
        
        case 2: // Algorithm 2 : Random Indexing (v10)
            printf("Random Indexing (v10) : \n");
            algo2_random_indexing_v10(args, predictions->predictions, nb_preds_algo, &(predictions->nb_predictions_faites));
            break;

        case 3: // Algorithm 3 : Random Indexing (v100)
            printf("Random Indexing (v100) : \n");
            algo2_random_indexing_v100(args, predictions->predictions, nb_preds_algo, &(predictions->nb_predictions_faites));
            break;

        case 4: // Algorithm 4 : Random Good Movies
            printf("Random Good Movies : \n");
            // Nothing to do here, the rest will be done after
            break;
        
        case 5: // Algorithm 5 : Select With Movies
            printf("Select With Movies : \n");
            algo5_select_with_films(main_data, args, predictions->predictions, nb_preds_algo, &(predictions->nb_predictions_faites));
            break;
            

        default: // Random Algorithm by default
            printf("Random Algorithm : \n");
            algo0_random_predictions(predictions->predictions, nb_preds_algo, &(predictions->nb_predictions_faites));
            break;
    }
    //
    // We complete the results with general good movies for a general discovery
    printf("Algorithm, predictions done : %d\n", predictions->nb_predictions_faites);
    int good_movies[2000];
    int nb_movies_with_avg_rating_above_4 = 0;
    for(int i=0; i<NUMBER_OF_MOVIES; i++){
        if(main_data->movies[i].avg_rating >= 4.0){
            good_movies[nb_movies_with_avg_rating_above_4] = i+1;
            nb_movies_with_avg_rating_above_4++;
        }
    }

    while(predictions->nb_predictions_faites < args->nb_predictions){
        predictions->predictions[predictions->nb_predictions_faites] = good_movies[(rand()%nb_movies_with_avg_rating_above_4)];
        predictions->nb_predictions_faites++;
    }

    // Prints the results
    time_t t2 = time(NULL);
    if(args->flags & FLAG_EXEC_TIME){
        printf("The prediction algorithm took %ld sec\n", (t2-t1));
    }

    return predictions;
}

void static_print_algos(const args_t* args, MainData* main_data){
    
    Predictions* predictions = main_algos(args, main_data);

    printf("\n\nPrediction results : \n");
    for(int i=0; i<predictions->nb_predictions_faites; i++){
        printf("  - %d - %s\n", predictions->predictions[i], main_data->movies[predictions->predictions[i]-1].name);
    }
    printf("\n\n");

    // Memory Free
    free(predictions->predictions);
    free(predictions);
}