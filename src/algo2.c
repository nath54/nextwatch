#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

#include "lib.h"
#include "args_parser.h"
#include "algo2.h"

void print_vec_10(MovieVect10* v){
    printf("vect(");
    for(unsigned int i = 0; i < v->dim; i++){
        if(i > 0) printf(", ");
        printf(".%d: %lf", i, v->vect[i]);
    }
    printf(");");
}

void print_vec_100(MovieVect100* v){
    printf("vect(");
    for(unsigned int i = 0; i < v->dim; i++){
        if(i > 0) printf(", ");
        printf(".%d: %lf", i, v->vect[i]);
    }
    printf(");");
}

double movie_vect_10_dist(MovieVect10* v1, MovieVect10* v2){
    if(v1 == NULL || v2 == NULL){
        fprintf(stderr, "\e[31mError : Vectors are NULL !\e[m\n");
        exit(42);
    }
    if(v1->dim != v2->dim){
        fprintf(stderr, "\e[31mError : Vectors have not the same dimension !\e[m\n");
        exit(42);
    }
    //
    double dist = 0;
    for(unsigned int i=0; i<v1->dim; i++){
        double tmp = v1->vect[i] - v2->vect[i];
        dist += tmp * tmp;
    }
    //
    dist = sqrt(dist);
    //
    return dist;
}

double movie_vect_100_dist(MovieVect100* v1, MovieVect100* v2){
    if(v1 == NULL || v2 == NULL){
        fprintf(stderr, "\e[31mError : Vectors are NULL !\e[m\n");
        exit(42);
    }
    if(v1->dim != v2->dim){
        fprintf(stderr, "\e[31mError : Vectors have not the same dimension !\e[m\n");
        exit(42);
    }
    //
    double dist = 0;
    for(unsigned int i=0; i<v1->dim; i++){
        double tmp = v1->vect[i] - v2->vect[i];
        dist += tmp * tmp;
    }
    //
    dist = sqrt(dist);
    //
    return dist;
}


double random_double_between_0_and_1(){
    return ((double) rand())/((double)RAND_MAX);
}

void randomise_vector_10(MovieVect10* v){
    if(v == NULL){
        fprintf(stderr, "\e[31mError : Vector is NULL (randomise vector) !\e[m\n");
        exit(42);
    }
    for(unsigned int i = 0; i < v->dim; i++){
        v->vect[i] = random_double_between_0_and_1();
    }
}

void randomise_vector_100(MovieVect100* v){
    if(v == NULL){
        fprintf(stderr, "\e[31mError : Vector is NULL (randomise vector) !\e[m\n");
        exit(42);
    }
    for(unsigned int i = 0; i < v->dim; i++){
        v->vect[i] = random_double_between_0_and_1();
    }
}

size_t custom_size_t_randint(size_t nb){
    return rand() % nb;
}

void linear_norm_vect_10(MovieVect10* v){
    if(v == NULL){
        fprintf(stderr, "\e[31mError : Vector is NULL (linear norm vect) !\e[m\n");
        exit(42);
    }
    //
    double sum = 0;
    for(unsigned int i = 0; i < v->dim; i++){
        if(v->vect[i] < 0){
            sum += -(v->vect[i]);
        }
        else{
            sum += v->vect[i];
        }
    }
    //
    //
    if(sum < 0){
        sum = -sum;
    }
    if(sum >= 0.1){
        for(unsigned int i = 0; i < v->dim; i++){
            v->vect[i] /= sum;
        }
    }
}


void squared_norm_vect_10(MovieVect10* v){
    if(v == NULL){
        fprintf(stderr, "\e[31mError : Vector is NULL (linear norm vect) !\e[m\n");
        exit(42);
    }
    //
    MovieVect10 zero = (MovieVect10){0};
    double vec_length = movie_vect_10_dist(v, &(zero));
    //
    if(vec_length >= 0.1){
        for(unsigned int i = 0; i < v->dim; i++){
            v->vect[i] /= vec_length;
        }
    }
}

