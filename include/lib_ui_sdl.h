#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define WIN_WIDTH 1800
#define WIN_HEIGHT 900
#define WIN_TITLE "NextWatch"
#define WIN_FLAGS 0
#define WIN_POS_X SDL_WINDOWPOS_CENTERED
#define WIN_POS_Y SDL_WINDOWPOS_CENTERED

#define COLOR_BLACK (SDL_Color){0, 0, 0, 255}
#define COLOR_VERY_DARK_GREY (SDL_Color){8, 8, 8, 255}
#define COLOR_DARK_GREY (SDL_Color){25, 25, 25, 255}
#define COLOR_GREY (SDL_Color){100, 100, 100, 255}
#define COLOR_LIGHT_GREY (SDL_Color){160, 160, 160, 255}
#define COLOR_WHITE (SDL_Color){255, 255, 255, 255}

#define COLOR_RED (SDL_Color){255, 0, 0, 255}
#define COLOR_SEMI_DARK_RED (SDL_Color){100, 0, 0, 255}
#define COLOR_DARK_RED (SDL_Color){25, 0, 0, 255}
#define COLOR_VERY_DARK_RED (SDL_Color){8, 0, 0, 255}

#define COLOR_GREEN (SDL_Color){0, 255, 0, 255}
#define COLOR_SEMI_DARK_GREEN (SDL_Color){0, 100, 0, 255}
#define COLOR_DARK_GREEN (SDL_Color){0, 25, 0, 255}
#define COLOR_VERY_DARK_GREEN (SDL_Color){0, 8, 0, 255}

#define COLOR_BLUE (SDL_Color){0, 0, 255, 255}
#define COLOR_SEMI_DARK_BLUE (SDL_Color){0, 0, 100, 255}
#define COLOR_DARK_BLUE (SDL_Color){0, 0, 25, 255}
#define COLOR_VERY_DARK_BLUE (SDL_Color){0, 0, 8, 255}

#define COLOR_CYAN (SDL_Color){0, 255, 255, 255}
#define COLOR_SEMI_DARK_CYAN (SDL_Color){0, 100, 100, 255}
#define COLOR_DARK_CYAN (SDL_Color){0, 25, 25, 255}
#define COLOR_VERY_DARK_CYAN (SDL_Color){0, 8, 8, 255}

#define COLOR_YELLOW (SDL_Color){255, 255, 0, 255}
#define COLOR_SEMI_DARK_YELLOW (SDL_Color){100, 100, 0, 255}
#define COLOR_DARK_YELLOW (SDL_Color){25, 25, 0, 255}
#define COLOR_VERY_DARK_YELLOW (SDL_Color){8, 8, 0, 255}

#define COLOR_MAGENTA (SDL_Color){255, 0, 255, 255}
#define COLOR_SEMI_DARK_MAGENTA (SDL_Color){100, 0, 100, 255}
#define COLOR_DARK_MAGENTA (SDL_Color){25, 0, 25, 255}
#define COLOR_VERY_DARK_MAGENTA (SDL_Color){8, 0, 8, 255}

#define COLOR_FOCUSED_ELT (SDL_Color){95, 89, 217, 255}

#define KEY_ESCAPE 41

struct Ui_Element;

enum UI_Element_Interaction_Types {
    UI_ELT_INT_CHANGE_COLOR,                        // C
    UI_ELT_INT_CHANGE_FONTSIZE,                     // F
    UI_ELT_INT_CHANGE_SCALE                         // S
};

enum Ui_Element_Types {
    // BASE SDL ELEMENTS
    UI_ELT_POINT,                                   // (0) P
    UI_ELT_LINE,                                    // (1) L
    UI_ELT_RECT,                                    // (2) R
    UI_ELT_TEXT,                                    // (3) Txt
    UI_ELT_TEXTURE,                                 // (4) Tex
    
    // MY CUSTOM ELEMENTS
    UI_ELT_BT_RECT,                                 // (5) Btr
    UI_ELT_BT_TEXTURE,                              // (6) Btt
    UI_ELT_CONTAINER,                               // (7) Cont
    UI_ELT_SCROLLBAR,                               // (8) Sb
    UI_ELT_TXT_INPUT,                               // (9) Inp
};

