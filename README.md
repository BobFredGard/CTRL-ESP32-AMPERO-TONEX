# CTRL-ESP32-AMPERO-TONEX

J'ai voulu réaliser un outil pour piloter à la fois mon HOTONE AMPERO II STOMP et mon IK-MULTIMEDIA TONEX
Cet outil peut-être adapté à tous types d'instruments MIDI après quelques modification du code sourcen bien évidement!
On peut y retrouver dans DATA la base de donnée et la modifier à souhait aussi...*

J'ai voulu une interface simple, pour commencer : 
    1 écran LCD LiquidChrystal_I²C
    et 2 boutons rotatifs incrémentaux

Le bouton de gauche donne accès rapide à 5 paramètres au choix (voir Amperoquick)!
Le bouton de gauche donne accès rapide à 5 paramètres au choix (voir toneXquick)!
Quand on reste appuyé plus de 2s sur le bouton 1, on entre dans l'édition totale des paramètres, en commençant par ceux de l'AMPERO, et idem en restant appuyé sur le bouton 2, on entre dans cette même fenêtre, en commençant par les paramètre du TONEX!
