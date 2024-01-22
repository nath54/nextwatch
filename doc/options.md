# Comment utiliser les options

## But du document

Ce markdown a pour but de permettre a tout novice de pouvoir utiliser les options disponibles pour ce programme.

## L'option `-r`

### <span style="color: #1748ed">A quoi sert `-r`

Lorsque vous rajoutez `-r` a votre ligne de commande en appelant l'excutable de ce projet vous pouvez preciser a la suite les identifiants des films que vous aimez.

### <span style="color: #1748ed">Comment utiliser `-r` 

Les identifiants des films doivent etre sous la forme d'une chaine de caracteres et etre separes par des virgules, sans espace.
Exemple d'appel : 
```c
./my_awesome_project -r "15,20,35,61,24"
``` 
Sinon la chaine de caractere peut contenir le chemin vers un fichier `.txt`.
Exemple d'appel :
```c
./my_awesome_project -r "../tests/data_tests/Mes_films_que_jaime.txt"
``` 
Celui-ci contiendrait un identifant de film par ligne comme dans [cet exemple](../tests/data_tests/Jaime.txt).


### <span style="color: #1748ed">Comment utiliser `-m` 

Lorsque vous rajoutez `-m` a votre ligne de commande en appelant l'excutable de ce projet vous pouvez rajoutez `window` qui vous permettra d'afficher l'interface graphique.

Exemple d'appel : 
```c
./my_awesome_project -m window
``` 