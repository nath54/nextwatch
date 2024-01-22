#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "lib_ui_sdl.h"

///////////////////////////
/* -- DEBUG FUNCTIONS -- */
///////////////////////////

void print_elt_type(Ui_Element* elt){
    printf("\e[32m Elt \"%s\" is of type (%d) ", elt->id, elt->type);
    switch ( elt->type )
    {
        // BASE SDL ELEMENTS
        case UI_ELT_POINT:       // (0) P
            printf("\e[31m point=0");
            break;
        case UI_ELT_LINE:        // (1) L
            printf("\e[31m line=1");
            break;
        case UI_ELT_RECT:        // (2) R
            printf("\e[31m rect=2");
            break;
        case UI_ELT_TEXT:        // (3) Txt
            printf("\e[31m text=3");
            break;
        case UI_ELT_TEXTURE:     // (4) Tex
            printf("\e[31m texture=4");
            break;
        // MY CUSTOM ELEMENTS
        case UI_ELT_BT_RECT:     // (5) Btr
            printf("\e[31m bt_rect=5");
            break;
        case UI_ELT_BT_TEXTURE:  // (6) Btt
            printf("\e[31m bt_texture=6");
            break;
        case UI_ELT_CONTAINER:   // (7) Cont
            printf("\e[31m container=7");
            break;
        case UI_ELT_SCROLLBAR:   // (8) Sb
            printf("\e[31m scrollbar=8");
            break;
        case UI_ELT_TXT_INPUT:   // (9) Inp
            printf("\e[31m txt_input=9");
            break;

        default:
            printf("\e[31m UNKNOW");
            break;
    }
    printf("\e[m \n");
}

///////////////////////////
/* -- UTILS FUNCTIONS -- */
///////////////////////////

// Set the cl_dst rgba properties
void set_color(SDL_Color* cl_dst, int src_red, int src_green, int src_blue, int src_alpha){
    if(cl_dst == NULL){
        fprintf(stderr, "\e[31mError : Can't set a color from a NULL pointer\e[m\n");
        return;
    }
    //
    cl_dst->r = src_red;
    cl_dst->g = src_green;
    cl_dst->b = src_blue;
    cl_dst->a = src_alpha;
}

