#pragma once

#include "constants.h"
#include <stdbool.h>
#include "opt.h"

#include "lib_ui_sdl.h"

/*
---- PARSER ----
• PARSER :              the option -p PARSER tells if we want to use the parser or the prediction algorithms
---- ALGOS PREDICTION ----
• LIKED_MOVIES :             the option -r LIKED_MOVIES precise the movies the clients did like
• NB_PREDICTIONS :          the option -n NB_PREDICTIONS precise the number of predictions the algorithm must generate
• PATH_TO_SAVE_RESULTS :    the option -f PATH_TO_SAVE_RESULTS precise the path where the results files will be saved
• LIMIT_DATE :              the option -l LIMIT_DATE will filter on the movie that are less recent than LIMIT_DATE
• STATS_MOVIE_ID :           the option -s STATS_MOVIE_ID will give informations on the movie of the id STATS_MOVIE_ID 
• CLIENTS_SELECTION :       the option -c "Client1, Client2, ..." will filter only the clients given
• BAD_CLIENTS :             the option -b BAD_CLIENTS will filter negatively the clients given in BAD_CLIENTS
• ELITE_CLIENTS :           the option -e MIN_MOVIES_VIEWED_BY_CLIENTS will filter only the clients who viewed more than MIN_MOVIES_VIEWED_BY_CLIENTS
• EXEC_TIME :               the option -t will tell the execution time of the algorithm
---- ----
*/

typedef enum flags_t {
    NO_FLAGS = 0,
    FLAG_PARSER = 1,
    FLAG_FOLDER = 1 << 2,
    FLAG_LIMIT_DATE = 1 << 3,
    FLAG_STATS_MOVIE_ID = 1 << 4,
    FLAG_CLIENTS_SELECTION = 1 << 5,
    FLAG_BAD_CLIENTS = 1 << 6,
    FLAG_ELITE_CLIENTS = 1 << 7,
    FLAG_EXEC_TIME = 1 << 8,
    FLAG_LIKED_MOVIES = 1 << 9,
    FLAG_NB_PREDICTIONS = 1 << 10,
    FLAG_SPECIFIED_ALGORITHM = 1 << 11,
    FLAG_MODE_WINDOW = 1 << 12,
    FLAG_MODE_CONSOLE = 1 << 13,
} flags_t;

typedef struct args_t {
    flags_t flags;
    char paths_to_save_results[5000];
    unsigned long limit_date;
    int stats_movie_id;
    char clients_selection[5000];
    char bad_clients[5000];
    int min_movies_viewed_by_clients;
    char liked_movies[5000];
    int nb_predictions;
    int algorithm_of_prediction;
    //
    int input_id_movies[NUMBER_OF_MOVIES_PLUS_UN];
    int nb_inputs;
    bool movies_selected[NUMBER_OF_MOVIES_PLUS_UN];
    //
    Ui_Element* algo_name_txt;
    Ui_Element* cb_filter_only_selected;
    //
} args_t;


// Init args
void init_args(args_t* args);


/**
 * @brief Parse the arguments
 * @param argc (standard main argument)
 * @param argv (standard main argument)
 * @param args will receive the argument values
 * @return OK if everything went well or ERROR_* if there was an error
 */
int
parse_args(int argc,  char ** argv, args_t *args);


/**
 * @brief print the usage
 */
void
print_usage(void);

