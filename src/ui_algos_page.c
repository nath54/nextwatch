#include "ui_algos_page.h"

typedef struct{
    //
    Ui_Element* bt_random;
    Ui_Element* bt_partial_clustering;
    Ui_Element* bt_random_indexing_v10;
    Ui_Element* bt_random_indexing_v100;
    Ui_Element* bt_random_good;
    Ui_Element* bt_movie_from_movie;
    //
    Ui_Element* descr_random;
    Ui_Element* descr_partial_clustering;
    Ui_Element* descr_random_indexing_v10;
    Ui_Element* descr_random_indexing_v100;
    Ui_Element* descr_random_good;
    Ui_Element* descr_movie_from_movie;
    //
    Ui* ui;
    int new_num;
    args_t* args;
    //
} Arg_algo_selection;

void on_algorithm_selection(Arg_algo_selection* arg){
    if(arg->new_num < 0 || arg->new_num > 5){
        fprintf(stderr, "\e[31mError: Bad algorithm id: %d\e[m\n", arg->new_num);
        return;
    }

    arg->args->algorithm_of_prediction = arg->new_num;

    // Reset visibility for all descriptions
    arg->descr_random->visible = false;
    arg->descr_partial_clustering->visible = false;
    arg->descr_random_indexing_v10->visible = false;
    arg->descr_random_indexing_v100->visible = false;
    arg->descr_random_good->visible = false;
    arg->descr_movie_from_movie->visible = false;

    // Reset color for all buttons
    set_color(&(arg->bt_random->color), 25, 25, 25, 255);
    set_color(&(arg->bt_partial_clustering->color), 25, 25, 25, 255);
    set_color(&(arg->bt_random_indexing_v10->color), 25, 25, 25, 255);
    set_color(&(arg->bt_random_indexing_v100->color), 25, 25, 25, 255);
    set_color(&(arg->bt_random_good->color), 25, 25, 25, 255);
    set_color(&(arg->bt_movie_from_movie->color), 25, 25, 25, 255);

    // Update visibility and color based on the selected algorithm
    switch (arg->new_num) {
        case 0:
            set_color(&(arg->bt_random->color), 0, 25, 0, 255);
            ui_update_elt_text(arg->ui, arg->args->algo_name_txt, "Random Algorithm");
            arg->descr_random->visible = true;
            break;
        case 1:
            set_color(&(arg->bt_partial_clustering->color), 0, 25, 0, 255);
            ui_update_elt_text(arg->ui, arg->args->algo_name_txt, "Partial Algorithm Algorithm");
            arg->descr_partial_clustering->visible = true;
            break;
        case 2:
            set_color(&(arg->bt_random_indexing_v10->color), 0, 25, 0, 255);
            ui_update_elt_text(arg->ui, arg->args->algo_name_txt, "Random Indexing (v10) Algorithm");
            arg->descr_random_indexing_v10->visible = true;
            break;
        case 3:
            set_color(&(arg->bt_random_indexing_v100->color), 0, 25, 0, 255);
            ui_update_elt_text(arg->ui, arg->args->algo_name_txt, "Random Indexing (v100) Algorithm");
            arg->descr_random_indexing_v100->visible = true;
            break;
        case 4:
            set_color(&(arg->bt_random_good->color), 0, 25, 0, 255);
            ui_update_elt_text(arg->ui, arg->args->algo_name_txt, "Good Movies Random Algorithm");
            arg->descr_random_good->visible = true;
            break;
        case 5:
            set_color(&(arg->bt_movie_from_movie->color), 0, 25, 0, 255);
            ui_update_elt_text(arg->ui, arg->args->algo_name_txt, "Movie From Movie Algorithm");
            arg->descr_movie_from_movie->visible = true;
            break;
        default:
            break;
    }
    //
}

