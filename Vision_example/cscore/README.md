# Acquire and stream video

Cet exemple permet d'acquérir la vidéo issue d'une caméra USB branchée au Raspberry puis de la diffuser par http.

On peut facilement lire le flux video dans un navigateur web avec comme url : `http://IP_du_Raspberry:port`.

- Pour connaître l'IP du Raspberry, entrer `ifconfig` dans le terminal. Si on cherche à acquérir le flux sur le raspberry qui l'émet on peut utiliser `localhost` ou `127.0.0.1` comme IP

- Si un nom d'hôte a été défini on peut remplacer l'adresse IP par `<hostname>.local`

- Le port est celui spécifié dans le code, ici 8081.

Exemples: `http://192.168.0.46:8081/` ou `http://pi5553.local:8081` si le nom d'hôte est `pi5553`

Il utilise la librairie [CameraServer](https://wpilib.screenstepslive.com/s/currentCS/m/vision/l/682778-read-and-process-video-cameraserver-class) pour capturer et diffuser la vidéo.
