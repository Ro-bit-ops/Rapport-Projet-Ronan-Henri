Remarque importante concernant l’esp32 :

Attention au choix des broches ADC.
Si l’on prend par exemple la GPIO 25, elle est de type ADC2
L’ ADC2 est bloqué quand le wifi est actif. On se retrouve donc bloqué dans notre contexte.
En utilisant GPIO 33 cela marche car elle est de type ADC1. Les ADC1 vont de GPIO 32 à 39.
Si l’on veut faire passer des informations via wi-fi ou lora on optera donc pour les ADC1.



Node red :

Notre node red est divisé en 3 parties. La première comporte les éléments qui fonctionne en direct comme la jauge et le graphique. Elle comporte aussi le traitement de la base de données.
La deuxième partie correspond à l’exportation des données dans un tableau retransmettant la base de données en direct sur l’interface node red lorsque l’on appuie sur INJECT. Elle possède aussi une branche qui envoie directement la base de données dans un fichier sur la raspberry.
Et la troisième correspond au slider qui permet de modifier, via l’interface node red, la valeur seuil de température à laquelle une led s’allume et un message discord est envoyé afin de prévenir de ce dépassement.



Sécurité et confidentialité :

En ce qui concerne la sécurité, nous avons configuré le mosquitto en « allow anonymous -> false ».
Pour accéder à notre mosquitto on doit donc saisir un identifiant et un mot de passe.



Choix du réseau et consommation :

Nous avons choisi d’opter pour LoraChoco. En effet, ce réseau est moins énergivore et plus adapté à ce genre de projet.


CONCLUSION DU PROJET :

Lire la température avec le capteur LM35 sur l’ESP32 --> CHECK

Transmettre les données au Raspberry Pi via MQTT --> CHECK

Minimiser la consommation électrique du module --> CHECK

Enregistrer les données dans une base SQLite sur le Raspberry Pi --> CHECK

Assurer une récupération efficace des données pour leur exploitation --> CHECK

Utiliser Node-RED pour afficher les données en temps réel --> CHECK

Ajouter un graphique interactif pour visualiser l’évolution de la température --> CHECK

Implémenter une authentification MQTT --> CHECK

Mettre en place un système d’alerte (email, discord, buzzer) si la température dépasse un seuil (modifiable par MQTT) --> CHECK

Intégrer une LED témoin sur l’ESP32 pour signaler un problème (couleur rouge) --> CHECK

Rédiger une documentation complète sur l’installation, la configuration et le fonctionnement --> CHECK

Expliquer l’architecture du projet avec un schéma clair --> CHECK

Fournir un guide de dépannage pour les problèmes courants --> CHECK
