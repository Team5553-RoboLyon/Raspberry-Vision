# Acquire and stream video

Cet exemple permet d'acquérir la vidéo issue d'une caméra USB branchée au Raspberry puis de la diffuser par http.

On peut facilement lire le flux video dans un navigateur web avec comme url : <code>http://IP_du_Raspberry:port</code>. Pour connaître l'IP du Raspberry, entrer <code>ifconfig</code> dans le terminal. Le port est celui spécifié dans le code, ici 8081.

Exemple: <code>http://192.168.0.46:8081/</code>

Il utilise la librairie <a href="https://wpilib.screenstepslive.com/s/currentCS/m/vision/l/682778-read-and-process-video-cameraserver-class">CameraServer</a> pour capturer et diffuser la vidéo.
