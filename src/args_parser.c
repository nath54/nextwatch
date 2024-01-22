#include "args_parser.h"

//#include <unistd.h>
//#include <getopt.h>
#include "opt.h"

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Init args
void init_args(args_t* args){
    args->flags=0;
    args->clients_selection[0]='\0';
    args->paths_to_save_results[0]='\0';
    args->stats_movie_id=-1;
    for(int i=0; i<5000; i++){
        args->liked_movies[i]='\0';
    }
    args->limit_date=0;
    args->bad_clients[0]='\0';
    args->min_movies_viewed_by_clients=-1;
    args->nb_predictions=10;
    args->algorithm_of_prediction = -1;
    args->nb_inputs = 0;
    //
    for(int i=0; i<NUMBER_OF_MOVIES_PLUS_UN; i++){
        args->input_id_movies[i] = 0;
    }
}


// Parse args
int parse_args(int argc,  char ** argv, args_t *args)
{
    args->flags = NO_FLAGS;

    // char* endPtr;

    opterr = 0;

    /*
    ---- PARSER ----
    • PARSER :                  the option -p PARSER tells if we want to use the parser or the prediction algorithms
    ---- ALGOS PREDICTION ----
    • LIKED_MOVIES :            the option -r LIKED_MOVIES precise the movies the clients did like
    • NB_PREDICTIONS :          the option -n NB_PREDICTIONS precise the number of predictions the algorithm must generate
    • EXEC_TIME :               the option -t will tell the execution time of the algorithm
    • ALGORITHM :               the option -a ALGORITHM will choose an algorithm from ['random', 'partial_clustering', 'random_indexing_v10', 'random_indexing_v100', 'good_random', 'movie_from_movie']
    • APP MODE :                the option -m MODE will select an application mode from ['window', 'console']
    ---- ----
    */

    // printf("First time : \n");

    const char shortopts[] = "hpf:l:s:c:b:e:tr:n:a:m:";
    
    int c = getopt (argc, argv, shortopts);

    while (c != -1)
    {

        // printf("Using OPT : %d = '%c' \n", c, c);
        switch (c)
        {
            case 'h':
                print_usage();
                exit(0);
                
            case 'p':
                args->flags |= FLAG_PARSER;
                break;
            case 't':
                args->flags |= FLAG_EXEC_TIME;
                break;
            case 'r':
                args->flags |= FLAG_LIKED_MOVIES;
                // args->liked_movies = optarg;
                strcpy(args->liked_movies, optarg);
                break;
            case 'n':
                args->flags |= FLAG_NB_PREDICTIONS;
                args->nb_predictions = atoi(optarg);
                break;
            case 'a':
                args->flags |= FLAG_SPECIFIED_ALGORITHM;
                if(strcmp(optarg, "random")==0){
                    args->algorithm_of_prediction = 0;
                    break;
                }
                if(strcmp(optarg, "partial_clustering")==0){
                    args->algorithm_of_prediction = 1;
                    break;
                }
                if(strcmp(optarg, "random_indexing_v10")==0){
                    args->algorithm_of_prediction = 2;
                    break;
                }
                if(strcmp(optarg, "random_indexing_v100")==0){
                    args->algorithm_of_prediction = 3;
                    break;
                }
                if(strcmp(optarg, "random_good_movies")==0){
                    args->algorithm_of_prediction = 4;
                    break;
                }
                if(strcmp(optarg, "movie_from_movie")==0){
                    args->algorithm_of_prediction = 4;
                    break;
                }
                // If we are here, it is because the user asked an algorithm that doesn't exist
                printf("\n\nYou aksed an algorithm that doesn't exist, here are the existing algorithms : \n");
                printf("   - 'random' : predicts random movies, very fast, you will discover a lot of new movies of all genders that can be interesting or boring!\n");
                printf("   - 'partial_clustering' : Dynamic user's cluster calculation, and next movie predictions from this cluster !\n");
                printf("   - 'random_indexing_v10' : Pre-calculated embeddings for the movies with the method of the random indexing, the embeddings have 10 dimensions !\n");
                printf("   - 'random_indexing_v100' : Pre-calculated embeddings for the movies with the method of the random indexing, the embeddings have 100 dimensions !\n");
                printf("   - 'random_good_movies' : Select movies randomly from a list of good liked movies\n");
                printf("   - 'movies_from_films_users' : Select randomly an user from the users that liked one of the movies in input and give another movie that is liked by this user\n");
                printf("\n");
                exit(0);
            case 'm':
                // Case window
                if(strcmp(optarg, "window") == 0){
                    // printf("\e[38m Windows mode !\e[m\n");
                    args->flags |= FLAG_MODE_WINDOW;
                    break;
                }
                // Case console
                if(strcmp(optarg, "console") == 0){
                    // printf("\e[38m Console mode !\e[m\n");
                    args->flags |= FLAG_MODE_CONSOLE;
                    break;
                }
                // Default
                fprintf(stderr, "\e[31mError : Unknow mode : '%s'. The two modes are 'window' or 'console'. Otherwise, remove the \"-m\" option.\e[m\n", optarg);
                exit(42);
                break;

            case '?':

                if (optopt == 'f')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
                else
                {
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }

                print_usage();
                exit(1);

            default:
                fprintf (stderr,
                        "Unknown option character `%c'.\n",
                        c);
                print_usage();
                exit(1);
        }

        //
        c = getopt (argc, argv, shortopts);
    }
        

    return 0;
}


void print_usage(void)
{
    puts(
        "Usage : \n"
        "---- PARSER ----\n"
        "• PARSER :                  the option -p PARSER tells if we want to use the parser or the prediction algorithms\n"
        "---- ALGOS PREDICTION ----\n"
        "• LIKED_MOVIES :            the option -r LIKED_MOVIES precise the movies the clients did like\n"
        "• NB_PREDICTIONS :          the option -n NB_PREDICTIONS precise the number of predictions the algorithm must generate\n"
        "• EXEC_TIME :               the option -t will tell the execution time of the algorithm\n"
        "• ALGORITHM :               the option -a ALGORITHM will choose an algorithm from ['random', 'partial_clustering', 'random_indexing_v10', 'random_indexing_v100', 'good_random', 'movie_from_movie']\n"
        "• APP MODE :                the option -m MODE will select an application mode from ['window', 'console']\n"
        "---- ----\n"
    );
}
