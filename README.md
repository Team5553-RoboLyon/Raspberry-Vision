# Raspberry-Vision
Ce repository a pour but d'aider à mettre en place une reconnaissance visuelle sur Raspberry Pi pour la FRC.
Il contient un script bash pour préparer le raspberry en, par exemple, installant les librairies
<a href="https://wpilib.screenstepslive.com/s/currentCS/m/75361/l/843361-what-is-networktables">ntcore</a>,
<a href="https://wpilib.screenstepslive.com/s/currentCS/m/vision/l/682778-read-and-process-video-cameraserver-class">cscore</a>
, opencv, ect... Il contient aussi des exemples complets de reconnaissance visuelle.

Le script et les exemples peuvent être executés par n'importe quel Rasberry tournant sous Rasbian mais il est cependant préférable d'avoir une installation "fraîche" de Rasbian Stretch (très facile à installer avec <a href="https://www.raspberrypi.org/documentation/installation/noobs.md"> NOOBS</a>).


Pour configurer le Raspberry il faudra donc suivre ces étapes:


- Installer git pour pouvoir cloner le repository:

    <code>sudo apt-get update</code>

    <code>sudo apt-get install git</code>


- Cloner le repository dans /home/pi:

    <code>git clone https://github.com/Team5553-RoboLyon/Raspberry-Vision</code>
    
    
- Ouvrir le script "Raspberry_init.bash" avec n'importe quel éditeur de texte et remplacer la ligne 36 par:

    <code>OPENCV_VER="<Le dernier numéro de version stable d'opencv>"</code>
    
    
- Se placer dans le repository:

    <code>cd /home/pi/Raspberry-Vision/</code>
    
    
- Autoriser l'execution du script:

    <code>chmod u+x Raspberry_init.bash</code>
    
    
- Executer le script:

    <code>./Raspberry_init.bash</code>
    
Le script prend pas mal de temps avant de se terminer.

Après cela, il sera possible de faire de la reconnaissance visuelle. Pour compiler, executer et personaliser les exemples, lire leur <a href="Vision_example/README.md">readme</a>.
