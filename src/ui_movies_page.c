#include "ui_movies_page.h"
#include "main_algos.h"

void int_to_id_movie(int id_movie, char str[]){
    sprintf(str, "%d", id_movie);
}

int id_movie_to_int(char str[]){
    return atoi(str);
}

typedef struct{
    args_t* args;
    int id_movie;
    Ui_Element* movie_line;
} Arg_On_Movie_Click;

void on_movie_click(Arg_On_Movie_Click* arg_fct){
    //
    args_t* args = arg_fct->args;
    int id_movie = arg_fct->id_movie;
    Ui_Element* movie_line = arg_fct->movie_line;
    //
    if(args->movies_selected[id_movie]){
        args->movies_selected[id_movie] = false;
        movie_line->children[0]->visible = false;
        movie_line->children[1]->bt_pressed = false;
    }
    else{
        args->movies_selected[id_movie] = true;
        movie_line->children[0]->visible = true;
        movie_line->children[1]->bt_pressed = true;
    }
    //
}

Ui_Element* create_movie_line(Ui* ui, args_t* args, MainData* main_data, int id_movie){
    if(id_movie <= 0 || id_movie > 17770){
        fprintf(stderr, "\e[31mError : %d is a bad id_movie\e[m\n", id_movie);
        ui_free(ui);
        exit(42);
    }
    //
    char idm[100] = "";
    sprintf(idm, "%d", id_movie);
    // movie_line
    Ui_Element* movie_line = ui_create_container(ui, idm, 0, 0, 600, 30, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 0);

    char idmbg[100] = "bg_movie_";
    sprintf(idmbg, "bg_movie_%d", id_movie);
    Ui_Element* bg_movie_line = ui_create_rect(ui, idmbg, 0, 0, 600, 30, true, COLOR_DARK_GREEN);
    bg_movie_line->visible = args->movies_selected[id_movie];
    ui_append_elt_to_elt(movie_line, bg_movie_line);

    char idmcb[100] = "checkbox_movie_";
    sprintf(idmcb, "checkbox_movie_%d", id_movie);
    Ui_Element* checkbox_movie_line = ui_create_toggle_bt_texture(ui, idmcb, 5, 5, 25, 25, "res/ui/textures/unchecked_box.png", "res/ui/textures/checked_box.png", "", COLOR_WHITE, 0, 0);
    checkbox_movie_line->bt_pressed = args->movies_selected[id_movie];
    ui_append_elt_to_elt(movie_line, checkbox_movie_line);

    char idmtxt[100] = "title_movie_";
    sprintf(idmtxt, "title_movie_%d", id_movie);
    Ui_Element* title_movie_line = ui_create_text(ui, idmtxt, 35, 5, main_data->movies[id_movie-1].name, COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(movie_line, title_movie_line);

    //

    Arg_On_Movie_Click* arg_on_movie_click = (Arg_On_Movie_Click*)calloc(1, sizeof(Arg_On_Movie_Click));
    arg_on_movie_click->args = args;
    arg_on_movie_click->id_movie = id_movie;
    arg_on_movie_click->movie_line = movie_line;
    ui_add_on_elt_click_event(checkbox_movie_line, 1, true, (void (*)(void*))on_movie_click, arg_on_movie_click);

    //

    return movie_line;
}

bool substring_is_in_string(const char main_string[], const char substring[]){
    int msl = strlen(main_string);
    int ssl = strlen(substring);
    for(int i=0; i<=msl - ssl; i++){
        bool bon = true;
        for(int j=0; j<ssl; j++){
            char msc = main_string[i+j];
            char ssc = substring[j];
            if(msc != ssc && !(ssc >= 'a' && ssc <= 'z' && ssc-'a'+'A' == msc)){
                bon = false;
                break;
            }
        }
        if(bon){
            return true;
        }
    }
    return false;
}

typedef struct {
    Ui* ui;
    args_t* args;
    Ui_Element* input_search_bar;
    Ui_Element* movie_list_container;
} Arg_search_movies;

void search_movies(Arg_search_movies* arg){
    // arg->elt1 will be the input search bar
    // arg->elt2 will be the movie_list_container
    Ui_Element* input_search_bar = arg->input_search_bar;
    Ui_Element* movie_list_container = arg->movie_list_container;
    //
    for(size_t i=0; i<movie_list_container->nb_children; i++){
        if(movie_list_container->children[i] == NULL || movie_list_container->children[i]->type != UI_ELT_CONTAINER) continue;
        Ui_Element* movie_line = movie_list_container->children[i];
        int id_movie = atoi(movie_line->id);
        if(id_movie < 0 || id_movie > NUMBER_OF_MOVIES){
            fprintf(stderr, "\e[31mError : bad movie id : %s -> %d\e[m\n", movie_line->id, id_movie);
            continue;
        }
        if(movie_line->type != UI_ELT_CONTAINER){
            fprintf(stderr, "\e[31mError : movie_line is not CONTAINER!\e[m\n");
            print_elt_type(movie_line);
            return;
        }
        if(movie_line->nb_children != 3){
            fprintf(stderr, "\e[31mError : movie_line have not 3 children!\e[m\n");
            return;
        }
        if(arg->args->cb_filter_only_selected != NULL){
            if(arg->args->cb_filter_only_selected->bt_pressed){
                if(!(arg->args->movies_selected[id_movie])){
                    movie_line->visible = false;
                    continue;
                }
            }
        }
        //
        if(strlen(input_search_bar->input_txt)==0){
            movie_line->visible = true;
        }
        else{
            char* movie_title = movie_line->children[2]->txt;
            if(substring_is_in_string(movie_title, input_search_bar->input_txt)){
                movie_line->visible = true;
            }
            else{
                movie_line->visible = false;
            }
        }
    }
    movie_list_container->children[0]->sb_value = 0;
    movie_list_container->container_view_dec_X = 0;
    movie_list_container->container_view_dec_Y = 0;
    update_container_elements_position(movie_list_container, 0);
    detect_visible_children(movie_list_container, movie_list_container->absolute_calculated_X, movie_list_container->absolute_calculated_Y, false);
}

void toggle_filter_container_visibility(Ui_Element* filter_container){
    filter_container->visible = !(filter_container->visible);
    detect_visible_children(filter_container->parent, filter_container->parent->absolute_calculated_X, filter_container->parent->absolute_calculated_Y, false);
}


typedef struct{
    Ui* ui;
    MainData* main_data;
    args_t* args;
    Ui_Element* results_container;
} Arg_On_Run_Pressed;

void on_run_pressed(Arg_On_Run_Pressed* arg_fct){
    //
    Ui* ui = arg_fct->ui;
    MainData* main_data = arg_fct->main_data;
    args_t* args = arg_fct->args;
    Ui_Element* results_container = arg_fct->results_container; // VBOX container

    // Préparer les arguments
    args->nb_inputs = 0;
    for(int i=1; i<NUMBER_OF_MOVIES_PLUS_UN; i++){
        if(args->movies_selected[i]){
            args->input_id_movies[args->nb_inputs] = i;
            args->nb_inputs++;
        }
    }

    // Lancer L'algorithme
    Predictions* predictions = main_algos(args, main_data);

    // Nettoyer l'affichage de l'ancien résultat
    for(size_t i=0; i<results_container->nb_children; i++){
        //
        if(results_container->children[i]==NULL || results_container->children[i]->sb_parent_container != NULL){
            continue;
        }
        //
        ui_free_element(&(results_container->children[i]));
        results_container->children[i] = NULL;
    }

    // Afficher les éléments
    for(int i=0; i<predictions->nb_predictions_faites; i++){
        char id_txt[100] = "";
        sprintf(id_txt, "txt_%d", i);
        Ui_Element* txt_result = ui_create_text(ui, id_txt, 0, 0, main_data->movies[predictions->predictions[i]-1].name, COLOR_WHITE, 18, 0);
        ui_append_elt_to_elt(results_container, txt_result);
    }
    detect_visible_children(results_container, results_container->absolute_calculated_X, results_container->absolute_calculated_Y, false);

    // Memory Free
    free(predictions->predictions);
    free(predictions);
}


void ui_init_movies_page(Ui* ui, args_t* args, MainData* main_data){
    
    // Colors
    const SDL_Color cl_bg = ui_cl(0, 0, 0, 255);
    const SDL_Color cl1 = ui_cl(15, 15, 15, 255);
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
    Ui_Element* _bt_header_movies = ui_create_bt_rect(ui, "bt_header_movies", 1500, 10, 125, 50, false, COLOR_DARK_GREY, "movies", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_header, _bt_header_movies);

    // Button Algos
    Ui_Element* _bt_header_algos = ui_create_bt_rect(ui, "bt_header_algos", 1650, 10, 125, 50, false, COLOR_VERY_DARK_GREY, "algos", COLOR_WHITE, 18, 0);

    Arg_ui_and_int* arg_chg_win_to_algos_1 = (Arg_ui_and_int*)calloc(1, sizeof(Arg_ui_and_int));
    arg_chg_win_to_algos_1->ui = ui;
    arg_chg_win_to_algos_1->number = 2;
    ui_add_on_elt_click_event(_bt_header_algos, 1, true, (void (*)(void*))change_current_windows, arg_chg_win_to_algos_1);
    
    ui_append_elt_to_elt(_container_header, _bt_header_algos);

    // ######################################
    // #########  Movies Selection  #########
    // ######################################

    // Movie Selection Text
    Ui_Element* _text_movie_selection = ui_create_text(ui, "text_movie_selection", 250, 100, "Movies Selection", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, _text_movie_selection);

    // Container Movies Selection
    Ui_Element* _container_movies_selection = ui_create_container(ui, "container_movies_selection", 10, 140, 700, 700, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_movies_selection);

    // Container Movies Selection Background
    Ui_Element* _bg_movies_selection = ui_create_rect(ui, "bg_movie_selection", 0, 0, 700, 700, true, COLOR_VERY_DARK_GREY);
    ui_append_elt_to_elt(_container_movies_selection, _bg_movies_selection);

    // Input search container
    Ui_Element* _container_input_search = ui_create_container(ui, "container_input_search", 10, 10, 680, 60, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_elt(_container_movies_selection, _container_input_search);

    // Text input
    Ui_Element* _search_input = ui_create_txt_input(ui, "search_input", 10, 0, 525, 30, 50, cl1, COLOR_WHITE, 0, 18, "search...");
    ui_append_elt_to_elt(_container_input_search, _search_input);

    // Search button
    Ui_Element* _search_button = ui_create_bt_rect(ui, "search_button", 550, 0, 120, 25, true, cl1, "search", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_input_search, _search_button);

    // Filter button
    Ui_Element* _filter_button_toggle = ui_create_bt_rect(ui, "filter_button", 550, 30, 120, 20, true, cl2, "filter", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_input_search, _filter_button_toggle);

    // ###### Movie List Container ######
    
    // Container
    Ui_Element* _movie_list_container = ui_create_container(ui, "movie_list_container", 10, 70, 680, 500, UI_VBOX_CONTAINER, COLOR_DARK_GREY, COLOR_WHITE, 15);
    _movie_list_container->separation_y = 2;
    Arg_search_movies* arg_search_movies = calloc(1, sizeof(Arg_search_movies));
    arg_search_movies->ui = ui;
    arg_search_movies->args = args;
    arg_search_movies->input_search_bar = _search_input;
    arg_search_movies->movie_list_container = _movie_list_container;
    ui_add_on_elt_click_event(_search_button, 1, true, (void (*)(void*))search_movies, arg_search_movies);
    //
    ui_append_elt_to_elt(_container_movies_selection, _movie_list_container);

    //
    for(int id_movie=1; id_movie<=NUMBER_OF_MOVIES; id_movie++){
        Ui_Element* movie_line = create_movie_line(ui, args, main_data, id_movie);
        ui_append_elt_to_elt(_movie_list_container, movie_line);
    }

    // ###### Filter Container ######

    Ui_Element* _filter_container = ui_create_container(ui, "filter_container", 10, 65, 680, 200, UI_OVERFLOW_SHOW, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_elt(_container_movies_selection, _filter_container);
    _filter_container->visible = false;
    ui_add_on_elt_click_event(_filter_button_toggle, 1, true, (void (*)(void*))toggle_filter_container_visibility, _filter_container);

    // Filter Container Bg
    Ui_Element* _bg_filter_container = ui_create_rect(ui, "bg_filter_container", 0, 0, 680, 200, true, cl2);
    ui_append_elt_to_elt(_filter_container, _bg_filter_container);

    // Checkbox button Filter Elt Selected
    Ui_Element* _checkbox_filter_elts_selected = ui_create_toggle_bt_texture(ui, "checkbox_filter_elts_selected", 10, 10, 25, 25, "res/ui/textures/unchecked_box.png", "res/ui/textures/checked_box.png", "", COLOR_WHITE, 0, 0);
    ui_append_elt_to_elt(_filter_container, _checkbox_filter_elts_selected);
    args->cb_filter_only_selected = _checkbox_filter_elts_selected;

    // Texte Filter Elt Selectionnes
    Ui_Element* _text_filter_elts_selected = ui_create_text(ui, "text_filter_elts_selected", 50, 12, "View only selected movies", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_filter_container, _text_filter_elts_selected);
    
    
    // ####################################
    // #########  Middle Buttons  #########
    // ####################################

    // Container Middle Buttons
    Ui_Element* _container_middle_buttons = ui_create_container(ui, "container_middle_buttons", 725, 140, 350, 700, UI_OVERFLOW_HIDE, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_middle_buttons);

    // Container Middle Buttons Background
    Ui_Element* _bg_middle_buttons = ui_create_rect(ui, "bg_middle_buttons", 0, 0, 350, 700, true, COLOR_BLACK);
    ui_append_elt_to_elt(_container_middle_buttons, _bg_middle_buttons);

    // Bt Run
    Ui_Element* _button_run = ui_create_bt_rect(ui, "button_run", 90, 50, 150, 40, true, cl2, "run ->", COLOR_WHITE, 24, 1);
    ui_append_elt_to_elt(_container_middle_buttons, _button_run);

    // Change Algorithm Button
    Ui_Element* _button_change_algorithm = ui_create_bt_rect(ui, "button_change_algorithm", 90, 470, 150, 40, true, cl2, "change", COLOR_WHITE, 18, 0);
    Arg_ui_and_int* arg_chg_win_to_algos_2 = (Arg_ui_and_int*)calloc(1, sizeof(Arg_ui_and_int));
    arg_chg_win_to_algos_2->ui = ui;
    arg_chg_win_to_algos_2->number = 2;
    ui_add_on_elt_click_event(_button_change_algorithm, 1, true, (void (*)(void*))change_current_windows, arg_chg_win_to_algos_2);
    ui_append_elt_to_elt(_container_middle_buttons, _button_change_algorithm);

    // Text Algorithm Title
    Ui_Element* _text_algorithm_title = ui_create_text(ui, "text_algorithm_title", 90, 400, "Algorithm selected : ", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_middle_buttons, _text_algorithm_title);

    // Algorithm Name
    Ui_Element* _text_algorithm_name = ui_create_text(ui, "text_algorithm_name", 5, 430, "Algorithm name", COLOR_WHITE, 18, 0);
    ui_append_elt_to_elt(_container_middle_buttons, _text_algorithm_name);
    args->algo_name_txt = _text_algorithm_name;

    // #######################################
    // #########  Algorithm Results  #########
    // #######################################

    // Movie Selection Text
    Ui_Element* _text_algorithm_results = ui_create_text(ui, "text_algorithm_results", 1340, 100, "Algorithm Results", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, _text_algorithm_results);

    // Container Algorithm Results
    Ui_Element* _container_algorithm_results = ui_create_container(ui, "container_algorithm_results", 1090, 140, 700, 700, UI_OVERFLOW_HIDE, COLOR_BLACK, COLOR_WHITE, 15);
    ui_append_elt_to_ui(ui, _container_algorithm_results);

    // Container Algorithm Results Background
    Ui_Element* _bg_algorithm_results = ui_create_rect(ui, "bg_algorithm_results", 0, 0, 700, 700, true, COLOR_VERY_DARK_GREY);
    ui_append_elt_to_elt(_container_algorithm_results, _bg_algorithm_results);

    // Algorithm Results List
    Ui_Element* _algorithm_results_list = ui_create_container(ui, "algorithm_results_list", 10, 10, 680, 680, UI_VBOX_CONTAINER, COLOR_BLACK, COLOR_WHITE, 10);
    ui_append_elt_to_elt(_container_algorithm_results, _algorithm_results_list);

    Arg_On_Run_Pressed* arg_on_run_pressed = (Arg_On_Run_Pressed*)calloc(1, sizeof(Arg_On_Run_Pressed));
    arg_on_run_pressed->args = args;
    arg_on_run_pressed->main_data = main_data;
    arg_on_run_pressed->results_container = _algorithm_results_list;
    arg_on_run_pressed->ui = ui;
    ui_add_on_elt_click_event(_button_run, 1, true, (void (*)(void*))on_run_pressed, arg_on_run_pressed);

}