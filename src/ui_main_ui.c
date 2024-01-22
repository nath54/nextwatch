#include "ui_main_ui.h"
#include "ui_home_page.h"
#include "ui_movies_page.h"
#include "ui_algos_page.h"

// Main ui
void main_ui(args_t* args, MainData* main_data){

    // Init the ui, with 3 windows
    Ui* ui = ui_init(3);

    // Adding fonts
    add_font(ui, "./res/ui/fonts/open_sans.ttf", 18); // font-id=0
    add_font(ui, "./res/ui/fonts/open_sans.ttf", 24); // font-id=1
    add_font(ui, "./res/ui/fonts/open_sans.ttf", 32); // font-id=2
    
    // Init the first window (Home page)
    ui_init_window(ui, 0);
    ui->current_window = 0;
    ui_init_home_page(ui);

    // Init the second window (Movie page)
    ui_init_window(ui, 1);
    ui->current_window = 1;
    ui_init_movies_page(ui, args, main_data);

    // Init the third window (Algos page)
    ui_init_window(ui, 2);
    ui->current_window = 2;
    ui_init_algos_page(ui, args);

    //
    ui->current_window = 0;

    // ###############################
    // ######### UI Mainloop #########
    // ###############################
    ui_mainloop(ui);

    // Close the windows & UI
    ui_free(ui);
}