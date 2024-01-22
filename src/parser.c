
#include "parser.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//
int parse_data_movie_line(char* line, int* id_movie, int* year, char* title){
    int id_comma1 = 0;
    int id_comma2 = 0;
    int n = strlen(line);
    // Searching first ','
    while(id_comma1 < n && line[id_comma1] != ','){
        id_comma1++;
    }
    if(line[id_comma1] != ',') return 0;
    // Searching second ','
    id_comma2 = id_comma1+1;
    while(id_comma2 < n && line[id_comma2] != ','){
        id_comma2++;
    }
    if(line[id_comma2] != ',') return 0;
    // Parsing
    //  - Id Movie
    char str_id_movie[100] = "";
    strncpy(str_id_movie, line, id_comma1);
    *id_movie = atoi(str_id_movie);
    //  - Year
    char str_year[100] = "";
    strncpy(str_year, line+id_comma1+1, id_comma2-id_comma1-1);
    *year = atoi(str_year);
    //  - Title
    strcpy(title, line+id_comma2+1);
    //
    return 1;
}

//
void load_data_movies(ParserData* parser_data){
    // We open the movies data file
    FILE* f = fopen("data/movie_titles.txt", "r");
    if(f == NULL){
        fprintf(stderr, "LE FICHIER EST NULL!!!!!!!!!\n");
        exit(42); }
    //
    int id_movie=1;
    int year=0;
    char title[700] = "";
    char line[1000] = "";
    for(int i = 1; i<=NUMBER_OF_MOVIES; i++){
        // Get all the line in the file to the variable `line`
        int debug = fscanf(f, "%[^\n]", line);
        // To escape the `\n` character, because if we don't have this line, it doesn't work :(
        fgetc(f);
        //
        debug = parse_data_movie_line(line, &id_movie, &year, title);
        if(debug == 0){
            fprintf(stderr, "ERROR\n");
            exit(42);
        }
        //
        if(id_movie != i){
            fprintf(stderr, "Error Parser Data %d != %d, year=%d, title=%s !\n", i, id_movie, year, title);
            exit(42);
        }
        // Adding the movie to parser_data
        strcpy(parser_data->movies[id_movie-1].name, title);
        parser_data->movies[id_movie-1].year = year;
        parser_data->movies[id_movie-1].id = id_movie;
    }
    // We close the file
    fclose(f);
}

// Le nom de la fonction parle de lui même
void add_zeros(int id_movie, char* fid){
    fid[7] = '\0';
    fid[8] = '\0'; // Au cas où je me serais trompé

    if(id_movie < 10){
        sprintf(fid, "000000%d", id_movie);
        return; }
    if(id_movie < 100){
        sprintf(fid, "00000%d", id_movie);
        return; }
    if(id_movie < 1000){
        sprintf(fid, "0000%d", id_movie);
        return; }
    if(id_movie < 10000){
        sprintf(fid, "000%d", id_movie);
        return; }
    if(id_movie < 100000){
        sprintf(fid, "00%d", id_movie);
        return; }
    if(id_movie < 1000000){
        sprintf(fid, "0%d", id_movie);
        return; }
    if(id_movie < 10000000){
        sprintf(fid, "%d", id_movie);
        return; }

    // On ne devrait pas arriver là, mais au cas où
    fprintf(stderr, "ERREUR !\n");
    exit(42);
}

