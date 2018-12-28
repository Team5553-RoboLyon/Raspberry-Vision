# Vision-examples

## Description des principaux exemples

- [acquire_and_stream_video](cscore/) situé dans cscore/ pour **acquérir et diffuser la video** d'une caméra USB
    
- [nt_client](ntcore/) situé dans ntcore/ pour **communiquer avec le roborio** par l'intermédiaire des Network Tables

- [video_to_disk](video_to_disk/) pour **acquérir et enregistrer la video d'une caméra USB** sur le Raspberry

- [hsl_threshold](hsl_threshold/) pour **acquérir et filtrer** la video d'une caméra USB

- [full_processing](full_processing/) pour **acquérir, diffuser, enregistrer et traiter la video** d'une caméra USB ansi que pour **communiquer avec le roborio le resultat du traitement d'image**


## Compiler et éxécuter les exemples

Ces exemples sont fait pour être compilé en ligne de commande (plutôt que dans un IDE comme eclipse ou vscode).

- Pour initialiser le projet (seulement la 1ère fois), il faut simplement se placer dans le bon dossier et entrer:

    `cmake .`

- Pour les compiler, se placer dans le même dossier et entrer:

    `make`


- Pour les exécuter, toujours dans le même dossier:

    `./<nom_de_l'executable>`
    
    Exemple pour hsl_threshold : `./hsl_threshold.exe`


## Exécuter un programme au démarrage

Voir la [section](../README.md#exécuter-un-programme-au-démarrage) correspondante dans le README du repository 