void ui_init_algos_page(Ui* ui, args_t* args){
    
    // Colors
    const SDL_Color cl_bg = ui_cl(0, 0, 0, 255);
    const SDL_Color cl2 = ui_cl(60, 36, 103, 255);
    
    // ################################
    // #########  Background  #########
    // ################################

    ui_set_bg_color(ui, cl_bg.r, cl_bg.g, cl_bg.b);

    // ############################
    // #########  Header  #########
    // ############################

    // Container Header
    Ui_Element* _container_header = ui_create_container(ui, "container_header", 0, 0, 1800, 75, UI_OVERFLOW_HIDE, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_header);

    // Logo
    Ui_Element* _logo = ui_create_texture(ui, "logo", 10, 10, 100, 50, "./res/ui/textures/nextwatch_transparent.png");
    ui_append_elt_to_elt(_container_header, _logo);

    // Button Movies
    Ui_Element* _bt_header_movies = ui_create_bt_rect(ui, "bt_header_movies", 1500, 10, 125, 50, false, COLOR_VERY_DARK_GREY, "movies", COLOR_WHITE, 18, 0);
    
    Arg_ui_and_int* arg_chg_win_to_movies_1 = (Arg_ui_and_int*)calloc(1, sizeof(Arg_ui_and_int));
    arg_chg_win_to_movies_1->ui = ui;
    arg_chg_win_to_movies_1->number = 1;
    ui_add_on_elt_click_event(_bt_header_movies, 1, true, (void (*)(void*))change_current_windows, arg_chg_win_to_movies_1);
    
    ui_append_elt_to_elt(_container_header, _bt_header_movies);

    // Button Algos
    Ui_Element* _bt_header_algos = ui_create_bt_rect(ui, "bt_header_algos", 1650, 10, 125, 50, false, COLOR_DARK_GREY, "algos", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_header, _bt_header_algos);


    // #########################################
    // #########  Algorithm Selection  #########
    // #########################################

    // Text Algorithm Selection
    Ui_Element* _text_algorithm_selection = ui_create_text(ui, "text_algorithm_selection", 240, 100, "Algorithm Selection", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, _text_algorithm_selection);

    // Container Algorithm Selection
    Ui_Element* _container_algorithm_selection = ui_create_container(ui, "container_algorithm_selection", 20, 140, 590, 340, UI_OVERFLOW_HIDE, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_algorithm_selection);

    // Bg Algorithm Selection
    Ui_Element* _bg_algorithm_selection = ui_create_rect(ui, "bg_algorithm_selection", 0, 0, 590, 340, true, COLOR_VERY_DARK_GREY);
    ui_append_elt_to_elt(_container_algorithm_selection, _bg_algorithm_selection);

    // Bt Random Algo
    Ui_Element* _bt_random_algo = ui_create_bt_rect(ui, "bt_random_algo", 10, 10, 570, 40, true, COLOR_BLACK, "Random Algorithm", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_algorithm_selection, _bt_random_algo);

    // Bt Partial Clustering Algo
    Ui_Element* _bt_partial_clustering_algo = ui_create_bt_rect(ui, "bt_partial_clustering", 10, 60, 570, 40, true, COLOR_BLACK, "Partial Clustering Algorithm", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_algorithm_selection, _bt_partial_clustering_algo);
    
    // Bt Random Indexing (v10) Algo
    Ui_Element* _bt_random_indexing_v10_algo = ui_create_bt_rect(ui, "bt_random_indexing_v10_algo", 10, 110, 570, 40, true, COLOR_BLACK, "Random Indexing (v10) Algorithm", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_algorithm_selection, _bt_random_indexing_v10_algo);

    // Bt Random Indexing (v100) Algo
    Ui_Element* _bt_random_indexing_v100_algo = ui_create_bt_rect(ui, "bt_random_indexing_v100_algo", 10, 160, 570, 40, true, COLOR_BLACK, "Random Indexing (v100) Algorithm", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_algorithm_selection, _bt_random_indexing_v100_algo);

    // Bt Good Movies Random Algo
    Ui_Element* _bt_random_good_algo = ui_create_bt_rect(ui, "bt_good_movies_random_algo", 10, 210, 570, 40, true, COLOR_BLACK, "Good Movies Random Algorithm", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_algorithm_selection, _bt_random_good_algo);

    // Bt Movie from Movie Algo
    Ui_Element* _bt_movie_from_movie_algo = ui_create_bt_rect(ui, "bt_movie_from_movie_random_algo", 10, 260, 570, 40, true, COLOR_BLACK, "Movie From Movie Algorithm", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_algorithm_selection, _bt_movie_from_movie_algo);

    // ###########################################
    // #########  Algorithm Description  #########
    // ###########################################

    // Text Algorithm Description
    Ui_Element* _text_algorithm_description = ui_create_text(ui, "text_algorithm_description", 480, 500, "Description", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, _text_algorithm_description);

    // Container Algorithm Description
    Ui_Element* _container_algorithm_description = ui_create_container(ui, "container_algorithm_description", 20, 540, 1200, 340, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_algorithm_description);

    // Bg Algorithm Description
    Ui_Element* _bg_algorithm_description = ui_create_rect(ui, "bg_algorithm_description", 0, 0, 1200, 340, true, COLOR_VERY_DARK_GREY);
    ui_append_elt_to_elt(_container_algorithm_description, _bg_algorithm_description);

    // ######## Random Algorithm Description ########

    Ui_Element* _container_random_algorithm_description = ui_create_container(ui, "container_random_algorithm_description", 10, 10, 980, 320, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);
    _container_random_algorithm_description->visible = false;
    ui_append_elt_to_elt(_container_algorithm_description, _container_random_algorithm_description);

    Ui_Element* _random_algorithm_description_title = ui_create_text(ui, "random_algorithm_description_title", 10, 10, "Random Algorithm Description : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_random_algorithm_description, _random_algorithm_description_title);

    Ui_Element* _random_algorithm_description_text_1 = ui_create_text(ui, "random_algorithm_description_text_1", 10, 40, "Predicts random movies, very fast, you will discover a lot of new movies of all genders that can be interesting or boring!", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_random_algorithm_description, _random_algorithm_description_text_1);

    // ######## Partial Clustering Algorithm Description ########

    Ui_Element* _container_partial_clustering_algorithm_description = ui_create_container(ui, "container_partial_clustering_algorithm_description", 10, 10, 980, 320, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);
    _container_partial_clustering_algorithm_description->visible = false;
    ui_append_elt_to_elt(_container_algorithm_description, _container_partial_clustering_algorithm_description);

    Ui_Element* _partial_clustering_algorithm_description_title = ui_create_text(ui, "partial_clustering_algorithm_description_title", 10, 10, "Partial Clustering Algorithm Description : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_partial_clustering_algorithm_description, _partial_clustering_algorithm_description_title);

    Ui_Element* _partial_clustering_algorithm_description_text_1 = ui_create_text(ui, "partial_clustering_algorithm_description_text_1", 10, 40, "Dynamic user's cluster calculation, and next movie predictions from this cluster !", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_partial_clustering_algorithm_description, _partial_clustering_algorithm_description_text_1);

    // ######## Random Indexing v10 Algorithm Description ########

    Ui_Element* _container_random_indexing_v10_algorithm_description = ui_create_container(ui, "container_random_indexing_v10_algorithm_description", 10, 10, 980, 320, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);
    _container_random_indexing_v10_algorithm_description->visible = false;
    ui_append_elt_to_elt(_container_algorithm_description, _container_random_indexing_v10_algorithm_description);

    Ui_Element* _random_indexing_v10_algorithm_description_title = ui_create_text(ui, "random_indexing_v10_algorithm_description_title", 10, 10, "Random Indexing v10 Algorithm Description : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_random_indexing_v10_algorithm_description, _random_indexing_v10_algorithm_description_title);

    Ui_Element* _random_indexing_v10_algorithm_description_text_1 = ui_create_text(ui, "random_indexing_v10_algorithm_description_text_1", 10, 40, "Pre-calculated embeddings for the movies with the method of the random indexing, the embeddings have 10 dimensions !", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_random_indexing_v10_algorithm_description, _random_indexing_v10_algorithm_description_text_1);

    // ######## Random Indexing v100 Algorithm Description ########

    Ui_Element* _container_random_indexing_v100_algorithm_description = ui_create_container(ui, "container_random_indexing_v100_algorithm_description", 10, 10, 980, 320, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);
    _container_random_indexing_v100_algorithm_description->visible = false;
    ui_append_elt_to_elt(_container_algorithm_description, _container_random_indexing_v100_algorithm_description);

    Ui_Element* _random_indexing_v100_algorithm_description_title = ui_create_text(ui, "random_indexing_v100_algorithm_description_title", 10, 10, "Random Indexing v100 Algorithm Description : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_random_indexing_v100_algorithm_description, _random_indexing_v100_algorithm_description_title);

    Ui_Element* _random_indexing_v100_algorithm_description_text_1 = ui_create_text(ui, "random_indexing_v100_algorithm_description_text_1", 10, 40, "Pre-calculated embeddings for the movies with the method of the random indexing, the embeddings have 100 dimensions !", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_random_indexing_v100_algorithm_description, _random_indexing_v100_algorithm_description_text_1);
    
    // ######## Good Random Algorithm Description ########

    Ui_Element* _container_good_random_algorithm_description = ui_create_container(ui, "container_good_random_algorithm_description", 10, 10, 980, 320, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);
    _container_good_random_algorithm_description->visible = false;
    ui_append_elt_to_elt(_container_algorithm_description, _container_good_random_algorithm_description);

    Ui_Element* _good_random_algorithm_description_title = ui_create_text(ui, "good_random_algorithm_description_title", 10, 10, "Good Movies Random Algorithm Description : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_good_random_algorithm_description, _good_random_algorithm_description_title);

    Ui_Element* _good_random_algorithm_description_text_1 = ui_create_text(ui, "good_random_algorithm_description_text_1", 10, 40, "Select movies randomly from a list of good liked movies", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_good_random_algorithm_description, _good_random_algorithm_description_text_1);

    
    // ######## Movie From Movie Algorithm Description ########

    Ui_Element* _container_movie_from_movie_algorithm_description = ui_create_container(ui, "container_movie_from_movie_algorithm_description", 10, 10, 980, 320, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);
    _container_movie_from_movie_algorithm_description->visible = false;
    ui_append_elt_to_elt(_container_algorithm_description, _container_movie_from_movie_algorithm_description);

    Ui_Element* _movie_from_movie_algorithm_description_title = ui_create_text(ui, "movie_from_movie_algorithm_description_title", 10, 10, "Movie From Movie Algorithm Description : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_movie_from_movie_algorithm_description, _movie_from_movie_algorithm_description_title);

    Ui_Element* _movie_from_movie_algorithm_description_text_1 = ui_create_text(ui, "movie_from_movie_algorithm_description_text_1", 10, 40, "Select randomly an user from the users that liked one of the movies in input and give another movie that is liked by this user", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_movie_from_movie_algorithm_description, _movie_from_movie_algorithm_description_text_1);


    // ##########################################
    // #########  Algorithm Parameters  #########
    // ##########################################

    // Text Algorithm Parameters
    Ui_Element* _text_algorithm_parameters = ui_create_text(ui, "text_algorithm_parameters", 1500, 100, "Parameters", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, _text_algorithm_parameters);

    // Container Algorithm Parameters
    Ui_Element* _container_algorithm_parameters = ui_create_container(ui, "container_algorithm_parameters", 1300, 140, 480, 740, UI_OVERFLOW_HIDE, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_algorithm_parameters);

    // Bg Algorithm Parameters
    Ui_Element* _bg_algorithm_parameters = ui_create_rect(ui, "bg_algorithm_parameters", 0, 0, 480, 740, true, COLOR_VERY_DARK_GREY);
    ui_append_elt_to_elt(_container_algorithm_parameters, _bg_algorithm_parameters);

    // ###########################################
    // #########  Back to movies Button  #########
    // ###########################################

    // Back to movies button
    Ui_Element* _bt_back_to_movies = ui_create_bt_rect(ui, "bt_back_to_movies", 740, 140, 250, 50, true, cl2, "<- back to movies", COLOR_WHITE, 18, 0);
    Arg_ui_and_int* arg_chg_win_to_movies_2 = (Arg_ui_and_int*)calloc(1, sizeof(Arg_ui_and_int));
    arg_chg_win_to_movies_2->ui = ui;
    arg_chg_win_to_movies_2->number = 1;
    ui_add_on_elt_click_event(_bt_back_to_movies, 1, true, (void (*)(void*))change_current_windows, arg_chg_win_to_movies_2);
    ui_append_elt_to_ui(ui, _bt_back_to_movies);


    // ""##################### Events #####################""
    
    // Random 
    Arg_algo_selection* arg_random_clicked = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_random_clicked->ui = ui;
    arg_random_clicked->args = args;
    arg_random_clicked->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_random_clicked->bt_random = _bt_random_algo;
    arg_random_clicked->bt_random_good = _bt_random_good_algo;
    arg_random_clicked->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_random_clicked->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_random_clicked->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_random_clicked->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_random_clicked->descr_random = _container_random_algorithm_description;
    arg_random_clicked->descr_random_good = _container_good_random_algorithm_description;
    arg_random_clicked->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_random_clicked->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_random_clicked->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_random_clicked->new_num = 0;
    ui_add_on_elt_click_event(_bt_random_algo, 1, true, (void (*)(void*))on_algorithm_selection, arg_random_clicked);

    // Partial Clustering 
    Arg_algo_selection* arg_partial_clustering_clicked = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_partial_clustering_clicked->ui = ui;
    arg_partial_clustering_clicked->args = args;
    arg_partial_clustering_clicked->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_partial_clustering_clicked->bt_random = _bt_random_algo;
    arg_partial_clustering_clicked->bt_random_good = _bt_random_good_algo;
    arg_partial_clustering_clicked->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_partial_clustering_clicked->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_partial_clustering_clicked->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_partial_clustering_clicked->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_partial_clustering_clicked->descr_random = _container_random_algorithm_description;
    arg_partial_clustering_clicked->descr_random_good = _container_good_random_algorithm_description;
    arg_partial_clustering_clicked->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_partial_clustering_clicked->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_partial_clustering_clicked->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_partial_clustering_clicked->new_num = 1;
    ui_add_on_elt_click_event(_bt_partial_clustering_algo, 1, true, (void (*)(void*))on_algorithm_selection, arg_partial_clustering_clicked);

    // Random Indexing v1O
    Arg_algo_selection* arg_random_indexing_v10_clicked = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_random_indexing_v10_clicked->ui = ui;
    arg_random_indexing_v10_clicked->args = args;
    arg_random_indexing_v10_clicked->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_random_indexing_v10_clicked->bt_random = _bt_random_algo;
    arg_random_indexing_v10_clicked->bt_random_good = _bt_random_good_algo;
    arg_random_indexing_v10_clicked->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_random_indexing_v10_clicked->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_random_indexing_v10_clicked->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_random_indexing_v10_clicked->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_random_indexing_v10_clicked->descr_random = _container_random_algorithm_description;
    arg_random_indexing_v10_clicked->descr_random_good = _container_good_random_algorithm_description;
    arg_random_indexing_v10_clicked->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_random_indexing_v10_clicked->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_random_indexing_v10_clicked->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_random_indexing_v10_clicked->new_num = 2;
    ui_add_on_elt_click_event(_bt_random_indexing_v10_algo, 1, true, (void (*)(void*))on_algorithm_selection, arg_random_indexing_v10_clicked);

    // Random Indexing v10O
    Arg_algo_selection* arg_random_indexing_v100_clicked = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_random_indexing_v100_clicked->ui = ui;
    arg_random_indexing_v100_clicked->args = args;
    arg_random_indexing_v100_clicked->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_random_indexing_v100_clicked->bt_random = _bt_random_algo;
    arg_random_indexing_v100_clicked->bt_random_good = _bt_random_good_algo;
    arg_random_indexing_v100_clicked->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_random_indexing_v100_clicked->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_random_indexing_v100_clicked->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_random_indexing_v100_clicked->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_random_indexing_v100_clicked->descr_random = _container_random_algorithm_description;
    arg_random_indexing_v100_clicked->descr_random_good = _container_good_random_algorithm_description;
    arg_random_indexing_v100_clicked->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_random_indexing_v100_clicked->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_random_indexing_v100_clicked->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_random_indexing_v100_clicked->new_num = 3;
    ui_add_on_elt_click_event(_bt_random_indexing_v100_algo, 1, true, (void (*)(void*))on_algorithm_selection, arg_random_indexing_v100_clicked);
    
    // Good Random 
    Arg_algo_selection* arg_good_random_clicked = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_good_random_clicked->ui = ui;
    arg_good_random_clicked->args = args;
    arg_good_random_clicked->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_good_random_clicked->bt_random = _bt_random_algo;
    arg_good_random_clicked->bt_random_good = _bt_random_good_algo;
    arg_good_random_clicked->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_good_random_clicked->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_good_random_clicked->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_good_random_clicked->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_good_random_clicked->descr_random = _container_random_algorithm_description;
    arg_good_random_clicked->descr_random_good = _container_good_random_algorithm_description;
    arg_good_random_clicked->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_good_random_clicked->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_good_random_clicked->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_good_random_clicked->new_num = 4;
    ui_add_on_elt_click_event(_bt_random_good_algo, 1, true, (void (*)(void*))on_algorithm_selection, arg_good_random_clicked);

    // Movie From Movie 
    Arg_algo_selection* arg_movie_from_movie_clicked = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_movie_from_movie_clicked->ui = ui;
    arg_movie_from_movie_clicked->args = args;
    arg_movie_from_movie_clicked->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_movie_from_movie_clicked->bt_random = _bt_random_algo;
    arg_movie_from_movie_clicked->bt_random_good = _bt_random_good_algo;
    arg_movie_from_movie_clicked->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_movie_from_movie_clicked->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_movie_from_movie_clicked->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_movie_from_movie_clicked->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_movie_from_movie_clicked->descr_random = _container_random_algorithm_description;
    arg_movie_from_movie_clicked->descr_random_good = _container_good_random_algorithm_description;
    arg_movie_from_movie_clicked->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_movie_from_movie_clicked->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_movie_from_movie_clicked->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_movie_from_movie_clicked->new_num = 5;
    ui_add_on_elt_click_event(_bt_movie_from_movie_algo, 1, true, (void (*)(void*))on_algorithm_selection, arg_movie_from_movie_clicked);

    // Initialisation

    Arg_algo_selection* arg_init = (Arg_algo_selection*)calloc(1, sizeof(Arg_algo_selection));
    arg_init->args = args;
    arg_init->ui = ui;
    arg_init->bt_partial_clustering = _bt_partial_clustering_algo;
    arg_init->bt_random = _bt_random_algo;
    arg_init->bt_random_good = _bt_random_good_algo;
    arg_init->bt_random_indexing_v100 = _bt_random_indexing_v100_algo;
    arg_init->bt_random_indexing_v10 = _bt_random_indexing_v10_algo;
    arg_init->bt_movie_from_movie = _bt_movie_from_movie_algo;
    arg_init->descr_partial_clustering = _container_partial_clustering_algorithm_description;
    arg_init->descr_random = _container_random_algorithm_description;
    arg_init->descr_random_good = _container_good_random_algorithm_description;
    arg_init->descr_random_indexing_v100 = _container_random_indexing_v100_algorithm_description;
    arg_init->descr_random_indexing_v10 = _container_random_indexing_v10_algorithm_description;
    arg_init->descr_movie_from_movie = _container_movie_from_movie_algorithm_description;
    arg_init->new_num = args->algorithm_of_prediction;
    if(arg_init->new_num == -1){
        arg_init->new_num = 0;
    }

    on_algorithm_selection(arg_init);

}