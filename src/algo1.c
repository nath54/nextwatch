#include "lib.h"
#include "args_parser.h"
#include "algo1.h"

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// # Pour Algo Partial Clustering #

#define N_pcluster_users 5
#define Min_note_aime 3
#define Min_score_movie 2

#define Min_rating_per_users 20
#define Max_rating_per_users 1000
#define User_Skip_Percentage 80

// # Pour Algo n°5 #

#define Min_Note_Aimee 3

float distance_between_users(UserMoviesVec* a, UserMoviesVec* b){
    float dist = 0;
    for(int i=0; i<NUMBER_OF_MOVIES; i++){
        float diff = (a->notes[i] - b->notes[i]);
        dist += diff * diff;
    }
    dist = sqrt(dist);
    return dist;
}

float distance_between_users_float(float* a, float* b, int nb_dim){
    float dist = 0;
    for(int i=0; i<nb_dim; i++){
        float diff = (a[i] - b[i]);
        dist += diff * diff;
    }
    dist = sqrt(dist);
    return dist;
}

int mini(int a, int b){
    if(a <= b){
        return a;
    }
    return b;
}

int my_randint(int a, int b){
    return a + rand() % (b-a);
}

/*

preds_id_movie contains the **id** of the movies from 1 to NUMBER_OF_MOVIES, and not their indexes of an array which go from 0 to 17769

nb_preds_asked contains the number of predictions asked

nb_predis contains the real number of predictions the algorithm did

*/

/*
ALGORITHM PRINCIPLE:
  - Find the N closest users
  - Select movies from the movies the N users liked
*/
void algo1_partial_clustering(MainData* main_data, const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis){
    
    int nb_dims_vec = 0;
    // Calcul de la dimension
    int* correspondance = (int*)calloc(sizeof(int), args->nb_inputs);
    for(int i=0; i<args->nb_inputs; i++){
        bool unique = true;
        for(int j=0; j<nb_dims_vec; j++){
            if(correspondance[j] == args->input_id_movies[i]){
                unique = false;
                break;
            }
        }
        if(!unique){
            continue;
        }
        correspondance[nb_dims_vec] = args->input_id_movies[i];
        nb_dims_vec++;
    }
    //

    float* input_movies_vect = (float*)calloc(sizeof(float), nb_dims_vec);
    for(int i=0; i<args->nb_inputs; i++){
        // Recherche du bon indice dans input_movies_vect
        int ii = -1;
        for(int j = 0; j<nb_dims_vec; j++){
            if(args->input_id_movies[i] == correspondance[j]){
                ii = j;
                break;
            }
        }
        if(ii == -1){
            printf("Problème\n");
            continue;
        }
        //
        if(input_movies_vect[ii] == 0){
            input_movies_vect[ii] = 3;
        }
        else{
            if(input_movies_vect[ii] < 5){
                input_movies_vect[ii] += 0.5;
            }
        }
    }

    int scores_movies[NUMBER_OF_MOVIES] = {0};

    // Find the N closest users
    size_t selectionnes[N_pcluster_users] = {0};
    int nb_selectionnes = 0;
    for(int s=0; s<N_pcluster_users; s++){
        //
        printf("## S = %d / %d\n", s, N_pcluster_users);
        //
        size_t min_user=0;
        float min_dist=-1;
        // Recherche du minimum
        for(size_t u=0; u<2649430; u++){
            if(u % 1000000 == 0){
                printf("s = %d, user = %ld\n", s, u);
            }
            // Test si l'user existe
            if(main_data->users[u].id == 0){
                continue;
            }
        
            // Pour optimiser les calculs, on ne vas pas traiter tous les users
            if( main_data->users[u].nb_ratings < Min_rating_per_users && main_data->users[u].nb_ratings > Max_rating_per_users  && rand()%100 < User_Skip_Percentage){
                continue;
            }

            // Test s'il n'a pas été sélectionné
            bool sel = false;
            for(int l=0; l<s; l++){
                if(selectionnes[l] == u){
                    sel = true;
                    break;
                }
            }
            if(sel){
                continue;
            }
            ///////////////////////////////////////
            UserMoviesVec uvec;
            for(int j=0; j<NUMBER_OF_MOVIES; j++){
                uvec.notes[j]=0;
            }
            //
            if(main_data->users[u].id > 0){ // tests if users exists
                for(unsigned int j=0; j<main_data->users[u].nb_ratings; j++){
                    uvec.notes[main_data->ratings[main_data->users[u].ratings[j]].id_movie - 1] = main_data->ratings[main_data->users[u].ratings[j]].rating;
                }
            }
            float* uvec_float = (float*)calloc(sizeof(float), nb_dims_vec);
            for(int j = 0; j<nb_dims_vec; j++){
                uvec_float[j] = uvec.notes[correspondance[j]];
            }
            ///////////////////////////////////////
            float d = distance_between_users_float(uvec_float, input_movies_vect, nb_dims_vec);
            if(min_dist == -1 || d < min_dist){
                min_dist = d;
                min_user = u;
            }
        }
        // On ajoute le minimum
        selectionnes[s] = min_user;
        nb_selectionnes += 1;
        for(unsigned int irr =0; irr < main_data->users[min_user].nb_ratings; irr++){
            unsigned int idx_rating = main_data->users[min_user].ratings[irr];
            // Test if user liked the movie
            if(main_data->ratings[idx_rating].rating >= Min_note_aime){
                scores_movies[main_data->ratings[idx_rating].id_movie-1]+=1;
            }
            else{
                scores_movies[main_data->ratings[idx_rating].id_movie-1]-=1;
            }
        }
        
    }

    printf("Nombre d'utilsateurs selectionnes : %d\n", nb_selectionnes);

    // Select movies from the movies the N users liked
    int nb_movies = 0;
    int capacity = 0;
    //
    for(int i=0; i<NUMBER_OF_MOVIES; i++){
        if(scores_movies[i] >= Min_score_movie){
            capacity++;
        }
    }
    //
    int* movies_clustered = (int*)calloc(sizeof(int), capacity);
    if(movies_clustered == NULL){
        printf("ERROR MALLOC\n");
        exit(42);
    }

    for(int i=0; i<NUMBER_OF_MOVIES; i++){
        if(scores_movies[i] >= Min_score_movie){
            movies_clustered[nb_movies] = i+1;
            nb_movies++;
        }
    }

    printf("Nb movies = %d \n", nb_movies);

    //
    if(nb_movies <= nb_preds_demandes){
        for( int i=0; i<nb_movies; i++){
            preds_id_movie[i] = movies_clustered[i];
            (*nb_predis)++;
        }
    }
    else{
        for(int i=0; i<nb_preds_demandes; i++){
            unsigned int idx_movie = ((unsigned int)rand())%nb_movies;
            // printf("preds_id_movie[%d] = %d\n", i, idx_movie);
            preds_id_movie[i] = movies_clustered[idx_movie];
            (*nb_predis)++;
        }
    }
}

