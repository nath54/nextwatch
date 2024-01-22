#include "args_parser.h"
#include "parser.h"
#include "data.h"
#include "utils.h"
#include "main_algos.h"
#include "opt.h"
#include "ui_main_ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void process_arguments(MainData* main_data, args_t* args){
    // Test Arguments Liked movies
    if(args->flags & FLAG_LIKED_MOVIES){
        int entry_type=0;
        get_list_of_liked_movies(args->liked_movies, &entry_type, args->input_id_movies, &(args->nb_inputs));
        printf("Liked Movies : '%s' (nb=%d) (entry_type=%d)\n", args->liked_movies, args->nb_inputs, entry_type);
        bool pas_bon = false;
        for(int i=0; i<args->nb_inputs; i++){
            printf("Movie : %d - %s \n", args->input_id_movies[i], main_data->movies[args->input_id_movies[i]-1].name);
            if(args->input_id_movies[i] >= NUMBER_OF_MOVIES || args->input_id_movies[i] <= 0) pas_bon = true;
        }
        if(pas_bon){
            printf("Il y a eu une erreur de lecture d'arguments !!!\n");
            exit(42);
        }
    }
    // Test Arguments Nb Predictions
    if(args->flags & FLAG_NB_PREDICTIONS){
        printf("Nb predictions : %d\n", args->nb_predictions);
    }
    // Test Arguments Paths_to_save_results
    if(args->flags & FLAG_FOLDER){
        printf("Path to save results : '%s'\n", args->paths_to_save_results);
    }
    // Test Arguments Limit Date
    if(args->flags & FLAG_LIMIT_DATE){
        printf("Limit date : %ld\n", args->limit_date);
    }
    // Test Arguments Stats Movie Id
    if(args->flags & FLAG_STATS_MOVIE_ID){
        printf("Stats Movie Id : %d\n", args->stats_movie_id);
    }
    // Test Argument Clients Selection
    if(args->flags & FLAG_CLIENTS_SELECTION){
        printf("Clients Selection : '%s'\n", args->clients_selection);
    }
    // Test Arguments Bad Clients
    if(args->flags & FLAG_BAD_CLIENTS){
        printf("Clients Bad : '%s'\n", args->bad_clients);
    }
    // Test Arguments Elite Clients
    if(args->flags & FLAG_ELITE_CLIENTS){
        printf("Clients Elite : %d\n", args->min_movies_viewed_by_clients);
    }
    // Test Arguments Exec Time
    if(args->flags & FLAG_EXEC_TIME){
        printf("Exec Time\n");
    }
    // Test Arguments Algorithm
    printf("Selected Algorithm : %d\n", args->algorithm_of_prediction);
    //
    for(int i=0; i<args->nb_inputs; i++){
        args->movies_selected[args->input_id_movies[i]] = true;
    }
    //
}


void console_mode(MainData* main_data, args_t* args){
    // Clearing the screen;
    printf("\033[2J");
    //
    printf("##################################\n");
    printf("#    NEXTWATCH  - CONSOLE MODE   #\n");
    printf("##################################\n");
    printf("----------------------------------\n");
    printf("-------------------------------\n");
    printf("- A movie prediction app made\n");
    printf("- by CERISARA Nathan\n");
    printf("- and LAFONT Loic\n");
    printf("--------\n");
    printf("---\n");
    printf("-\n\n");

    printf("Enter 'q' or 'quit' or 'exit' to quit the program\n\n");

    printf("Enter here a command for movie prediction.\nIt is meant to be used like the command to launch the app (./bin/main ...args...), but just without the (./bin/main).\n'-h' for help with the parameters.\n=]");
    char** my_argv = (char**)calloc(200, sizeof(char*));
    for(int i=0; i<200; i++){
        my_argv[i] = (char*)calloc(200, sizeof(char));
    }
    int* my_argv_length = (int*)calloc(200, sizeof(int));
    int my_argc = 0;
    //
    char input[5000];
    scanf("%[^\n]", input);
    getc(stdin);
    //
    while(strcmp(input, "q") || strcmp(input, "quit") || strcmp(input, "exit")){
        // Cleaning argv & argc
        for(int i=0; i<my_argc; i++){
            my_argv[i][0] = '\0';
            my_argv_length[i] = 0;
        }
        my_argc = 0;
        // On compte le nombre d'espaces
        int current_argv = 1;
        bool guillemet_ouvrant = false;
        size_t input_length = strlen(input);
        for(size_t i=0; i<input_length; i++){
            if(input[i] == '"'){
                guillemet_ouvrant = !guillemet_ouvrant;
                my_argv[current_argv][my_argv_length[current_argv]] = '\0';
                current_argv++;
                continue;
            }
            if(input[i] == ' ' && !guillemet_ouvrant){
                my_argv[current_argv][my_argv_length[current_argv]] = '\0';
                current_argv++;
                continue;
            }
            //
            my_argv[current_argv][my_argv_length[current_argv]] = input[i];
            my_argv_length[current_argv]++;
        }
        //
        my_argv[current_argv][my_argv_length[current_argv]] = '\0';
        current_argv++;
        my_argc = current_argv;
        //
        init_args(args);
        //
        parse_args(my_argc, my_argv, args);
        //
        process_arguments(main_data, args);
        //
        static_print_algos(args, main_data);
        //
        printf("\n=]");
        scanf("%[^\n]", input);
        getc(stdin);
        //
    }
    //

    for(int i=0; i<200; i++){
        free(my_argv[i]);
    }
    free(my_argv);
    free(my_argv_length);
}

int main(int argc, char** argv)
{
    //
    srand(time(NULL));
    //
    args_t* args = (args_t*)calloc(1, sizeof(args_t));
    init_args(args);
    //
    parse_args(argc, argv, args);
    
    // Test Arguments Parser
    if(args->flags & FLAG_PARSER){
        printf("Parser!\n");
        main_parser();
        return 0;
    }
    //
    printf("Not parser!\n");
    // Loading data
    MainData* main_data = load_data();
    //
    process_arguments(main_data, args);
    //
    // prepare_algo2(main_data);
    //
    if(args->flags & FLAG_MODE_WINDOW){
        // Window mode
        main_ui(args, main_data);
    }
    else{
        if(args->flags & FLAG_MODE_CONSOLE){
            // Console mode
            console_mode(main_data, args);
        }
        else{
            // Default option
            static_print_algos(args, main_data);
        }
    }
    //
    free(args->input_id_movies);
    free(args);
    free_data(main_data);
    //
    return 0;
}

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
