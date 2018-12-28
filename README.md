# Raspberry-Vision
Ce repository a pour but d'aider à mettre en place une reconnaissance visuelle sur Raspberry Pi pour la FRC.
Il contient un script bash pour installer les librairies
[ntcore](https://wpilib.screenstepslive.com/s/currentCS/m/75361/l/843361-what-is-networktables),
[cscore](https://wpilib.screenstepslive.com/s/currentCS/m/vision/l/682778-read-and-process-video-cameraserver-class)
et opencv nécéssaires pour la reconnaissance visuelle. Il contient aussi des exemples complets de reconnaissance visuelle.

Le script et les exemples peuvent être executés par n'importe quel Rasberry tournant sous Rasbian mais il est cependant préférable d'avoir une installation "fraîche" de Rasbian Stretch (très facile à installer avec [NOOBS](https://www.raspberrypi.org/documentation/installation/noobs.md)).

## Configuration du Raspberry
Pour configurer le Raspberry il faudra donc suivre ces étapes:


- Installer git pour pouvoir cloner le repository:

    `sudo apt-get update`

    `sudo apt-get install git`


- Cloner le repository dans /home/pi:

    `git clone https://github.com/Team5553-RoboLyon/Raspberry-Vision`
    
    
- Ouvrir le script "Raspberry_init.bash" avec n'importe quel éditeur de texte et remplacer la ligne 29 par:

    `OPENCV_VER="<Le dernier numéro de version stable d'opencv>"`


- Executer le script:

    `~/Raspberry-Vision/Raspberry_init.bash`
    
Le script prend pas mal de temps avant de se terminer.

## Exemples
Après cela, il sera possible de faire de la reconnaissance visuelle. Pour compiler et executer les exemples, lire leur [README](Vision_example/README.md).

## Configuration pour la compétition

### Exécuter un programme au démarrage

Pour exécuter un des programme au démarrage du Raspberry :

- Editer le fichier /etc/rc.local avec les droits root :

    `sudo nano /etc/rc.local`

- Ajouter la commande pour exécuter le programme en laissant la ligne `exit 0` à la fin.
    
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

- `ctrl + o` puis `Entrer` pour sauvegarder, `ctrl + x` pour fermer

:warning: **Attention** : les programmes avec une interface graphique ne peuvent pas être exécutés au démarrage avec cette méthode

### Desactiver le wifi et bluetooth

- Editer le fichier /boot/config.txt : `sudo nano /boot/config.txt`

- Ajouter ces lignes à la fin du fichier :
    ```
    # Uncomment this to disbale Wifi
    dtoverlay=pi3-disable-wifi

    # Uncomment this to disable Bluetooth
    dtoverlay=pi3-disable-bt
    ```
- `ctrl + o` puis `Entrer` pour sauvegarder, `ctrl + x` pour fermer

### Desactiver l'interface graphique

- Entrer dans la console : `sudo raspi-config`

- Aller à `Boot Options` puis à `Desktop / CLI`

- Sélectionner l'option `Console Autologin`

- Aller à `Finish` puis redémarrer le Raspberry

Pour lancer l'interface graphique après avoir démarré en mode console, entrer `startx`. 