size_t nb_liked_movies_by_user(MainData* main_data, size_t id_user){
    size_t nb = 0;
    //
    for(size_t i=0; i<main_data->users[id_user].nb_ratings; i++){
        if(main_data->ratings[main_data->users[id_user].ratings[i]].rating >= Min_Note_Aimee){
            nb += 1;
        }
    }
    //
    return nb;
}

/*
Take a random user that liked one of the movies given in input
and take a random movie that is liked by that user
*/
int algo5_aux(MainData* main_data, const args_t* args){

    //
    size_t sum_tot_nb_ratings = 0;
    for(int i=0; i<args->nb_inputs; i++){
        int id_movie = args->input_id_movies[i];
        sum_tot_nb_ratings = main_data->movies[id_movie-1].nb_ratings;
    }
    //
    MovieRating mr;
    //
    size_t ii = my_randint(0, sum_tot_nb_ratings-1);
    //
    size_t tmp = 0;
    for(int i=0; i<args->nb_inputs; i++){
        //
        int id_movie = args->input_id_movies[i];
        //
        if(tmp + main_data->movies[id_movie-1].nb_ratings > ii){
            mr = main_data->ratings[main_data->movies[id_movie-1].ratings[ii-tmp]];
            break;
        }
        //
        tmp = main_data->movies[id_movie-1].nb_ratings;
    }
    //
    while(mr.rating < Min_Note_Aimee || nb_liked_movies_by_user(main_data, mr.id_user) == 0){
        //
        ii = my_randint(0, sum_tot_nb_ratings-1);
        //
        size_t tmp = 0;
        for(int i=0; i<args->nb_inputs; i++){
            //
            int id_movie = args->input_id_movies[i];
            //
            if(tmp + (size_t)(main_data->movies[id_movie-1].nb_ratings) > ii){
                mr = main_data->ratings[main_data->movies[id_movie-1].ratings[ii-tmp]];
                break;
            }
            //
            tmp = main_data->movies[id_movie-1].nb_ratings;
        }
        //
    }
    //
    size_t id_user_selectionne = mr.id_user;
    //
    MovieRating mrr;
    //
    int iii = my_randint(0, main_data->users[id_user_selectionne].nb_ratings-1);
    mrr = main_data->ratings[main_data->users[id_user_selectionne].ratings[iii]];
    //
    while(mr.rating < Min_Note_Aimee || nb_liked_movies_by_user(main_data, mr.id_user) == 0){
        //
        iii = my_randint(0, main_data->users[id_user_selectionne].nb_ratings-1);
        mrr = main_data->ratings[main_data->users[id_user_selectionne].ratings[iii]];
        //
    }
    //
    return mrr.id_movie;
}

/*
Take a random user that liked one of the movies given in input
and take a random movie that is liked by that user
*/
void algo5_select_with_films(MainData* main_data, const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis){
    if(args->nb_inputs == 0){
        fprintf(stderr, "\e[32mNo Inputs !\e[m");
        return;
    }
    //
    for(int i=0; i<nb_preds_demandes; i++){
        //
        int id_mov = algo5_aux(main_data, args);
        //
        bool est_deja_dans_input_ou_reponse = false;
        //
        for(int j=0; j<args->nb_inputs; j++){
            if(id_mov == args->input_id_movies[j]){
                est_deja_dans_input_ou_reponse = true;
                break;
            }
        }
        //
        for(int j=0; j<(*nb_predis); j++){
            if(id_mov == preds_id_movie[j]){
                est_deja_dans_input_ou_reponse = true;
                break;
            }
        }
        //
        while(est_deja_dans_input_ou_reponse){
            id_mov = algo5_aux(main_data, args);
            //
            est_deja_dans_input_ou_reponse = false;
            //
            for(int j=0; j<args->nb_inputs; j++){
                if(id_mov == args->input_id_movies[j]){
                    est_deja_dans_input_ou_reponse = true;
                    break;
                }
            }
            //
            for(int j=0; j<(*nb_predis); j++){
                if(id_mov == preds_id_movie[j]){
                    est_deja_dans_input_ou_reponse = true;
                    break;
                }
            }
        }
        //
        preds_id_movie[*nb_predis] = id_mov;
        (*nb_predis)++;
        //
    }
    //
}