void linear_norm_vect_100(MovieVect100* v){
    if(v == NULL){
        fprintf(stderr, "\e[31mError : Vector is NULL (linear norm vect) !\e[m\n");
        exit(42);
    }
    //
    double sum = 0;
    for(unsigned int i = 0; i < v->dim; i++){
        if(v->vect[i] < 0){
            sum += -(v->vect[i]);
        }
        else{
            sum += v->vect[i];
        }
    }
    //
    if(sum < 0){
        sum = -sum;
    }
    if(sum >= 0.1){
        for(unsigned int i = 0; i < v->dim; i++){
            v->vect[i] /= sum;
        }
    }
}

void squared_norm_vect_100(MovieVect100* v){
    if(v == NULL){
        fprintf(stderr, "\e[31mError : Vector is NULL (linear norm vect) !\e[m\n");
        exit(42);
    }
    //
    MovieVect100 zero = (MovieVect100){0};
    double vec_length = movie_vect_100_dist(v, &(zero));
    //
    if(vec_length >= 0.1){
        for(unsigned int i = 0; i < v->dim; i++){
            v->vect[i] /= vec_length;
        }
    }
}
// v1 = v1 + coeff * v2
void add_vects_10_with_coeff(MovieVect10* v1, MovieVect10* v2, double coeff, bool normalise){
    if(v1 == NULL || v2 == NULL){
        fprintf(stderr, "\e[31mError : Vectors are NULL !\e[m\n");
        exit(42);
    }
    if(v1->dim != v2->dim){
        fprintf(stderr, "\e[31mError : Vectors have not the same dimension !\e[m\n");
        exit(42);
    }
    //
    for(unsigned int i = 0; i < v1->dim; i++){
        v1->vect[i] += coeff * v2->vect[i];
    }
    //
    if(normalise){
        linear_norm_vect_10(v1);
        //squared_norm_vect_10(v1);
    }
}

// v1 = v1 + coeff * v2
void add_vects_100_with_coeff(MovieVect100* v1, MovieVect100* v2, double coeff, bool normalise){
    if(v1 == NULL || v2 == NULL){
        fprintf(stderr, "\e[31mError : Vectors are NULL !\e[m\n");
        exit(42);
    }
    if(v1->dim != v2->dim){
        fprintf(stderr, "\e[31mError : Vectors have not the same dimension !\e[m\n");
        exit(42);
    }
    //
    for(unsigned int i = 0; i < v1->dim; i++){
        v1->vect[i] += coeff * v2->vect[i];
    }
    //
    if(normalise){
        linear_norm_vect_100(v1);
        //squared_norm_vect_100(v1);
    }
}

/*
Le principe de cet algo est simple : 
    - D'abord initialiser pour chaque film un vecteur aléatoire d'une certaine dimension
    - Ensuite, pour chaque utilisateur,
        - On récupère la liste des films que cet utilisateur a **aimé** (note >= note_min)
        - Pour chaque film film_1 de cette liste :
            - On en prend N autres film_2 au hasard de cet utilisateur
                - Et pour chaqu'un d'entre eux, on additionne le vecteur de film_2 au vecteur de film_1
            - On normalise le vecteur de film_1
*/
MovieVect10* random_vectors_10_algorithm(MainData* data, short note_min, size_t N, double sum_coefficient, bool sum_and_normalize_both){
    unsigned int v_dim = 10;
    // Initialisation (length = NUMBER_OF_MOVIES + 1)
    MovieVect10* film_vectors = (MovieVect10*)calloc(NUMBER_OF_MOVIES_PLUS_UN, sizeof(MovieVect10));
    for(unsigned int i = 0; i < NUMBER_OF_MOVIES_PLUS_UN; i++){
        film_vectors[i].dim = v_dim;
        randomise_vector_10(&(film_vectors[i]));
    }
    //
    for(size_t id_user = 0; id_user < TOTAL_IDS_OF_USERS; id_user++){
        // No users
        if(data->users[id_user].id == 0 || data->users[id_user].nb_ratings == 0){
            continue;
        }
        //
        printf("(Preparing algo v10) User : %ld\n", id_user);
        //
        unsigned short* list_liked_movies = (unsigned short*)calloc(data->users[id_user].nb_ratings, sizeof(unsigned short));
        size_t nb_liked_movies = 0;
        for(size_t j = 0; j < data->users[id_user].nb_ratings; j++){
            if(data->ratings[data->users[id_user].ratings[j]].rating >= note_min){
                list_liked_movies[nb_liked_movies] = data->ratings[data->users[id_user].ratings[j]].id_movie;
                nb_liked_movies++;
            }
        }
        //
        if(nb_liked_movies < N){
            for(size_t i = 0; i < nb_liked_movies; i++){
                for(size_t j = 0; j < i; j++){
                    size_t id_film_1 = list_liked_movies[i];
                    size_t id_film_2 = list_liked_movies[j];
                    //
                    add_vects_10_with_coeff(&(film_vectors[id_film_1]), &(film_vectors[id_film_2]), sum_coefficient, true);
                }
            }
        }
        else{
            for(size_t i = 0; i < nb_liked_movies; i++){
                for(size_t j = 0; j < i; j++){
                    size_t id_film_1 = list_liked_movies[i];
                    size_t id_film_2 = custom_size_t_randint(nb_liked_movies);
                    while(id_film_2 == id_film_1){
                        id_film_2 = custom_size_t_randint(nb_liked_movies);
                    }
                    //
                    add_vects_10_with_coeff(&(film_vectors[id_film_1]), &(film_vectors[id_film_2]), sum_coefficient, true);
                    if(sum_and_normalize_both){
                        add_vects_10_with_coeff(&(film_vectors[id_film_2]), &(film_vectors[id_film_1]), sum_coefficient, true);
                    }
                }
            }
        }
    }
    //
    return film_vectors;
}