enum Ui_Overflow_Mode {
    UI_OVERFLOW_SHOW,                               // Do not hide the part of the elements that exceed the size of the container
    UI_OVERFLOW_HIDE,                               // Do not display the part of the elements that exceed the size of the container
    UI_OVERFLOW_SCROLL_HV,                          // Add a scrollbar that scrolls on both vertical axis and horizontal axis
    UI_OVERFLOW_SCROLL_H,                           // Add a scrollbar that scrolls only on the horizontal axis, and hide the part of the elements that exceed the size of the containers
    UI_OVERFLOW_SCROLL_V,                           // Add a scrollbar that scrolls only on the vertical axis, and hide the part of the elements that exceed the size of the containers
    UI_HBOX_CONTAINER,                              // a SCROLL_H container with an automatic placement of the elements
    UI_VBOX_CONTAINER,                              // a SCROLL_V container with an automatic placement of the elements
    UI_VGRID_CONTAINER,                             // a VBOX container that wraps horizontally
    UI_HGRID_CONTAINER,                             // an HBOX conainer that wraps vertically
};

enum Ui_Scrollbar_Axis {
    UI_SB_HORIZONTAL,                               // Scrollbar axis Horizontal
    UI_SB_VERTICAL,                                 // Scrollbar axis Vertical
};

enum Ui_Event_Types {
    UI_EVENT_NONE,                                  // No events
    UI_EVENT_CLICK,                                 // When user click with the mouse
    UI_EVENT_KEY,                                   // When user press a key
};

typedef struct{
    //
    int event_type;                                 //
    int key_code;                                   //
    int mouse_button;                               //
    struct Ui_Element* elt_clicked;                 //
    //
    void (*function)(void* func_args);              //
    void* func_args;                                //
    //
    bool stop_following_events;                     //
    //
} Ui_Event;

typedef struct{
    unsigned short int type;                        // Type
    // Variable shared for all interaction types
    SDL_Color color;                                // Change color when on hover
    int font_size;                                  // Change font size when on hover
    float scale;                                    // Change scale when on hover
} Ui_Element_Interaction;

typedef struct Ui_Element{
    // VERY GROUND BASED ATTRIBUTES
    unsigned short int type;                        // element type
    char id[100];                                   // unique id
    bool visible;                                   // if the element is currently displayable
    bool is_a_duplicate;                            // to know if the element is from a duplication

    // element definition (variable shared for all of element types)
    int x;                                          // *
    int y;                                          // P-L-R-Txt-Tex-Btt-Btr
    int x2;                                         // L
    int y2;                                         // L
    int absolute_calculated_X;                      // *
    int absolute_calculated_Y;                      // *
    int w;                                          // *
    int h;                                          // *
    bool fill;                                      // R-Btr
    SDL_Color color;                                // P-L-R-Txt-Btr-Btt
    SDL_Color color2;                               // Btr
    bool bt_toggle_mode;                            // Btr - Btt
    bool bt_pressed;                                // Btr - Btt
    char txt[1000];                                 // Txt-Btr-Btt
    int baseFontsize;                               // Txt-Btr-Btt
    size_t font_id;                                 // Txt-Btr-Btt
    SDL_Rect rect;                                  // R-Txt-Tex-Btt-Btr
    SDL_Surface* surface;                           // R-Txt-Tex-Btt
    SDL_Surface* surface2;                          // Btt
    SDL_Texture* texture;                           // Txt-Tex-Btt
    SDL_Texture* texture2;                          // Btt
    SDL_Texture* texture3;                          // Btt(Toggle Mode)
    char* input_txt;                                // Inp
    SDL_Texture** input_txt_textures;               // Inp
    size_t input_length;                            // Inp
    size_t max_input_length;                        // Inp
    size_t input_cursor_position;                   // Inp
    double sb_start;                                // Sb
    double sb_end;                                  // Sb
    double sb_step;                                 // Sb
    double sb_value;                                // Sb
    SDL_Color sb_bg_color;                          // Sb-Scroll Containers
    SDL_Color sb_curs_color;                        // Sb-Scroll Containers
    int sb_width;                                   // Scroll Containers
    int sb_axis;                                    // Sb
    struct Ui_Element* sb_parent_container;         // Sb

    // element attributes

    // element interactive action
    //  - hover
    size_t nb_interactions_on_hover;                //
    Ui_Element_Interaction** interactions_on_hover; //

    //  - click
    bool clickable;
    Ui_Event* on_click_events;                      //
    size_t length_on_click_events;                  //
    size_t capacity_on_click_events;                //
    //
    Ui_Event* on_keypress_events;                   //
    size_t length_on_keypress_events;               //
    size_t capacity_on_keypress_events;             //

    // Ui Elements - children
    struct Ui_Element* parent;                      //
    size_t children_capacity;                       //
    size_t nb_children;                             //
    struct Ui_Element** children;                   //
    size_t nb_visible_children;                     //
    struct Ui_Element** visible_children;           //
    int container_view_dec_X;                       //
    int container_view_dec_Y;                       //
    struct Ui_Element* cont_sb_vertical;            //
    struct Ui_Element* cont_sb_horizontal;          //
    int cont_total_width;                           //
    int cont_total_height;                          //

    // Container properties
    int overflow_mode;                              //
    int padding_top;                                //
    int padding_right;                              //
    int padding_bottom;                             //
    int padding_left;                               //
    int separation_x;                               //
    int separation_y;                               //
    //
    bool is_freeing;                                //
    //
} Ui_Element;

