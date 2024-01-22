# Typographie du code

## Généralites sur la nomination

**<span style="color: #ed1717">ATTENTION : les noms des constantes, variables, fonctions et structures seront en anglais.**

### <span style="color: #1748ed">Les variables

Les variables seront écrites en minuscules et avec des underscores.
Exemple :
```c
int variable;
int variable_en_plusieurs_parties;
```

Les constantes sont écrites toutes en majuscules et avec des underscores.
Exemple :
```c
#define CONSTANTE 42
```

### <span style="color: #1748ed">Les fonctions

Les fonctions sont nommées en minuscules et avec des underscores.
Exemple :
```c
void oskour_sa_fonctionne_pas(int variable)
{
    /...
}
``` 

### <span style="color: #1748ed">Les structures

Les noms des structures commencent par une majuscule puis sont constitués de minuscules.
Exemple :
```c
Tableau films_aime(variable)
{
    /...
}
```

## Construction des fichiers .c et .h

### Les constantes et les structs

Les constantes seront définies après les librairies et les headers. On sautera de ligne entre les constantes et les includes ou le corps du fichier.

Il faut utiliser les constantes dans la mesure du raisonnable et ne pas non plus en abuser.

Exemple :
```c

// Les includes avant ...

// Les constantes 

#define MA_SUPER_CONSTANTE 42
#define PATRICK "Sebastien"

// Les enums juste après

enum ErreursFonction {
    ERREUR_OK,
    ERREUR_PAS_BIEN_CA,
    ERREUR_LE_COMPILATEUR_NE_MARCHE_PAS,
    ERREUR_JE_SAIS_PAS_CODER
};

// Les structs juste après

typedef struct {
    unsigned int id_boitier;
    unsigned int id_carte_mere;
    unsigned int id_cpu;
    unsigned int id_carte_graphique;
    unsigned long int stockage;
    unsigned int memoire_ram;
} Ordinateur;

// Le reste du code après ...

```

### Les includes

Les inclusions de librairies statiques se trouveront au debut des fichiers juste après le `#pragma once`. **Le `#pragma once` doit être au tout début des fichiers headers, c'est cette ligne qui protège contre les inclusions multiples !**

Suivront ensuite les inclusions des headers. On séparera les librairies statiques des headers par un saut de ligne. De même, on sautera une ligne avant d'ecrire le corps du fichier.

Finalement, les constantes seront placées après les inclusions de librairies.

Exemple :
```c
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "args_parser.h"
#include "utils.h"

#define MA_CONSTANTE 42
```

### Le corps du fichier

Les fonctions devront être précédées d'un commentaire **en anglais**, expliquant ce qu'elles font.
Exemple :
```c
/*
test is used to know if the function f is returning the waited 
result. By returning 1 on a good run and 0 on a bad one.
*/
int test(char* f)
{
    /...
}
```

Les variables devront être précédées d'un commentaire en **anglais**, expliquant ce qu'elles font.
Exemple :
```c
/*
test is used to know if the function f is returning the waited 
result. By returning 1 on a good run and 0 on a bad one.
*/
int test(char* f)
{
    // i is an incremental for the loop while line ...
    int i=0;
    // sum stores the result of the loop while line ...
    int sum=0;
    /...
}

```

Les boucles devront être précédées d'un commentaire en **anglais**, expliquant ce qu'elles font.
Exemple :
```c
/*
test is used to know if the function f is returning the waited 
result. By returning 1 on a good run and 0 on a bad one.
*/
int test(char* f)
{
    // i is an incremental for the loop while line ...
    int i=0;
    // sum stores the result of the loop while line ...
    int sum=0;

    //This loop add 1 to sum 100 times before ending.
    while (i<100)
        sum+=1;
    /...
}
```

Enfin lors d'une étape peu claire ou lors de l'utilisation d'une fonction d'une librairie non standard, un commentaire en **anglais** devra être présent pour l'expliquer.
Exemple :
```c
/*
test is used to know if the function f is returning the waited 
result. By returning 1 on a good run and 0 on a bad one.
*/
int test(char* f)
{
    // i is an incremental for the loop while line ...
    int i=0;
    // sum will store the result of the loop while line ...
    int sum=0;
    // tab will store what the function unclear_function return.
    int* tab[10]={0};

    //This loop add 1 to sum 100 times before ending.
    while (i<100)
        sum+=1;
    //unclear_function is used to do something really unclear.
    tab[10]=unclear_function(sum);
    /...
}
```

### Ajouts pour la lisibilite du code

On mettra des espaces entre les operandes.
Exemple :
```c
int test(char* f)
{
    // i is an incremental for the loop while line ...
    int i = 0;
    // sum will store the result of the loop while line ...
    int sum = 0;
    // tab will store what the function unclear_function return.
    int* tab[10] = {0};

    //This loop add 1 to sum 100 times before ending.
    while (i < 100)
        sum += 1;
    
    //unclear_function is used to do something really unclear.
    tab[10] = unclear_function(sum);
    /...
}
```
