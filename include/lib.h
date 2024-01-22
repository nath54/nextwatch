#pragma once

#include "constants.h"

typedef struct {
    unsigned char day;
    unsigned char month;
    short year;
} Date;

typedef struct {
    Date date;
    int id_user;
    short id_movie;
    unsigned char rating; // 1-5 rating
} __attribute__((packed))  MovieRating ;

typedef struct {
    char name[250];
    int id;
    int year;
} MovieParser;


typedef struct {
    char name[250];
    int id;
    int year;
    float avg_rating;
    unsigned int* ratings;
    unsigned int nb_ratings;
} Movie;

typedef struct {
    int id;
    unsigned int* ratings;
    unsigned int nb_ratings;
} User;

typedef struct{
    MovieParser movies[NUMBER_OF_MOVIES];
    MovieRating ratings[NUMBER_OF_RATINGS];
} ParserData;

// **THE** struct we will use to manipulate data for predictions algorithms
typedef struct{
    Movie movies[NUMBER_OF_MOVIES]; // 0 -> 17769
    User users[2649430]; // 480189 / 2649429, NULL users[id_user] is designed by an `id` equals to 0
    MovieRating ratings[NUMBER_OF_RATINGS];
} MainData;

// Create a User variable
User* user_create(int id_user);
