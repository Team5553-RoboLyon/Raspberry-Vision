# Vision-examples

Il y a 4 exemples:

- <a href="cscore/">acquire_and_stream_video</a> situé dans cscore/ pour <b>acquérir et diffuser la video</b> d'une caméra USB
    
- <a href="ntcore/">nt_client</a> situé dans ntcore/ pour <b>communiquer avec le roborio</b> par l'intermédiaire des Network Tables

- <a href="video_to_disk/">video_to_disk</a> pour <b>acquérir et enregistrer la video d'une caméra USB</b> sur le Raspberry

- <a href="hsl_threshold/">hsl_threshold</a> pour <b>acquérir et filtrer </b> la video d'une caméra USB

- <a href="full_processing/">full_processing</a> pour <b>acquérir, diffuser, enregistrer et traiter la video</b> d'une caméra USB ansi que pour <b>communiquer avec le roborio le resultat du traitement d'image</b>

<br />
<br />

Ces exemples sont fait pour être compilé en ligne de commande (plutôt que dans un IDE comme eclipse ou vscode).

- Pour initialiser le projet (seulement la 1ère fois), il faut simplement se placer dans le bon dossier et entrer:

    <code>cmake .</code>

- Pour les compiler, se placer dans le même dossier et entrer:

    <code>make</code>


- Pour les exécuter, toujours dans le même dossier:

    <code>./<nom_de_l'executable></code>
    
    exemple pour hsl_threshold : <code>./hsl_threshold.exe</code>

<br />