MovieVect100* random_vectors_100_algorithm(MainData* data, short note_min, size_t N, double sum_coefficient, bool sum_and_normalize_both){
    unsigned int v_dim = 100;
    // Initialisation (length = NUMBER_OF_MOVIES + 1)
    MovieVect100* film_vectors = (MovieVect100*)calloc(NUMBER_OF_MOVIES_PLUS_UN, sizeof(MovieVect100));
    for(unsigned int i = 0; i < NUMBER_OF_MOVIES_PLUS_UN; i++){
        film_vectors[i].dim = v_dim;
        randomise_vector_100(&(film_vectors[i]));
    }
    //
    for(size_t id_user = 0; id_user < TOTAL_IDS_OF_USERS; id_user++){
        // No users
        if(data->users[id_user].id == 0 || data->users[id_user].nb_ratings == 0){
            continue;
        }
        //
        printf("(Preparing algo v100) User : %ld\n", id_user);
        //
        unsigned short* list_liked_movies = (unsigned short*)calloc(data->users[id_user].nb_ratings, sizeof(unsigned short));
        size_t nb_liked_movies = 0;
        for(size_t j = 0; j < data->users[id_user].nb_ratings; j++){
            if(data->ratings[data->users[id_user].ratings[j]].rating >= note_min){
                list_liked_movies[nb_liked_movies] = data->ratings[data->users[id_user].ratings[j]].id_movie;
                nb_liked_movies++;
            }
        }
        //
        if(nb_liked_movies < N){
            for(size_t i = 0; i < nb_liked_movies; i++){
                for(size_t j = 0; j < i; j++){
                    size_t id_film_1 = list_liked_movies[i];
                    size_t id_film_2 = list_liked_movies[j];
                    //
                    add_vects_100_with_coeff(&(film_vectors[id_film_1]), &(film_vectors[id_film_2]), sum_coefficient, false);
                }
            }
        }
        else{
            for(size_t i = 0; i < nb_liked_movies; i++){
                for(size_t j = 0; j < i; j++){
                    size_t id_film_1 = list_liked_movies[i];
                    size_t id_film_2 = custom_size_t_randint(nb_liked_movies);
                    while(id_film_2 == id_film_1){
                        id_film_2 = custom_size_t_randint(nb_liked_movies);
                    }
                    //
                    add_vects_100_with_coeff(&(film_vectors[id_film_1]), &(film_vectors[id_film_2]), sum_coefficient, true);
                    if(sum_and_normalize_both){
                        add_vects_100_with_coeff(&(film_vectors[id_film_2]), &(film_vectors[id_film_1]), sum_coefficient, true);
                    }
                }
            }
        }
    }
    //
    return film_vectors;
}

