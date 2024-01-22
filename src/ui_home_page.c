#include "ui_home_page.h"


void ui_init_home_page(Ui* ui){

    // ###############################
    // ######### HOME PAGE   #########
    // ###############################
    
    // --- Background ---
    Ui_Element* bg_texture = ui_create_texture(ui, "bg_texture", 0, 0, 1800, 900, "./res/ui/bg/bg_app_v2.png");
    ui_append_elt_to_ui(ui, bg_texture);

    // --- Text Title ---
    Ui_Element* text_title = ui_create_text(ui, "text_title", 50, 500, "nextwatch", COLOR_WHITE, 32, 2);
    ui_append_elt_to_ui(ui, text_title);

    // --- Text Description ---
    Ui_Element* text_description_1 = ui_create_text(ui, "text_description_1", 50, 550, "a movie prediction app", COLOR_WHITE, 18, 0);
    ui_append_elt_to_ui(ui, text_description_1);

    Ui_Element* text_description_2 = ui_create_text(ui, "text_description_2", 50, 570, "made by CERISARA Nathan and LAFONT Loic", COLOR_WHITE, 18, 0);
    ui_append_elt_to_ui(ui, text_description_2);

    // --- button launch ---
    Ui_Element* bt_launch = ui_create_bt_rect(ui, "bt_launch", 1200, 400, 250, 50, true, COLOR_BLACK, "launch the app", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, bt_launch);

    Arg_ui_and_int* arg = (Arg_ui_and_int*)calloc(1, sizeof(Arg_ui_and_int));
    arg->ui = ui;
    arg->number = 1;
    ui_add_on_elt_click_event(bt_launch, 1, false, (void (*)(void*))change_current_windows, arg);

    // --- button quit ---
    Ui_Element* bt_quit = ui_create_bt_rect(ui, "bt_quit", 1100, 550, 100, 50, true, COLOR_BLACK, "quit", COLOR_WHITE, 24, 1);
    ui_append_elt_to_ui(ui, bt_quit);

    ui_add_on_elt_click_event(bt_quit, 1, true, (void (*)(void*))quit, ui);

}




