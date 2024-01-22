
# Données

## Architecture des données

Le dossier `data/` doit contenir :

```
movie_titles.txt
probe.txt
qualifying.txt

training_set/
    mv_0000001.txt
    mv_0000002.txt
    mv_0000003.txt
    ... 
    mv_0017770.txt
```

## Description des fichiers de données

### <span style="color: #1748ed">Que contient les fichiers `probe.txt` et `qualifying.txt` ?

Ils contiennent des données d'évaluation, on ne va pas y toucher pour le moment.

### <span style="color: #1748ed">Que contient le fichier `movie_titles.txt` ?

Movie information in "movie_titles.txt" is in the following format:

MovieID,YearOfRelease,Title

- MovieID do not correspond to actual Netflix movie ids or IMDB movie ids.
- YearOfRelease can range from 1890 to 2005 and may correspond to the release of
  corresponding DVD, not necessarily its theaterical release.
- Title is the Netflix movie title and may not correspond to 
  titles used on other sites.  Titles are in English.

### <span style="color: #1748ed">Que contient les fichiers `training_set/mv_00ID_FILM.txt` ?

The file "training_set.tar" is a tar of a directory containing 17770 files, one
per movie.  The first line of each file contains the movie id followed by a
colon.  Each subsequent line in the file corresponds to a rating from a customer
and its date in the following format:

CustomerID,Rating,Date

- MovieIDs range from 1 to 17770 sequentially.
- CustomerIDs range from 1 to 2649429, with gaps. There are 480189 users.
- Ratings are on a five star (integral) scale from 1 to 5.
- Dates have the format YYYY-MM-DD.