void prepare_algo2(MainData* data){
    //
    printf("Prepare 1...\n");
    MovieVect10* lst1 = random_vectors_10_algorithm(data, 3, 5, 0.1, false);
    //
    FILE* f1 = fopen("data_algo_2/v10_vects_2.bin", "wb");
    if(f1 == NULL){
        fprintf(stderr, "\e[31m Error ! File is NULL (prepare_algo2 - 1) \e[m\n");
        exit(42);
    }
    fwrite(lst1, sizeof(MovieVect10), NUMBER_OF_MOVIES_PLUS_UN, f1);
    fclose(f1);
    free(lst1);
    //
    printf("Prepare 2...\n");
    MovieVect100* lst2 = random_vectors_100_algorithm(data, 3, 50, 0.05, false);
    FILE* f2 = fopen("data_algo_2/v100_vects_2.bin", "wb");
    if(f2 == NULL){
        fprintf(stderr, "\e[31m Error ! File is NULL (prepare_algo2 - 2) \e[m\n");
        exit(42);
    }
    fwrite(lst2, sizeof(MovieVect100), NUMBER_OF_MOVIES_PLUS_UN, f2);
    fclose(f2);
    free(lst2);
    //
    printf("Done !\n");
}

MovieVect10* load_algo_list_v10(const char path[]){
    FILE* f = fopen(path, "rb");
    if(f == NULL){
        fprintf(stderr, "\e[31m Error ! File is NULL (load_algo_list) \e[m\n");
        exit(42);
    }
    MovieVect10* v = (MovieVect10*)calloc(NUMBER_OF_MOVIES + 1, sizeof(MovieVect10));
    fread(v, sizeof(MovieVect10), NUMBER_OF_MOVIES + 1, f);
    if(v == NULL){
        fprintf(stderr, "\e[31m Error ! v is NULL (load_algo_list) \e[m\n");
        exit(42);
    }
    fclose(f);
    //
    return v;
}

MovieVect100* load_algo_list_v100(const char path[]){
    FILE* f = fopen(path, "rb");
    if(f == NULL){
        fprintf(stderr, "\e[31m Error ! File is NULL (load_algo_list) \e[m\n");
        exit(42);
    }
    MovieVect100* v = (MovieVect100*)calloc(NUMBER_OF_MOVIES + 1, sizeof(MovieVect100));
    fread(v, sizeof(MovieVect100), NUMBER_OF_MOVIES + 1, f);
    if(v == NULL){
        fprintf(stderr, "\e[31m Error ! v is NULL (load_algo_list) \e[m\n");
        exit(42);
    }
    fclose(f);
    return v;
}

/*
The first half of predicted movies will be with all the inputs, and the second half will be with individual input movies
*/
void algo2_random_indexing_v10(const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis){
    // Loading movie vectors
    MovieVect10* movie_vects = load_algo_list_v10("data_algo_2/v10_vects.bin");
    //
    MovieVect10 input_sum_vect = {.dim = 10, .vect = {0}};
    //
    for(int i=0; i < nb_preds_demandes/2; i++){
        int min_movie = -1;
        double min_dist = 1000;
        for(int id_film = 1; id_film <= NUMBER_OF_MOVIES; id_film++){
            // Check if movie is not in input and not currently predicted
            bool skip = false;
            for(int j = 0; j < args->nb_inputs; j++){
                if(args->input_id_movies[j] == id_film){
                    skip = true;
                    break;
                }
            }
            for(int j = 0; j < *nb_predis; j++){
                if(preds_id_movie[j] == id_film){
                    skip = true;
                    break;
                }
            }
            if(skip){
                continue;
            }
            //
            double current_dist = movie_vect_10_dist(&(input_sum_vect), &(movie_vects[id_film]));
            //
            if(min_movie == -1 || current_dist < min_dist){
                min_movie = id_film;
                min_dist = current_dist;
            }
        }
        //
        if(min_movie != -1){
            preds_id_movie[*nb_predis] = min_movie;
            (*nb_predis)++;
        }
    }
    //
    for(int i=nb_preds_demandes/2; i < nb_preds_demandes; i++){
        //
        int id_random_movie = args->input_id_movies[custom_size_t_randint(args->nb_inputs)];
        MovieVect10 inp_vect = movie_vects[id_random_movie];
        //
        int min_movie = -1;
        double min_dist = DBL_MAX;
        //
        for(int id_film = 1; id_film <= NUMBER_OF_MOVIES; id_film++){
            // Check if movie is not in input and not currently predicted
            bool skip = false;
            for(int j = 0; j < args->nb_inputs; j++){
                if(args->input_id_movies[j] == id_film){
                    skip = true;
                    break;
                }
            }
            for(int j = 0; j < *nb_predis; j++){
                if(preds_id_movie[j] == id_film){
                    skip = true;
                    break;
                }
            }
            if(skip){
                continue;
            }
            //
            double current_dist = movie_vect_10_dist(&(inp_vect), &(movie_vects[id_film]));
            //
            if(min_movie == -1 || current_dist < min_dist){
                min_movie = id_film;
                min_dist = current_dist;
            }
        }
        //
        if(min_movie != -1){
            preds_id_movie[*nb_predis] = min_movie;
            (*nb_predis)++;
        }
    }
    //
}


