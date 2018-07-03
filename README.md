# Raspberry-Vision
Ce repository a pour but d'aider à mettre en place une reconnaissance visuelle sur Raspberry Pi pour la First Robotics Competition.
Il contient un script bash pour configurer le raspberry et installer les librairies
<a href="https://wpilib.screenstepslive.com/s/currentCS/m/75361/l/843361-what-is-networktables">ntcore</a>,
<a href="https://wpilib.screenstepslive.com/s/currentCS/m/vision/l/682778-read-and-process-video-cameraserver-class">cscore</a>
, opencv, ect...

Pour configurer le Raspberry il faudra donc suivre ces étapes:


- Installer git pour pouvoir cloner le repository:

    <code>sudo apt-get update</code>

    <code>sudo apt-get install git</code>


- Cloner le repository dans /home/pi:

    <code>git clone https://github.com/Team5553-RoboLyon/Raspberry-Vision</code>
    
    
- Autoriser l'execution du script:

    <code>chmod u+x Raspberry-Vision/Raspberry_init.bash</code>
    
    
- Executer le script:

    <code>./Raspberry-Vision/Raspberry_init.bash</code>
    
Le script prend quelques heures avant de se terminer et il faut parfois intervenir pour accepter l'installation de programmes.

Après cela, il sera possible de faire de la reconnaissance visuelle. Pour compiler et executer l'exemple lire le <a href="Vision_example/README.md">readme</a> de celui ci.