// Set the rect xywh properties
void set_rect(SDL_Rect* rect, int x, int y, int w, int h){
    if(rect == NULL){
        fprintf(stderr, "\e[31mError : Can't set a rect from a NULL pointer\e[m\n");
        return;
    }
    //
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

SDL_Color ui_cl(int r, int g, int b, int a){
    SDL_Color cl;
    cl.r = r;
    cl.g = g;
    cl.b = b;
    cl.a = a;
    return cl;
}

int minui(int a, int b){
    if(a <= b) return a;
    return b;
}

int maxui(int a, int b){
    if(a >= b) return a;
    return b;
}


int clampui(int value, int min_value, int max_value){
    if(value < min_value) return min_value;
    if(value > max_value) return max_value;
    return value;
}

// Create texture from text
SDL_Texture* ui_texture_from_text(Ui* ui, const char txt[], SDL_Color color, size_t ui_font_id){
    if(ui_font_id >= ui->nb_fonts){
        fprintf(stderr, "\e[31m ERROR : Try to render text with a font that doesn't exists !!!! \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    // Rendering the text into a texture
    SDL_Surface* surface_txt = TTF_RenderText_Solid(ui->fonts[ui_font_id], txt, color);
    if(surface_txt == NULL){
        fprintf(stderr, "\e[31mERROR : Can't render text !\e[m\n");
        ui_free(ui);
        exit(42);
    }
    SDL_Texture* texture_txt = SDL_CreateTextureFromSurface(ui->renderer, surface_txt);
    if(texture_txt == NULL){
        fprintf(stderr, "\e[31mERROR : Can't render texture from surface !\e[m\n");
        ui_free(ui);
        exit(42);
    }
    // Fr
    SDL_FreeSurface(surface_txt);
    //
    // printf("\e[35mCreated texture from text : \"%s\"\e[m\n", txt);
    //
    return texture_txt;
}

////////////////////////
/* -- UI FUNCTIONS -- */
////////////////////////

// Init the UI
Ui* ui_init(size_t nb_windows){
    Ui* ui = (Ui*)calloc(sizeof(Ui), 1);
    if(ui == NULL){
        fprintf(stderr, "\e[31mError MALLOC : can't create UI !\e[m\n");
        exit(42);
    }
    //

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "error initializing SDL: %s\n", SDL_GetError());
        exit(42);
    }
    // Init TTF
    if(TTF_Init() != 0){
        fprintf(stderr, "error initializing TTF: %s\n", TTF_GetError());
        exit(42);
    }
    // Create the window
    ui->window = SDL_CreateWindow(  WIN_TITLE,
                                    WIN_POS_X, WIN_POS_Y,
                                    WIN_WIDTH, WIN_HEIGHT,
                                    WIN_FLAGS);
    if(ui->window == NULL){
        fprintf(stderr, "error initializing SDL Window: %s\n", SDL_GetError());
        ui_free(ui);
        exit(42);
    }
    ui->renderer = SDL_CreateRenderer(ui->window, -1, SDL_RENDERER_ACCELERATED);
    if(ui->renderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        ui_free(ui);
        exit(42);
    }
    //
    ui->windows = (Ui_Window**)calloc(sizeof(Ui_Window*), nb_windows);
    if(ui->windows == NULL){
        fprintf(stderr, "Malloc Error, can't init the windows array!\n");
        ui_free(ui);
        exit(42);
    }
    ui->nb_windows = nb_windows;
    //
    ui->fonts_capacity = 4;
    ui->nb_fonts = 0;
    ui->fonts = (TTF_Font**)calloc(sizeof(TTF_Font*), ui->fonts_capacity);
    //
    return ui;
}


void ui_init_window(Ui* ui, size_t id_window){
    //
    if(ui->windows[id_window] != NULL){
        fprintf(stderr, "\e[31mError \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    if(id_window >= ui->nb_windows){
        fprintf(stderr, "Error, window id %ld is too large (>=%ld)!\n", id_window, ui->nb_windows);
        ui_free(ui);
        exit(42);
    }
    //
    ui->windows[id_window] = (Ui_Window*)calloc(1, sizeof(Ui_Window));
    if(ui->windows[id_window] == NULL){
        fprintf(stderr, "\e[31mError malloc : window \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    ui->current_window = id_window;
    //
    set_color(&(ui->windows[id_window]->bg_color), 0, 0, 0, 255);
    //
    ui->windows[id_window]->elements_capacity = 8;
    ui->windows[id_window]->nb_elements = 0;
    ui->windows[id_window]->elements = (Ui_Element**)calloc(ui->windows[id_window]->elements_capacity, sizeof(Ui_Element*));
    if(ui->windows[id_window]->elements == NULL){
        fprintf(stderr, "\e[31mError malloc : elements \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    ui->windows[id_window]->length_on_click_events = 0;
    ui->windows[id_window]->capacity_on_click_events = 2;
    ui->windows[id_window]->on_click_events = (Ui_Event*)calloc(ui->windows[id_window]->capacity_on_click_events, sizeof(Ui_Event));
    if(ui->windows[id_window]->on_click_events == NULL){
        fprintf(stderr, "\e[31mError malloc : on_click events \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    ui->windows[id_window]->length_on_keypress_events = 0;
    ui->windows[id_window]->capacity_on_keypress_events = 2;
    ui->windows[id_window]->on_keypress_events = (Ui_Event*)calloc(ui->windows[id_window]->capacity_on_keypress_events, sizeof(Ui_Event));
    if(ui->windows[id_window]->on_keypress_events == NULL){
        fprintf(stderr, "\e[31mError malloc : on keypress events \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    ui->windows[id_window]->delay_at_mainloop_end = 10;
    //
}

// Add a font to the UI
void add_font(Ui* ui, const char* font_filepath, int font_size){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        ui_free(ui);
        exit(42);
    }
    //

    // Testing if we need to extend the font capacity to store a new one
    if(ui->nb_fonts >= ui->fonts_capacity){
        ui->fonts_capacity *= 2;
        ui->fonts = (TTF_Font**)realloc(ui->fonts, sizeof(TTF_Font*) * (ui->fonts_capacity));
        if(ui->fonts == NULL){
            fprintf(stderr, "\e[31mMalloc Error, cannot expand fonts capacity!\e[m\n");
            ui_free(ui);
            exit(42);
        }
    }

    // Adding the font
    TTF_Font* new_font = TTF_OpenFont(font_filepath, font_size);
    if(new_font == NULL){
        fprintf(stderr, "\e[31m TTF ERROR : \n ! %s\n -> \"%s\"\e[m\n",  TTF_GetError(), font_filepath);
        ui_free(ui);
        exit(42);
    }
    ui->fonts[ui->nb_fonts] = new_font;
    ui->nb_fonts+=1;
}

// Change the background color of the UI
void ui_set_bg_color(Ui* ui, int r, int g, int b){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    set_color(&(ui->windows[ui->current_window]->bg_color), r, g, b, 255);
}

void ui_set_renderer_draw_color(Ui* ui, SDL_Color* cl){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        ui_free(ui);
        exit(42);
    }
    if(cl == NULL){
        fprintf(stderr, "\e[31mError : can't set color, color is NULL !\e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    if(0 != SDL_SetRenderDrawColor(ui->renderer, cl->r, cl->g, cl->b, cl->a)){
        fprintf(stderr, "\e[31mErreur SDL_SetRenderDrawColor : %s\e[m\n", SDL_GetError());
        ui_free(ui);
        exit(42);
    }
}

void ui_clear_renderer(Ui* ui){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    if(0 != SDL_RenderClear(ui->renderer)){
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        ui_free(ui);
        exit(42);
    }
}

// Function to display a element
void ui_display_element(Ui* ui, Ui_Element* elt, int parent_X, int parent_Y){
    int elt_X = parent_X + elt->x;
    int elt_Y = parent_Y + elt->y;
    if(elt->parent != NULL && elt->parent->type == UI_ELT_CONTAINER && elt->sb_parent_container == NULL){
        elt_X += elt->parent->container_view_dec_X;
        elt_Y += elt->parent->container_view_dec_Y;
    }
    elt->absolute_calculated_X = elt_X;
    elt->absolute_calculated_Y = elt_Y;
    SDL_Rect elt_Rect = {.x = elt_X, .y = elt_Y, .w = elt->rect.w, .h = elt->rect.h};
    SDL_Rect tmp_Rect = {.x = elt_X + elt->padding_left, .y = elt_Y + elt->padding_top, .w = elt_Rect.w - elt->padding_left - elt->padding_right, .h = elt_Rect.h - elt->padding_top - elt->padding_bottom};
    SDL_Color tmp_color = elt->color;
    ui_set_renderer_draw_color(ui, &(COLOR_WHITE));
    //
    switch ( elt->type )
    {
        // BASE SDL ELEMENTS
        case UI_ELT_POINT:       // (0) P
            ui_set_renderer_draw_color(ui, &(elt->color));
            SDL_RenderDrawPoint(ui->renderer, elt_X, elt_Y);
            break;

        case UI_ELT_LINE:        // (1) L
            ui_set_renderer_draw_color(ui, &(elt->color));
            SDL_RenderDrawLine(ui->renderer, elt_X, elt_Y, parent_X + elt->x2, parent_Y + elt->y2);
            break;
        
        case UI_ELT_RECT:        // (2) R
            ui_set_renderer_draw_color(ui, &(elt->color));
            if(elt->fill)
                SDL_RenderFillRect(ui->renderer, &elt_Rect);
            else
                SDL_RenderDrawRect(ui->renderer, &elt_Rect);
            break;
        
        case UI_ELT_TEXT:        // (3) Txt
            // Render text 
            SDL_QueryTexture(elt->texture, NULL, NULL, &(tmp_Rect.w), &(tmp_Rect.h));
            SDL_RenderCopy(ui->renderer, elt->texture, NULL, &(tmp_Rect));
            break;
        
        case UI_ELT_TEXTURE:     // (4) Tex
            SDL_RenderCopy(ui->renderer, elt->texture, NULL, &(elt_Rect));
            break;
        
        // MY CUSTOM ELEMENTS
        case UI_ELT_BT_RECT:     // (5) Btr
            if(elt->bt_pressed){
                tmp_color.r *= 0.5;
                tmp_color.g *= 0.5;
                tmp_color.b *= 0.5;
            }
            else{
                if(SDL_PointInRect(&(ui->mouse_position), &(elt_Rect))){
                    tmp_color.r *= 0.9;
                    tmp_color.g *= 0.9;
                    tmp_color.b *= 0.9;
                }
            }
            ui_set_renderer_draw_color(ui, &(tmp_color));
            SDL_RenderFillRect(ui->renderer, &elt_Rect);
            // Render text 
            SDL_QueryTexture(elt->texture, NULL, NULL, &(tmp_Rect.w), &(tmp_Rect.h));
            tmp_Rect.x = elt_Rect.x + (elt_Rect.w - tmp_Rect.w) / 2;
            tmp_Rect.y = elt_Rect.y + (elt_Rect.h - tmp_Rect.h) / 2;
            SDL_RenderCopy(ui->renderer, elt->texture, NULL, &(tmp_Rect));
            break;
        
        case UI_ELT_BT_TEXTURE:  // (6) Btt
            if(elt->bt_toggle_mode && elt->bt_pressed && elt->texture3 != NULL){
                SDL_RenderCopy(ui->renderer, elt->texture3, NULL, &(elt_Rect));
            }
            else{
                SDL_RenderCopy(ui->renderer, elt->texture, NULL, &(elt_Rect));
            }
            // Render text 
            SDL_QueryTexture(elt->texture2, NULL, NULL, &(tmp_Rect.w), &(tmp_Rect.h));
            tmp_Rect.x = elt_Rect.x + (elt_Rect.w - tmp_Rect.w) / 2;
            tmp_Rect.y = elt_Rect.y + (elt_Rect.h - tmp_Rect.h) / 2;
            SDL_RenderCopy(ui->renderer, elt->texture2, NULL, &(tmp_Rect));
            break;
        
        case UI_ELT_CONTAINER:   // (7) Cont
            break;
    
        case UI_ELT_SCROLLBAR:   // (8) Sb
            // Render background
            ui_set_renderer_draw_color(ui, &(elt->sb_bg_color));
            SDL_RenderFillRect(ui->renderer, &elt_Rect);
            //
            ui_set_renderer_draw_color(ui, &(elt->sb_curs_color));
            if(elt->sb_axis == UI_SB_HORIZONTAL){
                SDL_RenderFillRect(
                    ui->renderer, 
                    &((SDL_Rect){
                        .x=elt_X + (int)((double)((double)elt->sb_value / (double)(elt->sb_end - elt->sb_start))*(double)elt->w),
                        .y=elt_Y,
                        .w=minui( ((double)((double)elt->sb_step / (double)(elt->sb_end - elt->sb_start))*(double)elt->w) , 5), 
                        .h=elt->h
                    }
                ));
                //
            }
            else{
                // printf("Scroll x (%s) : %d\n", elt->id, elt_X);
                // printf("Scroll y (%s) : %d\n", elt->id, elt_Y + (int)((double)((double)elt->sb_value / (double)(elt->sb_end - elt->sb_start))*(double)elt->h));
                // printf("Scroll w (%s) : %d\n", elt->id, elt->w);
                // printf("Scroll h (%s) : %d\n", elt->id, maxui( ((double)((double)elt->sb_step / (double)(elt->sb_end - elt->sb_start))*(double)elt->h) , 5));
                SDL_RenderFillRect(
                    ui->renderer,
                    &((SDL_Rect){
                        .x=elt_X,
                        .y=elt_Y + (int)((double)((double)elt->sb_value / (double)(elt->sb_end - elt->sb_start))*(double)elt->h),
                        .w=elt->w,
                        .h=maxui( ((double)((double)elt->sb_step / (double)(elt->sb_end - elt->sb_start))*(double)elt->h) , 5)
                    }
                ));
                //
            }
            //
            break;
        
        case UI_ELT_TXT_INPUT:   // (9) Inp
            // Render the background rect
            if(SDL_PointInRect(&(ui->mouse_position), &(elt_Rect))){
                tmp_color.r *= 0.9;
                tmp_color.g *= 0.9;
                tmp_color.b *= 0.9;
            }
            ui_set_renderer_draw_color(ui, &(tmp_color));
            SDL_RenderFillRect(ui->renderer, &elt_Rect);
            // Draw each letter
            int dec_X = elt->padding_left;
            int dec_Y = elt->padding_top;
            for(size_t i=0; i<elt->input_length; i++){
                SDL_Texture* tl = elt->input_txt_textures[i];
                int texture_width; int texture_height;
                SDL_QueryTexture(tl, NULL, NULL, &texture_width, &texture_height);
                SDL_RenderCopy(ui->renderer, tl, NULL, &((SDL_Rect){.x = elt_X + dec_X, .y = elt_Y + dec_Y, .w = texture_width, .h = texture_height}));
                // Draw the cursor if focused
                if(ui->windows[ui->current_window]->focused_element == elt){
                    if(time(NULL) % 2 != 0 && elt->input_cursor_position == i){
                        ui_set_renderer_draw_color(ui, &(COLOR_BLACK));
                        //SDL_RenderFillRect(ui->renderer, &((SDL_Rect){.x = elt_X + dec_X, .y = elt_Y, .w = 2, .h = texture_height}));
                        SDL_RenderFillRect(ui->renderer, &((SDL_Rect){.x = elt_X + dec_X, .y = elt_Y + dec_Y, .w = 2, .h = elt->rect.h - elt->padding_top - elt->padding_bottom}));
                    }
                }
                //
                dec_X += texture_width;
            }

            if(ui->windows[ui->current_window]->focused_element == elt && elt->input_cursor_position == elt->input_length && time(NULL) % 2 != 0 ){
                ui_set_renderer_draw_color(ui, &(elt->color2));
                SDL_RenderFillRect(ui->renderer, &((SDL_Rect){.x = elt_X + dec_X, .y = elt_Y, .w = 2, .h = elt->rect.h - elt->padding_top - elt->padding_bottom}));
            }

            //
            break;

        default:
            break;
    }

    //
    if(elt->nb_children > 0){
        ui_display_container(ui, elt, elt_X, elt_Y, elt_Rect.w, elt_Rect.h);
    }

    // Focus
    if(ui->windows[ui->current_window]->focused_element == elt){
        SDL_Color cl = COLOR_FOCUSED_ELT;
        ui_set_renderer_draw_color(ui, &(cl));
        SDL_RenderDrawRect(ui->renderer, &(elt_Rect));
    }

    // Grab
    if(ui->windows[ui->current_window]->grabbed_elt == elt){
        SDL_Color cl = COLOR_YELLOW;
        ui_set_renderer_draw_color(ui, &(cl));
        SDL_RenderDrawRect(ui->renderer, &(elt_Rect));
    }

}

// Auxiliar function 
void ui_display_container(Ui* ui, Ui_Element* elt, int posX, int posY, int width, int height){
    // Finalise the render for previous display before changing the viewport
    //
    if(elt->overflow_mode == UI_OVERFLOW_HIDE){
        SDL_RenderSetClipRect(ui->renderer, &((SDL_Rect){.x=posX, .y=posY, .w=width, .h=height}));
    }
    else{
        SDL_RenderSetClipRect(ui->renderer, &((SDL_Rect){.x=0, .y=0, .w=WIN_WIDTH, .h=WIN_HEIGHT}));
    }
    //
    if(elt->type == UI_ELT_CONTAINER && elt->overflow_mode != UI_OVERFLOW_SHOW){
        for(size_t i=0; i<elt->nb_visible_children; i++){
            if(elt->visible_children[i] == elt->cont_sb_horizontal || elt->visible_children[i] == elt->cont_sb_vertical) continue;
            if(elt->visible_children[i] == NULL) continue;
            Ui_Element* child = elt->visible_children[i];
            //
            if(!child->visible) continue;
            //
            ui_display_element(ui, child, posX+elt->padding_left, posY+elt->padding_top);
        }
        //
        if(elt->cont_sb_horizontal != NULL && elt->cont_sb_horizontal->visible){
            ui_display_element(ui, elt->cont_sb_horizontal, posX, posY);
        }
        if(elt->cont_sb_vertical != NULL && elt->cont_sb_vertical->visible){
            ui_display_element(ui, elt->cont_sb_vertical, posX, posY);
        }
    }
    else{
        for(size_t i=0; i<elt->nb_children; i++){
            if(elt->children[i] == NULL) continue;
            Ui_Element* child = elt->children[i];
            //
            if(!child->visible) continue;
            //
            ui_display_element(ui, child, posX+elt->padding_left, posY+elt->padding_top);
        }
    }
    //
}


// Display the UI
void ui_display(Ui* ui){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    if(!(ui->windows[ui->current_window]->initial_visible_elements_detection)){
        ui->windows[ui->current_window]->initial_visible_elements_detection = true;
        for(size_t j = 0; j<ui->windows[ui->current_window]->nb_elements; j++){
            if(ui->windows[ui->current_window]->elements[j] != NULL){
                detect_visible_children(ui->windows[ui->current_window]->elements[j], 0, 0, true);
            }
        }
    }
    // Clean Display
    ui_set_renderer_draw_color(ui, &(ui->windows[ui->current_window]->bg_color));
    ui_clear_renderer(ui);
    // Display all the elements
    // TODO : effects on hover
    //
    for(size_t i=0; i<ui->windows[ui->current_window]->nb_elements; i++){
        if(ui->windows[ui->current_window]->elements[i] == NULL) continue;
        Ui_Element* elt = ui->windows[ui->current_window]->elements[i];
        //
        if(!elt->visible) continue;
        //
        SDL_RenderSetClipRect(ui->renderer, &((SDL_Rect){.x=0, .y=0, .w=WIN_WIDTH, .h=WIN_HEIGHT}));
        ui_display_element(ui, elt, 0, 0);
    }
    // Finalise the render
    SDL_RenderPresent(ui->renderer);
}

// Recursive function to get the clicked element
bool f_aux_ui_get_id_of_clicked_element(Ui* ui, Ui_Element* elt, int parentX, int parentY, char id_elt_trouve[100]){
    // We go through each child, by the last added to the first added
    // because the last element added is above the first element added
    // Long long int instead of size_t for i is because i decrease, and the loop stops when i = -1 (<- negative number)
    for(long long int i=elt->nb_children - 1; i>=0; i--){
        // If the child is NULL, we do nothing
        if(elt->children[i] == NULL) continue;
        // Else, we get the child variable
        Ui_Element* child = elt->children[i];
        // If the child is not visible, we cannot click it
        if(!child->visible) continue;
        // Collision test
        // We get the absolute coordinates on the screen for the child element
        int child_X = parentX + child->x;
        int child_Y = parentY + child->y;
        if(elt->parent!= NULL && elt->parent->type == UI_ELT_CONTAINER && elt->parent->overflow_mode != UI_OVERFLOW_SHOW){
            child_X += elt->parent->container_view_dec_X;
            child_Y += elt->parent->container_view_dec_Y;
        }
        SDL_Rect child_Rect = {.x = child_X, .y = child_Y, .w = child->w, .h = child->h};
        // If the child is clickable, we test the collision with the mouse cursor
        if(child->clickable){
            // Collision test
            if(SDL_PointInRect(&(ui->mouse_position), &(child_Rect))){
                // We copy the id of the clicked element 
                strcpy(id_elt_trouve, child->id);
                return true;
            }
        }
        // We repeat this function recursively, because each child can also have its own childs
        if(f_aux_ui_get_id_of_clicked_element(ui, child, child_X, child_Y, id_elt_trouve)){
            return true;
        }
    }
    // No elements have been found
    return false;
}

// Get id of Clicked Element
// Put the id of the first element that is clicked in `id_elt`
void ui_get_id_of_clicked_element(Ui* ui, char id_elt_trouve[100]){
    if(ui == NULL){
        fprintf(stderr, "\e[31mError : Can't get clicked elements of UI because is NULL !\e[m\n");
        return;
    }
    //
    for(long long int i=ui->windows[ui->current_window]->nb_elements - 1; i>=0; i--){
        if(ui->windows[ui->current_window]->elements[i] == NULL) continue;
        Ui_Element* elt = ui->windows[ui->current_window]->elements[i];
        //
        if(!elt->visible) continue;
        // Collision test
        if(elt->clickable){
            if(SDL_PointInRect(&(ui->mouse_position), &(elt->rect))){
                strcpy(id_elt_trouve, elt->id);
                return;
            }
        }
        // Child collision test
        if(f_aux_ui_get_id_of_clicked_element(ui, elt, elt->x, elt->y, id_elt_trouve)){
            return;
        }
        //
    }
    // No element is clicked this frame
    strcpy(id_elt_trouve, "");
}

// Free an UI element
void ui_free_element(Ui_Element** elt_p){
    Ui_Element* elt = *elt_p;
    //
    if(elt == NULL){
        fprintf(stderr, "\e[31mError: Can't free an element that is NULL !\e[m\n");
        return;
    }
    if(elt->is_freeing){
        return;
    }
    elt->is_freeing = true;
    //
    // Free the children elements
    for(size_t i=0; i < elt->nb_children; i++){
        ui_free_element(&(elt->children[i]));
    }
    //
    free(elt->children);
    //
    if(elt->surface != NULL){
        SDL_FreeSurface(elt->surface);
    }
    //
    if(elt->texture != NULL){
        SDL_DestroyTexture(elt->texture);
    }
    //
    if(elt->texture2 != NULL){
        SDL_DestroyTexture(elt->texture2);
    }
    //
    if(elt->texture3 != NULL){
        SDL_DestroyTexture(elt->texture3);
    }
    //
    if(elt->interactions_on_hover != NULL){
        for(size_t i=0; i<elt->nb_interactions_on_hover; i++){
            free(elt->interactions_on_hover[i]);
        }
        //
        free(elt->interactions_on_hover);
    }
    //
    if(elt->input_txt != NULL){
        free(elt->input_txt);
    }
    //
    if(elt->input_length > 0){
        for(size_t i=0; i<elt->input_length; i++){
            SDL_DestroyTexture(elt->input_txt_textures[i]);
        }
    }
    // Free events
    for(size_t i=0; i<elt->length_on_click_events; i++){
        if(elt->on_click_events[i].func_args != NULL){
            free(elt->on_click_events[i].func_args);
        }
    }
    free(elt->on_click_events);
    //
    for(size_t i=0; i<elt->length_on_keypress_events; i++){
        if(elt->on_keypress_events[i].func_args != NULL){
            free(elt->on_keypress_events[i].func_args);
        }
    }
    free(elt->on_keypress_events);

    // Free the element struct
    free(elt);
    //
    *elt_p = NULL;
}

// Free the UI
void ui_free(Ui* ui){
    if(ui == NULL){
        fprintf(stderr, "\e[31mError : Can't free UI because UI is NULL !\e[m\n");
        return;
    }
    // Free all the windows
    for(size_t k=0; k<ui->nb_windows; k++){
        // Free all the elements
        for(size_t i=0; i<ui->windows[k]->nb_elements; i++){
            if(ui->windows[k]->elements[i] != NULL){
                ui_free_element(&(ui->windows[k]->elements[i]));
                ui->windows[k]->elements[i] = NULL;
            }
        }
        //
        free(ui->windows[k]->elements);
        //
        //
        free(ui->windows[k]->on_click_events);
        free(ui->windows[k]->on_keypress_events);
    }

    // Destroy all the fonts
    for(size_t i=0; i<ui->nb_fonts; i++){
        if(ui->fonts[i] != NULL){
            TTF_CloseFont(ui->fonts[i]);
            ui->fonts[i] = NULL;
        }
    }
    free(ui->fonts);

    // Destroy renderer
    if(ui->renderer != NULL)
        SDL_DestroyRenderer(ui->renderer);

    // Destroy window
    if(ui->window != NULL)
        SDL_DestroyWindow(ui->window);

    // Quit SDL
    SDL_Quit();

    // Free ui
    free(ui);

}

//
void ui_pre_event(Ui* ui){
    // Update mouse position
    SDL_GetMouseState(&(ui->mouse_position.x), &(ui->mouse_position.y));
}

// Remove all the elements on the window
void ui_clean_window(Ui* ui, size_t id_window){
    for(size_t i=0; i<ui->windows[id_window]->nb_elements; i++){
        if(ui->windows[id_window]->elements[i] != NULL){
            ui_free_element(&(ui->windows[id_window]->elements[i]));
            ui->windows[id_window]->elements[i] = NULL;
        }
    }
    ui->windows[id_window]->nb_elements = 0;
}

//////////////////////////////
/* -- ELEMENTS FUNCTIONS -- */
//////////////////////////////

// Detect visible children of container element
void detect_visible_children(Ui_Element* parent_elt, int abs_parent_posX, int abs_parent_posY, bool recursive){
    // This function is useful only for containers that hide overflowed elements
    bool true_viewed = false;
    // clean the visible_children array
    parent_elt->nb_visible_children = 0;
    // We go through each child
    // fprintf(stderr, "Detect visible children of element \"%s\"\n", parent_elt->id);
    for(size_t i=0; i<parent_elt->nb_children; i++){
        // If the child is NULL, we do nothing
        if(parent_elt->children[i] == NULL) continue;
        // Else, we get the child variable
        Ui_Element* child = parent_elt->children[i];
        // If the child is not visible, it can't be visible
        if(!child->visible) continue;
        //
        // Collision test
        // We get the absolute coordinates on the screen for the child element
        int child_X = abs_parent_posX + child->x + parent_elt->container_view_dec_X;
        int child_Y = abs_parent_posY + child->y + parent_elt->container_view_dec_Y;
        child->absolute_calculated_X = child_X;
        child->absolute_calculated_Y = child_Y;

        if(parent_elt->type == UI_ELT_CONTAINER && parent_elt->overflow_mode != UI_OVERFLOW_SHOW){

            SDL_Rect child_Rect = {
                .x = child_X,
                .y = child_Y,
                .w = child->w,
                .h = child->h
            };
            SDL_Rect parent_Rect = {
                .x = abs_parent_posX,
                .y = abs_parent_posY,
                .w = parent_elt->w,
                .h = parent_elt->h
            };
            SDL_Rect collision_Rect = {0, 0, 0, 0};
            if(SDL_IntersectRect(&(child_Rect), &(parent_Rect), &(collision_Rect))){
                parent_elt->visible_children[parent_elt->nb_visible_children] = child;
                parent_elt->nb_visible_children++;
                //
                if(!true_viewed){
                    true_viewed = true;
                }
            }
            else{
                if(true_viewed){
                    if(parent_elt->overflow_mode == UI_HBOX_CONTAINER || parent_elt->overflow_mode == UI_VBOX_CONTAINER){
                        return;
                    }
                }
            }
        }
        //
        if(recursive){
            detect_visible_children(child, child_X, child_Y, recursive);
        }
    }
}

// Create an empty element for UI
Ui_Element* ui_init_empty_element(const char id[]){
    Ui_Element* elt = (Ui_Element*)calloc(sizeof(Ui_Element), 1);
    //
    elt->texture = NULL;
    elt->interactions_on_hover = NULL;
    //
    elt->visible = true;
    //
    strcpy(elt->id, id);
    //
    elt->parent = NULL;
    elt->children_capacity = 1;
    elt->nb_children = 0;
    elt->children = (Ui_Element**)calloc(sizeof(Ui_Element*), elt->children_capacity);
    elt->visible_children = (Ui_Element**)calloc(sizeof(Ui_Element*), elt->children_capacity);
    //
    elt->length_on_click_events = 0;
    elt->capacity_on_click_events = 2;
    elt->on_click_events = (Ui_Event*)calloc(sizeof(Ui_Event), elt->capacity_on_click_events);
    //
    elt->length_on_keypress_events = 0;
    elt->capacity_on_keypress_events = 2;
    elt->on_keypress_events = (Ui_Event*)calloc(sizeof(Ui_Event), elt->capacity_on_keypress_events);
    //
    elt->padding_top = 0;
    elt->padding_left = 0;
    elt->padding_bottom = 0;
    elt->padding_left = 0;
    //
    return elt;
}

// Get element idx with id
size_t ui_get_element_id(Ui* ui, const char id[]){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    for(size_t idx=0; idx<ui->windows[ui->current_window]->nb_elements; idx++){
        if(ui->windows[ui->current_window]->elements[idx] == NULL) continue;
        if(strcmp(ui->windows[ui->current_window]->elements[idx]->id, id)==0){
            return idx;
        }
    }
    //
    fprintf(stderr, "\e[31mError, element with id \"%s\" is not in the list !\e[m\n", id);
    exit(42);
    return 0;
}

// Get element node with id
// (please try not to do something stupid if you change Ui_Element properties out of `lib_ui` functions)
Ui_Element* ui_get_element(Ui* ui, const char id[]){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    return ui->windows[ui->current_window]->elements[ui_get_element_id(ui, id)];
}

// Get element node with id
// (please try not to do something stupid if you change Ui_Element properties out of `lib_ui` functions)
Ui_Element* ui_get_element_rec_f_aux(Ui_Element* elt, char id_to_search[]){
    // Correct arguments test
    if(elt == NULL){
        fprintf(stderr, "\e[31mUI Error : elt is NULL!\e[m\n");
        exit(42);
    }
    // We go through each children
    for(size_t i = 0; i<elt->nb_children; i++){
        // If the child is NULL, we do nothing
        if(elt->children[i] == NULL) continue;
        // Otherwise, we get the child variable
        Ui_Element* child = elt->children[i];
        // If the child id is the same as the one we are looking for
        if(strcmp(child->id, id_to_search) == 0){
            // We return the child element
            return child;
        }
        // Otherwise, we search into the child's children
        Ui_Element* res = ui_get_element_rec_f_aux(child, id_to_search);
        // If one children has been found, we return it
        if(res != NULL){
            return res;
        }
    }
    // Otherwise, we return NULL
    return NULL;
}

// Get element node with id
// (please try not to do something stupid if you change Ui_Element properties out of `lib_ui` functions)
Ui_Element* ui_get_element_rec(Ui* ui, char id_to_search[]){
    // Correct arguments test
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    // fprintf(stderr, "Ui ne elements = %ld\n", ui->nb_elements);
    // We go through each elements of the ui
    for(size_t i = 0; i<ui->windows[ui->current_window]->nb_elements; i++){
        // If the element is NULL, we do nothing
        if(ui->windows[ui->current_window]->elements[i] == NULL) continue;
        // Otherwise, we get the element variable
        Ui_Element* elt = ui->windows[ui->current_window]->elements[i];
        // If the id of the element is the same as the one we are looking for
        if(strcmp(elt->id, id_to_search) == 0){
            // We return the element
            return elt;
        }
        // Otherwise, we search into the element children
        Ui_Element* res = ui_get_element_rec_f_aux(elt, id_to_search);
        // If an element has been found
        if(res != NULL){
            // We return it
            return res;
        }
    }
    // Otherwise, we return an error message and exit the program
    fprintf(stderr, "\e[31mError, the element with id \"%s\" is not in the list !\e[m\n", id_to_search);
    exit(42);
    return NULL;
}


// Delete an ui with id
void ui_delete_element(Ui* ui, const char id[], bool move){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    size_t idx = ui_get_element_id(ui, id);
    //
    ui_free_element(&(ui->windows[ui->current_window]->elements[idx]));
    ui->windows[ui->current_window]->elements[idx] = NULL;
    //
    if(move){
        // We move all the elements at the right back of one case
        for(size_t j=idx; j<ui->windows[ui->current_window]->nb_elements-1; j++){
            ui->windows[ui->current_window]->elements[j] = ui->windows[ui->current_window]->elements[j+1];
        }
        //
        ui->windows[ui->current_window]->nb_elements--;
    }
}

// Adding the elt to the ui
void ui_append_elt_to_ui(Ui* ui, Ui_Element* elt){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    if(elt == NULL){
        fprintf(stderr, "\e[32mWarning : adding a NULL element to the UI !\e[m\n");
    }
    // Resize the array if it is full
    if(ui->windows[ui->current_window]->nb_elements >= ui->windows[ui->current_window]->elements_capacity){
        ui->windows[ui->current_window]->elements_capacity *= 2;
        ui->windows[ui->current_window]->elements = (Ui_Element**)realloc(ui->windows[ui->current_window]->elements, sizeof(Ui_Element*) * ui->windows[ui->current_window]->elements_capacity);
        if(ui->windows[ui->current_window]->elements == NULL){
            fprintf(stderr, "\e[31mMalloc Error!\e[m\n");
            exit(42);
        }
        //
        for(size_t i = ui->windows[ui->current_window]->nb_elements; i<ui->windows[ui->current_window]->elements_capacity; i++){
            ui->windows[ui->current_window]->elements[i] = NULL;
        }
    }
    // Adding the element to the array
    ui->windows[ui->current_window]->elements[ui->windows[ui->current_window]->nb_elements] = elt;
    ui->windows[ui->current_window]->nb_elements ++;
}

// Add an element to a parent element
void ui_append_elt_to_elt(Ui_Element* parent, Ui_Element* elt){
    if(parent == NULL){
        fprintf(stderr, "\e[31mUI_Element Error : Parent is NULL!\e[m\n");
        exit(42);
    }
    //
    if(elt == NULL){
        fprintf(stderr, "\e[32mWarning : adding a NULL element to the UI !\e[m\n");
        exit(42);
    }
    // Resize the array if it is full
    if(parent->nb_children >= parent->children_capacity){
        parent->children_capacity *= 2;
        parent->children = (Ui_Element**)realloc(parent->children, sizeof(Ui_Element*) * parent->children_capacity);
        parent->visible_children = (Ui_Element**)realloc(parent->visible_children, sizeof(Ui_Element*) * parent->children_capacity);
        if(parent->children == NULL || parent->visible_children == NULL){
            fprintf(stderr, "\e[31mMalloc Error!\e[m\n");
            exit(42);
        }
        //
        for(size_t i = parent->nb_children; i<parent->children_capacity; i++){
            parent->children[i] = NULL;
            parent->visible_children[i] = NULL;
        }
    }
    // Adding the element to the array
    // printf("Adding elt \"%s\" to \"%s\" at pos %ld / capa %ld\n", elt->id, parent->id, parent->nb_children, parent->children_capacity);
    parent->children[parent->nb_children] = elt;
    parent->nb_children++;
    elt->parent = parent;
    //
    int prev_row_max_height=0;  // For HGRID containers
    int prev_col_max_width=0;   // For VGRID containers
    Ui_Element* last_child = NULL; //
    size_t id_last_child = 0;
    for(long int j = parent->nb_children - 2; j>=0; j--){
        if(parent->children[j] == NULL || parent->children[j]->sb_parent_container != NULL){
            continue;
        }
        id_last_child = j;
        last_child = parent->children[j];
        // printf("Last child of \"%s\"is \"%s\"\n", parent->id, last_child->id);
        break;
    }
    if(!(elt->sb_parent_container != NULL)){
            
        switch (parent->overflow_mode)
        {
            case UI_VBOX_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                    break;
                }
                else{
                    elt->x = 0;
                    elt->y = last_child->y + last_child->h + parent->separation_y;
                }
                break;
            
            case UI_HBOX_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                    break;
                }
                else{
                    elt->x = last_child->x + last_child->w + parent->separation_x;
                    elt->y = 0;
                }
                break;

            case UI_VGRID_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                    break;
                }
                if(last_child->y + last_child->h >= parent->h){ // Break to a new column
                    // Getting the max previous column width
                    prev_col_max_width = last_child->w;
                    for(long long int i = id_last_child - 1; i >= 0 && (parent->children[i] == NULL || parent->children[i]->x == last_child->x); i--){
                        if(parent->children[i] == NULL){
                            continue;
                        }
                        if(parent->children[i]->w > prev_col_max_width){
                            prev_col_max_width = parent->children[i]->w;
                        }
                    }
                    //
                    elt->x = last_child->x + prev_col_max_width + parent->separation_x;
                    elt->y = 0;
                    //
                } else{ // Continue the column
                    elt->x = last_child->x;
                    elt->y = last_child->y + last_child->h + parent->separation_y;
                }
                break;
            
            case UI_HGRID_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                    break;
                }
                if(last_child->x + last_child->w >= parent->w){ // Break to a new line
                    // Getting the max previous row height
                    prev_row_max_height = last_child->h;
                    for(long long int i = id_last_child - 1; i >= 0 && (parent->children[i] == NULL || parent->children[i]->y == last_child->y); i--){
                        if(parent->children[i] == NULL){
                            continue;
                        }
                        if(parent->children[i]->h > prev_row_max_height){
                            prev_row_max_height = parent->children[i]->h;
                        }
                    }
                    //
                    elt->x = 0;
                    elt->y = last_child->y + prev_row_max_height + parent->separation_y;
                    //
                } else{ // Continue the line
                    elt->x = last_child->x + last_child->w + parent->separation_x;
                    elt->y = last_child->y;
                }
                break;

            default:
                break;
        }
    }
    // Update scrollbars
    switch (parent->overflow_mode)
    {
        // If no scrollbar
        case UI_OVERFLOW_HIDE:
        case UI_OVERFLOW_SHOW:
            break;

        default:
            // We search the max width & the max height
            if(elt->x+elt->w >= parent->cont_total_width){
                parent->cont_total_width = elt->x+elt->w;
                // If scrollbar, we need to update it
                if(parent->cont_sb_horizontal != NULL){
                    parent->cont_sb_horizontal->sb_end = parent->cont_total_width;
                }
            }
            if(elt->y+elt->h >= parent->cont_total_height){
                parent->cont_total_height = elt->y+elt->h;
                // If scrollbar, we need to update it
                if(parent->cont_sb_vertical != NULL){
                    parent->cont_sb_vertical->sb_end = parent->cont_total_height;
                }
            }
            break;
    }
    //
}

//
void update_container_elements_position(Ui_Element* container, long long int starts_from){
    if(container == NULL){
        fprintf(stderr, "\e[31mUI_Element Error : Container is NULL!\e[m\n");
        exit(42);
    }
    container->cont_total_height = 0;
    container->cont_total_width = 0;
    starts_from = clampui(starts_from, 0, container->nb_children);
    //
    for(size_t j=starts_from; j < container->nb_children; j++){
        //
        if(container->children[j] == NULL){
            continue;
        }
        if(container->children[j]->sb_parent_container != NULL){
            continue;
        }
        if(!(container->children[j]->visible)){
            continue;
        }
        //
        Ui_Element* elt = container->children[j];
        //
        int prev_row_max_height=0;  // For HGRID containers
        int prev_col_max_width=0;   // For VGRID containers
        Ui_Element* last_child = NULL; //
        size_t id_last_child = 0;
        for(long int jj = j - 1; jj>=0; jj--){
            //
            if(container->children[jj] == NULL){
                continue;
            }
            if(container->children[jj]->sb_parent_container != NULL){
                continue;
            }
            if(!(container->children[jj]->visible)){
                continue;
            }
            //
            id_last_child = jj;
            last_child = container->children[jj];
            break;
        }
        switch (container->overflow_mode)
        {
            case UI_VBOX_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                }
                else{
                    elt->x = 0;
                    elt->y = last_child->y + last_child->h + container->separation_y;
                }
                // printf("Set elt '%s' at position (x=%d, y=%d)\n", elt->id, elt->x, elt->y);
                break;
            
            case UI_HBOX_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                }
                else{
                    elt->x = last_child->x + last_child->w + container->separation_x;
                    elt->y = 0;
                }
                break;

            case UI_VGRID_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                }
                if(last_child->y + last_child->h >= container->h){ // Break to a new column
                    // Getting the max previous column width
                    prev_col_max_width = last_child->w;
                    for(long long int i = id_last_child - 1; i >= 0 && (container->children[i] == NULL || container->children[i]->x == last_child->x); i--){
                        if(container->children[i] == NULL){
                            continue;
                        }
                        if(container->children[i]->sb_parent_container != NULL){
                            continue;
                        }
                        if(!(container->children[i]->visible)){
                            continue;
                        }
                        //
                        if(container->children[i]->w > prev_col_max_width){
                            prev_col_max_width = container->children[i]->w;
                        }
                    }
                    //
                    elt->x = last_child->x + prev_col_max_width + container->separation_x;
                    elt->y = 0;
                    //
                } else{ // Continue the column
                    elt->x = last_child->x;
                    elt->y = last_child->y + last_child->h + container->separation_y;
                }
                break;
            
            case UI_HGRID_CONTAINER:
                if(last_child == NULL){
                    elt->x = 0;
                    elt->y = 0;
                }
                if(last_child->x + last_child->w >= container->w){ // Break to a new line
                    // Getting the max previous row height
                    prev_row_max_height = last_child->h;
                    for(long long int i = id_last_child - 1; i >= 0 && (container->children[i] == NULL || container->children[i]->y == last_child->y); i--){
                        if(container->children[i] == NULL){
                            continue;
                        }
                        if(container->children[i]->sb_parent_container != NULL){
                            continue;
                        }
                        if(!(container->children[i]->visible)){
                            continue;
                        }
                        //
                        if(container->children[i]->h > prev_row_max_height){
                            prev_row_max_height = container->children[i]->h;
                        }
                    }
                    //
                    elt->x = 0;
                    elt->y = last_child->y + prev_row_max_height + container->separation_y;
                    //
                } else{ // Continue the line
                    elt->x = last_child->x + last_child->w + container->separation_x;
                    elt->y = last_child->y;
                }
                break;

            default:
                break;
        }
        //
        if(elt->y+elt->h > container->cont_total_height){
            container->cont_total_height = elt->y+elt->h;
            // If scrollbar, we need to update it
            if(container->cont_sb_vertical != NULL){
                container->cont_sb_vertical->sb_end = container->cont_total_height;
                if(container->cont_sb_vertical->sb_value > container->cont_sb_vertical->sb_end){
                    container->cont_sb_vertical->sb_value = container->cont_sb_vertical->sb_end;
                }
            }
        }
        if(elt->x+elt->x > container->cont_total_width){
            container->cont_total_width = elt->x+elt->x;
            // If scrollbar, we need to update it
            if(container->cont_sb_horizontal != NULL){
                container->cont_sb_horizontal->sb_end = container->cont_total_width;
                if(container->cont_sb_horizontal->sb_value > container->cont_sb_horizontal->sb_end){
                    container->cont_sb_horizontal->sb_value = container->cont_sb_horizontal->sb_end;
                }
            }
        }
    }
}


// Add a point to the ui
Ui_Element* ui_create_point(Ui* ui, const char id[], int x, int y, SDL_Color cl){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_POINT;
    elt->x = x;
    elt->y = y;
    elt->color = cl;
    //
    return elt;
}

// Add a line to the ui
Ui_Element* ui_create_line(Ui* ui, const char id[], int x1, int y1, int x2, int y2, SDL_Color cl){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_LINE;
    elt->x = x1;
    elt->y = y1;
    elt->x2 = x2;
    elt->y2 = y2;
    elt->color = cl;
    //
    return elt;
}

// Add a rect to the ui
Ui_Element* ui_create_rect(Ui* ui, const char id[], int x, int y, int w, int h, bool fill_rect, SDL_Color cl){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_RECT;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    elt->fill = fill_rect;
    elt->color = cl;
    set_rect(&(elt->rect), x, y, w, h);
    elt->surface = SDL_CreateRGBSurface(0, w, h, 1, 0, 0, 0, 0);
    //
    return elt;
}

void ui_update_elt_text(Ui* ui, Ui_Element* elt, char new_txt[]){
    //
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    if(elt == NULL){
        fprintf(stderr, "\e[31mError : Elt is NULL!\e[m\n");
        exit(42);
    }
    if(elt->type != UI_ELT_TEXT){
        fprintf(stderr, "\e[31mError : Elt is not TEXT!\e[m\n");
        exit(42);
    }
    //
    SDL_DestroyTexture(elt->texture);
    elt->texture = NULL;
    strcpy(elt->txt, new_txt);
    elt->texture = ui_texture_from_text(ui, new_txt, elt->color, elt->font_id);
    SDL_QueryTexture(elt->texture, NULL, NULL, &(elt->w), &(elt->h));
    set_rect(&(elt->rect), elt->x, elt->y, elt->w, elt->h);
    //
}

// Add text to the ui
Ui_Element* ui_create_text(Ui* ui, const char id[], int x, int y, char txt[], SDL_Color cl, int fontsize, size_t ui_font_id){
    if(ui_font_id >= ui->nb_fonts){
        fprintf(stderr, "\e[31m ERROR : Try to render text with a font that doesn't exists !!!! \e[m\n");
        ui_free(ui);
        exit(42);
    }
    //
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_TEXT;
    strcpy(elt->txt, txt);
    elt->x = x;
    elt->y = y;
    elt->color = cl;
    elt->baseFontsize = fontsize;
    elt->font_id = ui_font_id;
    elt->texture = ui_texture_from_text(ui, txt, cl, ui_font_id);
    SDL_QueryTexture(elt->texture, NULL, NULL, &(elt->w), &(elt->h));
    set_rect(&(elt->rect), x, y, elt->w, elt->h);
    //
    return elt;
}

// Add a texture to the ui
Ui_Element* ui_create_texture(Ui* ui, const char id[], int x, int y, int w, int h, char texture_filepath[]){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    SDL_Texture* texture = IMG_LoadTexture(ui->renderer, texture_filepath);
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_TEXTURE;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    set_rect(&(elt->rect), x, y, w, h);
    elt->surface = SDL_CreateRGBSurface(0, w, h, 1, 0, 0, 0, 0);
    elt->texture = texture;
    //
    return elt;
}

// Add a button with a rectangle background to the ui
Ui_Element* ui_create_bt_rect(Ui* ui, const char id[], int x, int y, int w, int h, bool fill_rect, SDL_Color bg_cl , char txt[], SDL_Color txt_cl, int fontsize, size_t ui_font_id){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_BT_RECT;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    elt->fill = fill_rect;
    elt->color = bg_cl;
    elt->color2 = txt_cl;
    set_rect(&(elt->rect), x, y, w, h);
    elt->surface = SDL_CreateRGBSurface(0, w, h, 1, 0, 0, 0, 0);
    strcpy(elt->txt, txt);
    elt->baseFontsize = fontsize;
    elt->font_id = ui_font_id;
    elt->texture = ui_texture_from_text(ui, txt, txt_cl, ui_font_id);
    //
    elt->clickable = true;
    //
    return elt;
}

// Add a button with a texture background to the ui
Ui_Element* ui_create_bt_texture(Ui* ui, const char id[], int x, int y, int w, int h, char texture_filepath[], char txt[], SDL_Color txt_cl, int fontsize, size_t ui_font_id){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    // Rendering the text into a texture
    SDL_Surface* surface_txt = TTF_RenderText_Solid(ui->fonts[ui_font_id], txt, txt_cl); 
    SDL_Texture* texture_txt = SDL_CreateTextureFromSurface(ui->renderer, surface_txt);
    //
    SDL_Texture* texture_bg = IMG_LoadTexture(ui->renderer, texture_filepath);
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_BT_TEXTURE;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    elt->color = txt_cl;
    set_rect(&(elt->rect), x, y, w, h);
    elt->surface = SDL_CreateRGBSurface(0, w, h, 1, 0, 0, 0, 0);
    elt->texture = texture_bg;
    elt->texture2 = texture_txt;
    elt->surface2 = surface_txt;
    strcpy(elt->txt, txt);
    elt->baseFontsize = fontsize;
    elt->font_id = ui_font_id;
    //
    elt->clickable = true;
    return elt;
}


// Add a button with a texture background to the ui
Ui_Element* ui_create_toggle_bt_texture(Ui* ui, const char id[], int x, int y, int w, int h, char texture_bg_off_filepath[], char texture_bg_on_filepath[], char txt[], SDL_Color txt_cl, int fontsize, size_t ui_font_id){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    // Rendering the text into a texture
    SDL_Surface* surface_txt = TTF_RenderText_Solid(ui->fonts[ui_font_id], txt, txt_cl); 
    SDL_Texture* texture_txt = SDL_CreateTextureFromSurface(ui->renderer, surface_txt);
    //
    SDL_Texture* texture_bg_off = IMG_LoadTexture(ui->renderer, texture_bg_off_filepath);
    SDL_Texture* texture_bg_on = IMG_LoadTexture(ui->renderer, texture_bg_on_filepath);
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_BT_TEXTURE;
    elt->bt_toggle_mode = true;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    elt->color = txt_cl;
    set_rect(&(elt->rect), x, y, w, h);
    elt->surface = SDL_CreateRGBSurface(0, w, h, 1, 0, 0, 0, 0);
    elt->texture = texture_bg_off;
    elt->texture3 = texture_bg_on;
    elt->texture2 = texture_txt;
    elt->surface2 = surface_txt;
    strcpy(elt->txt, txt);
    elt->baseFontsize = fontsize;
    elt->font_id = ui_font_id;
    //
    elt->clickable = true;
    return elt;
}

// Create a container element
Ui_Element* ui_create_container(Ui* ui, const char id[], int x, int y, int w, int h, int overflow_mode, SDL_Color sb_bg_color, SDL_Color sb_curs_color, int sb_width){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_CONTAINER;
    elt->overflow_mode = overflow_mode;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    set_rect(&(elt->rect), x, y, w, h);
    //
    if(overflow_mode == UI_OVERFLOW_SCROLL_H || overflow_mode == UI_HBOX_CONTAINER || overflow_mode == UI_VGRID_CONTAINER){
        //
        char id_scroll_h[110] = "";
        if(strlen(elt->id) + strlen("_scroll_h") >= 100){
            fprintf(stderr, "\e[31mERROR : the id \"%s\" is too long !\e[m\n", elt->id);
            exit(42);
        }
        sprintf(id_scroll_h, "%s_scroll_h", elt->id);
        //
        elt->cont_sb_horizontal = ui_create_scrollbar(
            ui, id_scroll_h, elt->w - sb_width, 0, w, sb_width, 0, elt->cont_total_width, 20, UI_SB_HORIZONTAL, sb_bg_color, sb_curs_color
        );
        elt->cont_sb_horizontal->sb_parent_container = elt;
        ui_append_elt_to_elt(elt, elt->cont_sb_horizontal);
    }
    else if(overflow_mode == UI_OVERFLOW_SCROLL_V || overflow_mode == UI_VBOX_CONTAINER || overflow_mode == UI_HGRID_CONTAINER){
        //
        char id_scroll_v[110] = "";
        if(strlen(elt->id) + strlen("_scroll_v") >= 100){
            fprintf(stderr, "\e[31mERROR : the id \"%s\" is too long !\e[m\n", elt->id);
            exit(42);
        }
        sprintf(id_scroll_v, "%s_scroll_v", elt->id);
        //
        elt->cont_sb_vertical = ui_create_scrollbar(
            ui, id_scroll_v, elt->w - sb_width, 0, sb_width, h, 0, elt->cont_total_height, 20, UI_SB_VERTICAL, sb_bg_color, sb_curs_color
        );
        elt->cont_sb_vertical->sb_parent_container = elt;
        ui_append_elt_to_elt(elt, elt->cont_sb_vertical);
    }
    else if(overflow_mode == UI_OVERFLOW_SCROLL_HV){
        //
        char id_scroll_h[110] = "";
        if(strlen(elt->id) + strlen("_scroll_h") >= 100){
            fprintf(stderr, "\e[31mERROR : the id \"%s\" is too long !\e[m\n", elt->id);
            exit(42);
        }
        sprintf(id_scroll_h, "%s_scroll_h", elt->id);
        //
        elt->cont_sb_horizontal = ui_create_scrollbar(
            ui, id_scroll_h, 0, elt->h - sb_width, w-sb_width, sb_width, 0, elt->cont_total_width, 20, UI_SB_HORIZONTAL, sb_bg_color, sb_curs_color
        );
        elt->cont_sb_horizontal->sb_parent_container = elt;
        ui_append_elt_to_elt(elt, elt->cont_sb_horizontal);
        //
        char id_scroll_v[110] = "";
        if(strlen(elt->id) + strlen("_scroll_v") >= 100){
            fprintf(stderr, "\e[31mERROR : the id \"%s\" is too long !\e[m\n", elt->id);
            exit(42);
        }
        sprintf(id_scroll_v, "%s_scroll_v", elt->id);
        //
        elt->cont_sb_horizontal = ui_create_scrollbar(
            ui, id_scroll_v, elt->w - sb_width, 0, sb_width, h-sb_width, 0, elt->cont_total_height, 20, UI_SB_VERTICAL, sb_bg_color, sb_curs_color
        );
        elt->cont_sb_vertical->sb_parent_container = elt;
        ui_append_elt_to_elt(elt, elt->cont_sb_vertical);
    }
    //
    return elt;
}

// Create a scrollbar
Ui_Element* ui_create_scrollbar(Ui* ui, const char id[], int x, int y, int w, int h, int val_start, int val_end, int val_step, int sb_axis, SDL_Color bg_color, SDL_Color cursor_color){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_SCROLLBAR;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    set_rect(&(elt->rect), x, y, w, h);
    elt->sb_start = val_start;
    elt->sb_end = val_end;
    elt->sb_step = val_step;
    elt->sb_axis = sb_axis;
    elt->sb_bg_color = bg_color;
    elt->sb_curs_color = cursor_color;
    //
    elt->clickable = true;
    //
    return elt;
}

// Create a text input element
Ui_Element* ui_create_txt_input(Ui* ui, const char id[], int x, int y, int w, int h, int max_length, SDL_Color bg_color, SDL_Color txt_color, size_t ui_font_id, int font_size, char placeholder[]){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    //
    // Creating the element
    Ui_Element* elt = ui_init_empty_element(id);
    // Assign the good values
    elt->type = UI_ELT_TXT_INPUT;
    elt->x = x;
    elt->y = y;
    elt->w = w;
    elt->h = h;
    set_rect(&(elt->rect), x, y, w, h);
    elt->padding_top = 2;
    elt->padding_bottom = 2;
    elt->padding_left = 5;
    elt->padding_right = 5;
    //
    elt->color = bg_color;
    elt->color2 = txt_color;
    elt->font_id = ui_font_id;
    elt->baseFontsize = font_size;
    strcpy(elt->txt, placeholder);
    elt->max_input_length = max_length;
    elt->input_length = 0;
    elt->input_txt = (char*)calloc(sizeof(char), max_length+1);
    elt->input_txt_textures = (SDL_Texture**)calloc(sizeof(SDL_Texture*), max_length+1);
    elt->clickable = true;
    //
    return elt;
}

/////////////////////
/* -- UI EVENTS -- */
/////////////////////

//
void ui_add_event(
    Ui_Event* arr_events, size_t* arr_length, size_t* arr_capacity,
    int event_type, int event_key, int event_mbutton, bool stop_following_events, Ui_Element* elt_clicked,
    void (*function)(void* function_args), void* function_args
){
    if(arr_events == NULL || arr_length == NULL || arr_capacity == NULL){
        fprintf(stderr, "\e[31mError : passing NULL elements to ui_add_event function !!!\e[m\n");
        exit(42);
    }
    
    // Capacity
    if(*arr_length >= *arr_capacity){
        *arr_capacity *= 2;
        arr_events = (Ui_Event*)realloc(arr_events, sizeof(Ui_Event) * (*arr_capacity));
        if(arr_events == NULL){
            fprintf(stderr, "\e[31mError : Malloc crashed !!!\e[m\n");
            exit(42);
        }
        //
        for(size_t i=*arr_length; i <= *arr_capacity; i++){
            arr_events[i].event_type = UI_EVENT_NONE;
            arr_events[i].function = NULL;
            arr_events[i].func_args = NULL;
            arr_events[i].key_code = 0;
            arr_events[i].mouse_button = 0;
            //
            arr_events[i].stop_following_events = false;
        }
    }

    //
    arr_events[*arr_length].event_type = event_type;
    arr_events[*arr_length].key_code = event_key;
    arr_events[*arr_length].mouse_button = event_mbutton;
    arr_events[*arr_length].stop_following_events = stop_following_events;
    arr_events[*arr_length].elt_clicked = elt_clicked;
    arr_events[*arr_length].function = function;
    arr_events[*arr_length].func_args = function_args;
    (*arr_length)++;

}

//
void ui_add_on_elt_click_event(
    Ui_Element* elt, int event_mbutton, bool stop_following_events,
    void (*function)(void* function_args), void* function_args
){
    ui_add_event(
        elt->on_click_events, &(elt->length_on_click_events), &(elt->capacity_on_click_events),
        UI_EVENT_CLICK, 0, event_mbutton, stop_following_events, elt,
        function, function_args
    );
}

//
void ui_add_on_elt_keypress_event(
    Ui_Element* elt,
    int event_key, bool stop_following_events,
    void (*function)(void* function_args), void* function_args
){
    ui_add_event(
        elt->on_keypress_events, &(elt->length_on_keypress_events), &(elt->capacity_on_keypress_events),
        UI_EVENT_KEY, event_key, 0, stop_following_events, NULL,
        function, function_args
    );
}

//
void ui_add_global_keypress_event(
    Ui* ui,
    int event_key, bool stop_following_events,
    void (*function)(void* function_args), void* function_args
){
    ui_add_event(
        ui->windows[ui->current_window]->on_keypress_events, &(ui->windows[ui->current_window]->length_on_keypress_events), &(ui->windows[ui->current_window]->capacity_on_keypress_events),
        UI_EVENT_KEY, event_key, 0, stop_following_events, NULL, 
        function, function_args
    );
}

/////////////////////////////////////////////
/* -- UI EVENTS MAYBE USEFUL FUNCTIONS  -- */
/////////////////////////////////////////////

void change_current_windows(Arg_ui_and_int* arg){
    if(arg->number >= 0 && (size_t)arg->number < arg->ui->nb_windows){
        arg->ui->current_window = arg->number;
    }
    else{
        fprintf(stderr, "Error, can't change to a windows that doesn't exists!\n");
    }
}

void quit(Ui* ui){
    ui_free(ui);
    exit(0);
}

///////////////////////
/* -- UI MAINLOOP -- */
///////////////////////


// ui mainloop
void ui_mainloop(Ui* ui){

    // 
    for(size_t j=0; j<ui->nb_windows; j++){

        for(size_t i=0; i<ui->windows[j]->nb_elements; i++){
            if(ui->windows[j]->elements[i] == NULL) continue;
            Ui_Element* elt = ui->windows[j]->elements[i];
            detect_visible_children(elt, elt->x, elt->y, true);
        }
    }

    // UI mainloop
    SDL_Event event;
    ui->window_is_open = true;


    // The Mainloop
    while(ui->window_is_open){

        // Little stuff here, like updating the mouse position variable
        ui_pre_event(ui);

        // Get all the events from the event's buffer
        while(SDL_PollEvent(&event)){

            // Event of type QUIT : (ex: ALT+F4 or Button `close windows` pressed)
            if(event.type == SDL_QUIT){
                ui->window_is_open = false;
                break;
            }

            // ####################################
            // ###         MOUSE EVENTS         ###
            // ####################################

            // the user presse the button of his mouse
            if(event.type == SDL_MOUSEBUTTONDOWN){
                // Mouse position stored in : ui->mouse_position
                char id_elt_pressed[100] = "";
                ui_get_id_of_clicked_element(ui, id_elt_pressed);
                //
                Ui_Element* clicked_elt = NULL;
                if(strlen(id_elt_pressed) != 0)
                    clicked_elt = ui_get_element_rec(ui, id_elt_pressed);
                //
                if(clicked_elt != NULL){

                    // printf("\e[34mPressed element :\n  - id = \"%s\"\n  - type = %d\e[m\n", clicked_elt->id, clicked_elt->type);
                    
                    // Focus test
                    switch (clicked_elt->type)
                    {
                        case UI_ELT_SCROLLBAR:
                            ui->windows[ui->current_window]->grabbed_elt = clicked_elt;
                            break;
                        
                        case UI_ELT_BT_RECT:
                        case UI_ELT_BT_TEXTURE:
                            if(!(clicked_elt)->bt_toggle_mode){
                                clicked_elt->bt_pressed = true;
                            }
                            break;
                        
                        default:
                            break;
                    }
                }
            }

            // the user released the button of his mouse == Click
            if(event.type == SDL_MOUSEBUTTONUP){
                // Mouse position stored in : ui->mouse_position
                char id_elt_pressed[100] = "";
                ui_get_id_of_clicked_element(ui, id_elt_pressed);
                //
                Ui_Element* clicked_elt = NULL;
                if(strlen(id_elt_pressed) != 0)
                    clicked_elt = ui_get_element_rec(ui, id_elt_pressed);
                //
                ui->windows[ui->current_window]->grabbed_elt = NULL;
                //
                if(clicked_elt != NULL){

                    // printf("\e[34mClicked element :\n  - id = \"%s\"\n  - type = %d\e[m\n", clicked_elt->id, clicked_elt->type);

                    // Focus test
                    switch (clicked_elt->type)
                    {
                        case UI_ELT_TXT_INPUT:
                            ui->windows[ui->current_window]->focused_element = clicked_elt;
                            break;

                        case UI_ELT_BT_RECT:
                        case UI_ELT_BT_TEXTURE:
                            if(!(clicked_elt)->bt_toggle_mode){
                                clicked_elt->bt_pressed = false;
                            }
                            else{
                                clicked_elt->bt_pressed = !(clicked_elt->bt_pressed);
                            }
                            break;
                        
                        default:
                            break;
                    }

                    // printf("\e[34mClicked on element \"%s\" !\e[m\n", clicked_elt->id);
                    // printf("Clicked element has %ld event(s) on the click to execute!\n", clicked_elt->length_on_click_events);

                    // Clicked Element Click Events
                    for(size_t i=0; i<clicked_elt->length_on_click_events; i++){
                        Ui_Event evt = clicked_elt->on_click_events[i];
                        // If the mouse button is good
                        if(event.button.button == evt.mouse_button){
                            // We call the function of the event
                            evt.function(evt.func_args);
                            // If the event blocks the following events
                            if(evt.stop_following_events){
                                break;
                            }
                        }
                    }

                    if(ui->windows[ui->current_window]->focused_element != NULL && ui->windows[ui->current_window]->focused_element != clicked_elt){
                        ui->windows[ui->current_window]->focused_element = NULL;
                    }
   
                }
                else{
                    ui->windows[ui->current_window]->focused_element = NULL;
                }

                // Global Click Events
                for(size_t i=0; i<ui->windows[ui->current_window]->length_on_click_events; i++){
                    Ui_Event evt = ui->windows[ui->current_window]->on_click_events[i];
                    // If the good mouse button is clicked
                    if(event.button.button == evt.mouse_button){
                        // We call the function of the event
                        evt.function(evt.func_args);
                        // If the event blocks the following events
                        if(evt.stop_following_events){
                            break;
                        }
                    }
                }
            }

            // ####################################
            // ###       KEYBOARD EVENTS        ###
            // ####################################


            // The user released a Key of his keyboard
            if(event.type == SDL_KEYUP){
                int keycode = event.key.keysym.scancode;
                // printf("DEBUG KEY PRESSED : '%d'\n", keycode);
                if(keycode == KEY_ESCAPE){
                    if(ui->windows[ui->current_window]->focused_element != NULL){
                        ui->windows[ui->current_window]->focused_element = NULL;
                    }
                    break;
                }

                // Keypress events for the focused element
                if(ui->windows[ui->current_window]->focused_element != NULL){
                    if(ui->windows[ui->current_window]->focused_element->type == UI_ELT_TXT_INPUT){
                        if( detect_keyboard_txt_input(ui, ui->windows[ui->current_window]->focused_element, keycode) ) {
                            // printf("New txt input : \"%s\"\n", ui->focused_element->input_txt);
                        }
                    }
                    //
                    
                }

                // Keypress events
                for(size_t i=0; i<ui->windows[ui->current_window]->length_on_keypress_events; i++){
                    Ui_Event evt = ui->windows[ui->current_window]->on_click_events[i];
                    // If the event is clicked
                    if(keycode == evt.key_code){
                        // We call the function of the event
                        evt.function(evt.func_args);
                        // If the event blocks the following events
                        if(evt.stop_following_events){
                            break;
                        }
                    }
                }
            }
        }

        // Gestion of grabbed elements
        if(ui->windows[ui->current_window]->grabbed_elt != NULL){
            switch (ui->windows[ui->current_window]->grabbed_elt->type)
            {
                case UI_ELT_SCROLLBAR:
                    if(ui->windows[ui->current_window]->grabbed_elt->sb_axis == UI_SB_VERTICAL){
                        ui->windows[ui->current_window]->grabbed_elt->sb_value = (double)(clampui(ui->mouse_position.y - ui->windows[ui->current_window]->grabbed_elt->absolute_calculated_Y, 0, ui->windows[ui->current_window]->grabbed_elt->h)) /  ui->windows[ui->current_window]->grabbed_elt->h * (ui->windows[ui->current_window]->grabbed_elt->sb_end - ui->windows[ui->current_window]->grabbed_elt->sb_start);
                        if(ui->windows[ui->current_window]->grabbed_elt->sb_parent_container != NULL){
                            ui->windows[ui->current_window]->grabbed_elt->sb_parent_container->container_view_dec_Y = -ui->windows[ui->current_window]->grabbed_elt->sb_value;
                            detect_visible_children(ui->windows[ui->current_window]->grabbed_elt->sb_parent_container, ui->windows[ui->current_window]->grabbed_elt->sb_parent_container->absolute_calculated_X, ui->windows[ui->current_window]->grabbed_elt->sb_parent_container->absolute_calculated_Y, false);
                        }
                    }
                    else{
                        ui->windows[ui->current_window]->grabbed_elt->sb_value = (double)(clampui(ui->mouse_position.x - ui->windows[ui->current_window]->grabbed_elt->absolute_calculated_X, 0, ui->windows[ui->current_window]->grabbed_elt->w)) /  ui->windows[ui->current_window]->grabbed_elt->w * (ui->windows[ui->current_window]->grabbed_elt->sb_end - ui->windows[ui->current_window]->grabbed_elt->sb_start);
                        if(ui->windows[ui->current_window]->grabbed_elt->sb_parent_container != NULL){
                            ui->windows[ui->current_window]->grabbed_elt->sb_parent_container->container_view_dec_X = -ui->windows[ui->current_window]->grabbed_elt->sb_value;
                            detect_visible_children(ui->windows[ui->current_window]->grabbed_elt->sb_parent_container, ui->windows[ui->current_window]->grabbed_elt->sb_parent_container->absolute_calculated_X, ui->windows[ui->current_window]->grabbed_elt->sb_parent_container->absolute_calculated_Y, false);
                        }
                    }
                    break;
                
                default:
                    break;
            }
        }

        // Display the screen
        ui_display(ui);

        // Add a little delay to not waste too much CPU
        SDL_Delay(ui->windows[ui->current_window]->delay_at_mainloop_end);
    }
}

void decale_droite_txt_input(Ui_Element* txt_input, size_t idx_decale){
    for(size_t i=txt_input->input_length; i>=idx_decale; i--){
        txt_input->input_txt[i] = txt_input->input_txt[i-1];
        txt_input->input_txt_textures[i] = txt_input->input_txt_textures[i-1];
    }
    txt_input->input_txt[idx_decale] = '#';
    txt_input->input_txt_textures[idx_decale] = NULL;
}

void decale_gauche_txt_input(Ui_Element* txt_input, size_t idx_decale){
    for(size_t i=idx_decale; i<txt_input->input_length && i < txt_input->max_input_length-1; i++){
        txt_input->input_txt[i] = txt_input->input_txt[i+1];
        txt_input->input_txt_textures[i] = txt_input->input_txt_textures[i+1];
    }
}

void add_letter_to_txt_input(Ui* ui, Ui_Element* txt_input, char letter){
    if(ui == NULL){
        fprintf(stderr, "\e[31mUI Error : UI is NULL!\e[m\n");
        exit(42);
    }
    if(txt_input == NULL){
        fprintf(stderr, "\e[31mUI Error : txt_input is NULL!\e[m\n");
        exit(42);
    }
    //
    if(txt_input->input_length < txt_input->max_input_length){
        char txt[2]; txt[0] = letter; txt[1] = '\0';
        if(txt_input->input_cursor_position != txt_input->input_length){
            decale_droite_txt_input(txt_input, txt_input->input_cursor_position);
        }
        txt_input->input_txt[txt_input->input_cursor_position] = letter;
        txt_input->input_txt_textures[txt_input->input_cursor_position] = ui_texture_from_text(ui, txt, txt_input->color2, txt_input->font_id);
        txt_input->input_length++;
        txt_input->input_cursor_position++;
    }
}

// Return true if there were an interaction on the txt input
bool detect_keyboard_txt_input(Ui* ui, Ui_Element* txt_input, int keycode){
    //
    char letter_found = '\0';
    //
    switch (keycode)
    {
        case 42: // Backspace
            if(txt_input->input_cursor_position > 0){
                // Free the texture
                SDL_DestroyTexture(txt_input->input_txt_textures[txt_input->input_cursor_position-1]);
                //
                decale_gauche_txt_input(txt_input, txt_input->input_cursor_position-1);
                txt_input->input_cursor_position--;
                txt_input->input_length--;
            }
            return true;

        case 80: // Left arrow
            if(txt_input->input_cursor_position > 0){
                txt_input->input_cursor_position--;
            }
            return true;

        case 79: // Right arrow
            if(txt_input->input_cursor_position < txt_input->input_length){
                txt_input->input_cursor_position++;
            }
            return true;

        case 20: // a
            letter_found = 'a';
            break;

        case 26: // z
            letter_found = 'z';
            break;
            
        case 8: // e
            letter_found = 'e';
            break;
            
        case 21: // r
            letter_found = 'r';
            break;
            
        case 23: // t
            letter_found = 't';
            break;
        
        case 28: // y
            letter_found = 'y';
            break;
            
        case 24: // u
            letter_found = 'u';
            break;
            
        case 12: // i
            letter_found = 'i';
            break;
            
        case 18: // o
            letter_found = 'o';
            break;
            
        case 19: // p
            letter_found = 'p';
            break;

        case 4: // 
            letter_found = 'q';
            break;

        case 22: // 
            letter_found = 's';
            break;

        case 7: // 
            letter_found = 'd';
            break;

        case 9: // 
            letter_found = 'f';
            break;

        case 10: // 
            letter_found = 'g';
            break;

        case 11: // 
            letter_found = 'h';
            break;

        case 13: // 
            letter_found = 'j';
            break;

        case 14: // 
            letter_found = 'k';
            break;

        case 15: // 
            letter_found = 'l';
            break;

        case 51: // 
            letter_found = 'm';
            break;

        case 29: // 
            letter_found = 'w';
            break;

        case 27: // 
            letter_found = 'x';
            break;

        case 6: // 
            letter_found = 'c';
            break;

        case 25: // 
            letter_found = 'v';
            break;

        case 5: // 
            letter_found = 'b';
            break;

        case 17: // 
            letter_found = 'n';
            break;
        
        case 44: // space
            letter_found = ' ';
            break;
        
        default:
            printf("Keycode : %d\n", keycode);
            break;
    }
    //
    if(letter_found != '\0'){
        // printf("Key -> '%c'\n", letter_found);
        if(ui->windows[ui->current_window]->is_shift_pressed && letter_found >= 'a' && letter_found <= 'z'){
            letter_found += 'A' - 'a';
        }
        add_letter_to_txt_input(ui, txt_input, letter_found);
        return true;
    }
    //
    return false;
}