/*
The first half of predicted movies will be with all the inputs, and the second half will be with individual input movies
*/
void algo2_random_indexing_v100(const args_t* args, int* preds_id_movie, int nb_preds_demandes, int* nb_predis){
    // Loading movie vectors
    MovieVect100* movie_vects = load_algo_list_v100("data_algo_2/v100_vects.bin");
    //
    MovieVect100 input_sum_vect = {.dim = 100, .vect = {0}};
    for(int i = 0; i < args->nb_inputs; i++){
        add_vects_100_with_coeff(&(input_sum_vect), &(movie_vects[args->input_id_movies[i]]), 1, true);
    }
    //
    for(int i=0; i < nb_preds_demandes/2; i++){
        int min_movie = -1;
        double min_dist = 0;
        for(int id_film = 1; id_film <= NUMBER_OF_MOVIES; id_film++){
            // Check if movie is not in input and not currently predicted
            bool skip = false;
            for(int j = 0; j < args->nb_inputs; j++){
                if(args->input_id_movies[j] == id_film){
                    skip = true;
                    break;
                }
            }
            for(int j = 0; j < *nb_predis; j++){
                if(preds_id_movie[j] == id_film){
                    skip = true;
                    break;
                }
            }
            if(skip){
                continue;
            }
            //
            double current_dist = movie_vect_100_dist(&(input_sum_vect), &(movie_vects[id_film]));
            //
            if(min_movie == -1 || current_dist < min_dist){
                min_movie = id_film;
                min_dist = current_dist;
            }
        }
        //
        if(min_movie != -1){
            preds_id_movie[*nb_predis] = min_movie;
            (*nb_predis)++;
        }
    }
    //
    for(int i=nb_preds_demandes/2; i < nb_preds_demandes; i++){
        //
        int id_random_movie = args->input_id_movies[custom_size_t_randint(args->nb_inputs)];
        MovieVect100 inp_vect = movie_vects[id_random_movie];
        //
        int min_movie = -1;
        double min_dist = 0;
        for(int id_film = 1; id_film <= NUMBER_OF_MOVIES; id_film++){
            // Check if movie is not in input and not currently predicted
            bool skip = false;
            for(int j = 0; j < args->nb_inputs; j++){
                if(args->input_id_movies[j] == id_film){
                    skip = true;
                    break;
                }
            }
            for(int j = 0; j < *nb_predis; j++){
                if(preds_id_movie[j] == id_film){
                    skip = true;
                    break;
                }
            }
            if(skip){
                continue;
            }
            //
            double current_dist = movie_vect_100_dist(&(inp_vect), &(movie_vects[id_film]));
            //
            if(min_movie == -1 || current_dist < min_dist){
                min_movie = id_film;
                min_dist = current_dist;
            }
        }
        //
        if(min_movie != -1){
            preds_id_movie[*nb_predis] = min_movie;
            (*nb_predis)++;
        }
    }
    //
}