typedef struct{
    int delay_at_mainloop_end;                      // In miliseconds
    // Background Color
    SDL_Color bg_color;                             //
    // Ui Elements
    size_t elements_capacity;                       //
    size_t nb_elements;                             //
    Ui_Element** elements;                          //
    //
    Ui_Element* focused_element;                    //
    //
    bool initial_visible_elements_detection;        //
    // Events
    // On Click
    size_t length_on_click_events;                  //
    size_t capacity_on_click_events;                //
    Ui_Event* on_click_events;                      //
    // On keypress
    size_t length_on_keypress_events;               //
    size_t capacity_on_keypress_events;             //
    Ui_Event* on_keypress_events;                   //
    bool is_ctrl_pressed;                           //
    bool is_shift_pressed;                          //
    bool is_alt_pressed;                            //
    bool is_alt_gr_pressed;                         //
    Ui_Element* grabbed_elt;                        //
} Ui_Window;

typedef struct {
    // Base SDL variables
    SDL_Window* window;                             //
    SDL_Renderer* renderer;                         //
    //
    bool window_is_open;                            //
    // Fonts
    size_t nb_fonts;                                //
    size_t fonts_capacity;                          //
    TTF_Font** fonts;                               //
    // Mouse pos
    SDL_Point mouse_position;                       //
    // Windows
    size_t nb_windows;                              //
    int current_window;                             //
    Ui_Window** windows;                            //
} Ui;

/* -- DEBUG FUNCTIONS -- */

void print_elt_type(Ui_Element* elt);

/* -- UTILS FUNCTIONS -- */

// Set the cl_dst rgba properties
void set_color(SDL_Color* cl_dst, int src_red, int src_green, int src_blue, int src_alpha);

// Set the rect xywh properties
void set_rect(SDL_Rect* rect, int x, int y, int w, int h);

SDL_Color ui_cl(int r, int g, int b, int a);

// Create texture from text
SDL_Texture* ui_texture_from_text(Ui* ui, const char txt[], SDL_Color color, size_t ui_font_id);

////////////////////////
/* -- UI FUNCTIONS -- */
////////////////////////

// Init the UI
Ui* ui_init(size_t nb_windows);

// Init a window
void ui_init_window(Ui* ui, size_t id_window);

// Add a font to the UI
void add_font(Ui* ui, const char* font_filepath, int font_size);

// Change the background color of the UI
void ui_set_bg_color(Ui* ui, int r, int g, int b);

// Function to display a element
void ui_display_element(Ui* ui, Ui_Element* elt, int parent_X, int parent_Y);

// Function to display all the children of an element 
void ui_display_container(Ui* ui, Ui_Element* elt, int posX, int posY, int width, int height);

// Display the UI
void ui_display(Ui* ui);

// Get id of Clicked Element
// Put the id of the first element that is clicked in `id_elt`
void ui_get_id_of_clicked_element(Ui* ui, char id_elt[100]);

// Free an UI element
void ui_free_element(Ui_Element** elt);

// Free the UI
void ui_free(Ui* ui);

void ui_pre_event(Ui* ui);

void ui_clean_windows(Ui* ui);

//////////////////////////////
/* -- ELEMENTS FUNCTIONS -- */
//////////////////////////////

// Create an empty element for UI
Ui_Element* ui_init_empty_element(const char id[]);

// Get element idx with id
size_t ui_get_element_id(Ui* ui, const char id[]);

// Get element node with id
// (please try not to do something stupid if you change Ui_Element properties out of `lib_ui` functions)
Ui_Element* ui_get_element(Ui* ui, const char id[]);

// Get element node with id
// (please try not to do something stupid if you change Ui_Element properties out of `lib_ui` functions)
Ui_Element* ui_get_element_rec(Ui* ui, char id_to_search[]);

// Delete an ui with id
void ui_delete_element(Ui* ui, const char id[], bool move);

// Add a point to the ui
Ui_Element* ui_create_point(Ui* ui, const char id[], int x, int y, SDL_Color cl);

// Add a line to the ui
Ui_Element* ui_create_line(Ui* ui, const char id[], int x1, int y1, int x2, int y2, SDL_Color cl);

// Add a rect to the ui
Ui_Element* ui_create_rect(Ui* ui, const char id[], int x, int y, int w, int h, bool fill_rect, SDL_Color cl);

