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

    <code>cmake .</code>

- Pour les compiler, se placer dans le même dossier et entrer:

    <code>make</code>


- Pour les exécuter, toujours dans le même dossier:

    <code>./<nom_de_l'executable></code>
    
    Exemple pour hsl_threshold : <code>./hsl_threshold.exe</code>


## Exécuter les exemples au démarage

Pour exécuter un des programme au démarage du Raspberry :

- Editer le fichier /etc/rc.local avec les droits root :

    <code>sudo nano /etc/rc.local</code>

- Ajouter la commande pour exécuter le programme en laissant la ligne <code>exit 0</code> à la fin.
    
    Exemple du fichier /etc/rc.local pour full_processing :
    ```sh
    #!/bin/sh -e
    
    #... Script déjà existant que l'on laisse (sauf exit 0) ...
    
    #Si le fichier existe alors on l'exécute
    if [ -e /home/pi/Raspberry-Vision/Vision_example/full_processing/full_processing.exe ]
    then
            /home/pi/Raspberry-Vision/Vision_example/full_processing/full_processing.exe &
    fi

    exit 0
    ```

:warning: **Attention** : les programmes avec une interface graphique ne peuvent pas être exécutés au démarage avec cette méthode
