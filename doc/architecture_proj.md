# Architecture du Projet

## Sommaire

## Architecture

```
bin/                                    /// Dossier qui contient l'éxecutable du programme
    main                                // Executable du projet

data/                                   /// Contient les fichiers de données nécessaires pour le parser
    training_set/                       /// Contient toutes les données des ratings
        mv_0000001.txt                  // Ratings du film 1
           ...                          // ...
        mv_0017770.txt                  // Ratings du film 2
    movie_titles.txt                    // Contient les titres des films
    probe.txt                           // Fichier allant avec la base de donnée
    qualifying.txt                      // Fichier allant avec la base de donnée
    README                              // Le README du netflix prize

data_algo_2/                            /// Contient les données générées par l'algorithme random_indexing
    v10_vects_2.bin                     // Random Indexing avec dim=10 et coef=0.1
    v10_vects.bin                       // Random Indexing avec dim=10 et coef=1
    v100_vects_2.bin                    // Random Indexing avec dim=100 et coef=0.1
    v100_vects.bin                      // Random Indexing avec dim=100 et coef=1

doc/                                    /// Contient des fichiers de documentation
    architecture_proj.md            // Contient l'architecture du projet ainsi que la description de chaques dossiers/fichiers
    data.md                             // Explique comment sont formés les fichiers de données dans le dossier data/
    options.md                          // Explique comment utiliser l'option -r
    typo.md                             // Essais d'une imposition de typo

include/                                /// Contient les headers
    algo0.h                             // Algo aléatoire
    algo1.h                             // Algos partial_clustering et movie_from_movie
    algo2.h                             // Algo random_indexing
    an_data.h                           // Contient le nombre de rating pour chaque films et chaque utilisateurs
    args_parser.h                       // Structure args_t et fonction parse_args
    constants.h                         // Définit quelques constantes
    data.h                              // Fonction de chargement des connées
    lib_ui_sdl.h                        // Librairie graphique
    lib.h                               // Structures Date, MovieRating, MovieParser, Movie, User, ParserData, MainData
    main_algos.h                        // Structure Predictions, Fonction qu'on appelle pour lancer un algorithme
    opt.h                               // Inclusions pour opt
    parser.h                            // Pour lancer le Parser
    test_utils.h                        // Pour tester la fonction get_list_of_liked_movies
    ui_algos_page.h                     // Pour initialiser la page des algorithmes dans l'interface graphique
    ui_home_page.h                      // Pour initialiser la page d'acceuil dans l'interface graphique
    ui_main_ui.h                        // Pour lancer l'interface graphique
    ui_movies_page.h                    // Pour initialiser la page des films dans l'interface graphique
    utils.h                             // Pour avoir la fonction get_list_of_liked_movies

lst_films/                              /// Dossiers qui contient des fichiers qui contiennent des listes de films
    star_wars.txt                       // Liste de tous les Star Wars, même les trucs nuls

processed_bin_data/                     /// Dossier où seront enregistrées les données binaires du parser
    parser_data_movies_ratings.bin      // Données binaires des ratings, tableau de la structure MovieRatings
    parser_data_movies.bin              // Données binaires des films, tableau de la structure MovieParser

res/                                    /// Dossier qui contient des ressources (autre que du code et des fichiers binaires)
    ui/                                 /// Pour l'interface graphique
        bg/                             /// Pour l'arrière plan de l'écran d'accueil
            bg_app.png                  // Vieux arrière plan de l'écran d'accueil
            bg_app_v2.png               // Version actuelle de l'arrière plan de l'écran d'accueil
        
        fonts/                          /// Contient des fonts
            open_sans_medium.ttf        // Font OpenSans - Medium
            open_sans.ttf               // Font OpenSans
            roboto.ttf                  // Font Roboto
        
        textures/                       /// Contient des textures
            checked_box.png             // Bouton de checkbox activée
            unchecked_box.png           // Bouton de checkbox désactivée
            logo.png                    // Première version du logo
            logo2.png                   // Deuxième version du logo
            nextwatch_black.png         // Dernière version du logo
            nextwatch_transparent.png   // Dernière version du logoi - fond transparent

src/                                    /// Contient les scripts du projet
    algo0.c                             // Algo aléatoire
    algo1.c                             // Algos partial_clustering et movie_from_movie
    algo2.c                             // Algo random_indexing
    args_parser.c                       // Traitement des paramètres donnés en ligne de commandes
    data.c                              // Charge les données binaires
    lib_ui_sdl.c                        // Librairie graphique, surcouche de SDL
    lib.c                               // Contient une fonction pour initialiser la struct User 
    main_algos.c                        // Pour lancer les algorithmes
    main.c                              // Point de départ du programme
    parser.c                            // Convertit les fichiers textuels en fichiers binaires
    ui_algos_page.c                     // Page pour changer d'algorithme dans l'interface graphique
    ui_home_page.c                      // Page d'accueil de l'interface graphique
    ui_main_ui.c                        // Page pour initialiser l'interface graphique
    ui_movies_page.c                    // Page pour sélectionner des films et lancer les algorithmes, et voir les résultats des algorithmes
    utils.c                             // Contient la fonction get_list_of_liked_movies

tests/                                  /// Contient des tests qui nous servaient à tester certaines fonctions
    ...

.gitignore                              // Les fichiers à ignorer pour git
Makefile                                // Pour compiler le projet
Rapport_Projet_Programmation.pdf        // Le rapport de ce projet, en pdf
README.md                               // Le rapport de ce projet, en markdown
```