// Updates text elt
void ui_update_elt_text(Ui* ui, Ui_Element* elt, char new_txt[]);

// Add text to the ui
Ui_Element* ui_create_text(Ui* ui, const char id[], int x, int y, char txt[], SDL_Color cl, int fontsize, size_t ui_font_id);

// Add a texture to the ui
Ui_Element* ui_create_texture(Ui* ui, const char id[], int x, int y, int w, int h, char texture_filepath[]);

// Add a button with a rectangle background to the ui
Ui_Element* ui_create_bt_rect(Ui* ui, const char id[], int x, int y, int w, int h, bool fill_rect, SDL_Color bg_cl, char txt[], SDL_Color txt_cl, int fontsize, size_t ui_font_id);

// Add a button with a texture background to the ui
Ui_Element* ui_create_bt_texture(Ui* ui, const char id[], int x, int y, int w, int h, char texture_filepath[], char txt[], SDL_Color txt_cl, int fontsize, size_t ui_font_id);

// Add a button with a texture background to the ui
Ui_Element* ui_create_toggle_bt_texture(Ui* ui, const char id[], int x, int y, int w, int h, char texture_bg_off_filepath[], char texture_bg_on_filepath[], char txt[], SDL_Color txt_cl, int fontsize, size_t ui_font_id);

// Create a container element
Ui_Element* ui_create_container(Ui* ui, const char id[], int x, int y, int w, int h, int overflow_mode, SDL_Color sb_bg_color, SDL_Color sb_curs_color, int sb_width);

// Create a scrollbar
Ui_Element* ui_create_scrollbar(Ui* ui, const char id[], int x, int y, int w, int h, int val_start, int val_end, int val_step, int sb_axis, SDL_Color bg_color, SDL_Color cursor_color);

// Create a text input element
Ui_Element* ui_create_txt_input(Ui* ui, const char id[], int x, int y, int w, int h, int max_length, SDL_Color bg_color, SDL_Color txt_color, size_t ui_font_id, int font_size, char placeholder[]);

// Add an element to the UI
void ui_append_elt_to_ui(Ui* ui, Ui_Element* elt);

// Add an element to a parent element
void ui_append_elt_to_elt(Ui_Element* parent, Ui_Element* elt);

// Detect visible children of container element
void detect_visible_children(Ui_Element* parent_elt, int abs_parent_posX, int abs_parent_posY, bool recursive);

//
void update_container_elements_position(Ui_Element* container, long long int starts_from);

/////////////////////
/* -- UI EVENTS -- */
/////////////////////

//
void ui_add_event(
    Ui_Event* arr_events, size_t* arr_length, size_t* arr_capacity,
    int event_type, int event_key, int event_mbutton, bool stop_following_events, Ui_Element* elt_clicked,
    void (*function)(void* function_args), void* function_args
);

//
void ui_add_on_elt_click_event(
    Ui_Element* elt, int event_mbutton, bool stop_following_events,
    void (*function)(void* function_args), void* function_args
);

//
void ui_add_on_elt_keypress_event(
    Ui_Element* elt,
    int event_key, bool stop_following_events,
    void (*function)(void* function_args), void* function_args
);

//
void ui_add_global_keypress_event(
    Ui* ui,
    int event_key, bool stop_following_events,
    void (*function)(void* function_args), void* function_args
);

/////////////////////////////////////////////
/* -- UI EVENTS MAYBE USEFUL FUNCTIONS  -- */
/////////////////////////////////////////////

typedef struct {
    Ui_Element* elt1;
    Ui_Element* elt2;
} Arg_elts_duo;

typedef struct {
    Ui_Element* elt1;
    Ui_Element* elt2;
    Ui_Element* elt3;
} Arg_elts_trio;

typedef struct {
    Ui* ui;
    int number;
} Arg_ui_and_int;

typedef struct {
    Ui* ui;
    Ui_Element* elt;
} Arg_ui_and_elt;

typedef struct {
    Ui* ui;
    Ui_Element* elt1;
    Ui_Element* elt2;
} Arg_ui_and_elts_duo;

typedef struct {
    Ui* ui;
    Ui_Element* elt1;
    Ui_Element* elt2;
    Ui_Element* elt3;
} Arg_ui_and_elts_trio;


void change_current_windows(Arg_ui_and_int* arg);

void quit(Ui* ui);

///////////////////////
/* -- UI MAINLOOP -- */
///////////////////////

// ui mainloop
void ui_mainloop(Ui* ui);

//
void add_letter_to_txt_input(Ui* ui, Ui_Element* txt_input, char letter);

//
bool detect_keyboard_txt_input(Ui* ui, Ui_Element* txt_input, int keycode);

