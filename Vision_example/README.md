# Vision-examples

Il y a 4 exemples:

- <a href="cscore/">acquire_and_stream_video</a> situé dans cscore/ pour <b>acquérir et diffuser la video</b> d'une caméra USB
    
- <a href="ntcore/">nt_client</a> situé dans ntcore/ pour <b>communiquer avec le roborio</b> par l'intermédiaire des Network Tables

- <a href="video_to_disk/">video_to_disk</a> pour <b>acquérir et enregistrer la video d'une caméra USB</b> sur le Raspberry

- <a href="full_processing/">full_processing</a> pour <b>acquérir, diffuser, enregistrer et traiter la video</b> d'une caméra USB ansi que pour <b>communiquer avec le roborio le resultat du traitement d'image</b>

<br />
<br />

Ces exemples sont fait pour être compilé en ligne de commande (plutôt que dans un IDE comme eclipse).

- Pour les compiler, il faut simplement se placer dans le bon dossier et entrer:

    <code>make</code>


- Pour les exécuter, se placer dans le même dossier et entrer:

    <code>make run</code>

<br />

Il est possible d'exécuter ces commande directement dans ce dossier (Vision-examples), cela compilera/exécutera alors les programmes <a href="ntcore/">nt_client</a> et <a href="cscore/">acquire_and_stream_video</a> l'un après l'autre.
