# CTRL VIA ESP32 & PACER AMPERO II STOMP & TONEX

J'ai voulu réaliser un outil pour piloter à la fois mon HOTONE AMPERO II STOMP et mon IK-MULTIMEDIA TONEX à partir de mon NEKTAR PACER ou tout autre pédalier MIDI.
Cet outil peut-être adapté à tous types d'instruments MIDI après quelques modification du code source bien évidement!
On peut y retrouver dans DATA la base de donnée et la modifier à souhait aussi...*

J'ai voulu une interface simple, pour commencer : 
    1 écran LCD LiquidChrystal_I²C
    et 2 boutons rotatifs incrémentaux
    et 2 prises MIDI

Le bouton de gauche donne accès rapide à 5 paramètres au choix (voir Amperoquick)!
Le bouton de gauche donne accès rapide à 5 paramètres au choix (voir toneXquick)!
Quand on reste appuyé plus de 2s sur le bouton 1, on entre dans l'édition totale des paramètres, en commençant par ceux de l'AMPERO, et idem en restant appuyé sur le bouton 2, on entre dans cette même fenêtre, en commençant par les paramètre du TONEX!

En ce qui concerne le PACER, il envoit des 'ProgramChange' et des 'ControlChange' sur des canaux différents. Je me sers de ces valeurs envoyées pour déclencher mes propres évènements MIDI à destination de mes 2 appareils!

2 'ProgramChange' sont envoyés aux 2 appareils si besoin!!!

Je donne un fichier Zip d'un circuit imprimé possible! Le code ne correspond pas exactement à ce circuit, mais il vous sera facile de l'adapter!!!
