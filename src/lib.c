#include "lib.h"

#include <stdio.h>
#include <stdlib.h>

// Create a User Variable
User* user_create(int id_user){
    User* user = (User*)calloc(sizeof(User), 1);
    user->id = id_user;
    // user->films_rated = array_create(sizeof(MovieRating*), false);
    return user;
}