//
void load_data_ratings(ParserData* parser_data){
    //
    size_t nb_ratings = 0;
    // We open the movies data file
    char path[50];
    for(int id_movie = 1; id_movie <= NUMBER_OF_MOVIES; id_movie++){
        printf("Parsing movie : %d\n", id_movie);
        //
        char fid[10];
        add_zeros(id_movie, fid);
        sprintf(path, "data/training_set/mv_%s.txt", fid);

        // We open the file
        FILE* f = fopen(path, "r");
        
        if(f == NULL){
            fprintf(stderr, "LE FICHIER '%s' EST NULL!!!!!!!!!\n", path);
            exit(42); }

        // 
        int id_user=-1;
        int rating=-1;
        int year=-1;
        int month=-1;
        int day=-1;

        //
        int test_id;
        int debug = fscanf(f, "%d:", &test_id);
        if(debug == 0){}
        if(test_id != id_movie){
            fprintf(stderr, "ERROR : BAD DATA !\n");
            exit(42);
        }
        
        char c = fgetc(f);
        
        while( c != EOF ){
            // Getting the current line
            debug = fscanf(f, "%d,%d,%d-%d-%d", &id_user, &rating, &year, &month, &day);
            if(id_user <= 0 || id_user >= 2649430){
                fprintf(stderr, "ERROR : Bad user id : %d\n", id_user);
                exit(42);
            }
            c = fgetc(f);
            if(c == EOF) break;
            if(debug < 5){
                fprintf(stderr, "ERROR\n");
                exit(42); }
            // We append the rating
            parser_data->ratings[nb_ratings].date.day = day;
            parser_data->ratings[nb_ratings].date.month = month;
            parser_data->ratings[nb_ratings].date.year = year;
            parser_data->ratings[nb_ratings].rating = rating;
            parser_data->ratings[nb_ratings].id_movie = id_movie;
            parser_data->ratings[nb_ratings].id_user = id_user;
            //
            nb_ratings++;
        }
        
        // We close the file
        fclose(f);
    }
    printf("\n!###!\n\nNombre de ratings : %ld\n\n!###!\n", nb_ratings);
}

//
ParserData* load_parser_data(){
    //
    ParserData* parser_data = (ParserData*)calloc(sizeof(ParserData), 1);
    //
    load_data_movies(parser_data);
    load_data_ratings(parser_data);
    //
    return parser_data;
}

//
void save_parser_data(ParserData* parser_data){
    printf("SAVING RATINGS\n");
    FILE* f = fopen("processed_bin_data/parser_data_movies_ratings.bin", "wb+");
    int nb_a_sauvegarder = 10000;
    int nb_total = NUMBER_OF_RATINGS;
    int crt = 0;
    while(crt < nb_total){
        int nb_crt;
        if(crt+nb_a_sauvegarder <= nb_total){
            nb_crt = nb_a_sauvegarder;
        }
        else{
            nb_crt = nb_total - crt;
        }
        printf("Writing %d bytes %d/%d\n", nb_crt, crt, nb_total);
        fwrite((parser_data->ratings)+crt, sizeof(MovieRating), nb_a_sauvegarder, f);
        crt += nb_crt;
    }
    //
    fclose(f);
    //
    printf("SAVING MOVIES\n");
    f = fopen("processed_bin_data/parser_data_movies.bin", "wb");
    fwrite(parser_data->movies, sizeof(MovieParser), NUMBER_OF_MOVIES, f);
    fclose(f);
    //
}

//
void free_parser_data(ParserData* parser_data){
    free(parser_data);
}

void print_movie(ParserData* parser_data, int id_movie){
    if(id_movie <= 0 || id_movie >= NUMBER_OF_MOVIES){
        printf("This movie doesn't exist.\n");
    }
    //
    printf("---- MOVIE ----\n");

    printf(" id : %d\n", parser_data->movies[id_movie].id);
    printf(" name : %s\n", parser_data->movies[id_movie].name);
    printf(" year : %d\n", parser_data->movies[id_movie].year);

    printf("---- ##### ----\n\n");
}

//
void naviguate_parser(ParserData* parser_data){
    printf("ParserData Explorer :\n");
    printf("\nSelect one in [movie] or enter 'exit'\n>>> ");
    char input[100] = "";
    int id;
    int debug = 0;
    debug = scanf("%s", input);
    while((strcmp(input, "exit")!=0) && (strcmp(input, "q")!=0)){
        
        if(strcmp(input, "movie")==0){
            // Movie
            printf("\nPlease enter the id of the movie you want to analyse\n: ");
            debug = scanf("%d", &id);
            //
            print_movie(parser_data, id);
            //
        }else{
            
            {
                printf("Error : This option is not recognized.\n");
            }
        }
        //
        printf("\nSelect one [movie, user] or enter 'exit'\n>>> ");
        debug = scanf("%s", input);
    }
    if(debug==0){ }
}

// Entry Point of the parser
void main_parser(){

    // Load all the data
    ParserData* parser_data = load_parser_data();

    if(false){
        // Naviguate on the data
        naviguate_parser(parser_data);
    }

    // Save all the data inside binaries files
    save_parser_data(parser_data);

    // Freeing 
    free_parser_data(parser_data);
}
