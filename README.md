# CTRL VIA ESP32 & PACER AMPERO II STOMP & TONEX
# Version 1.1.1 (Réduction mémoire utilisée de 3%)
*** Cette version est ma version prod. J'ai enlevé la partie server car l'antenne wifi générait des interférences avec mes pédalier et pédale!!!

*** Sur cette version, la scène 6 a été retirée au profit de l'activation du stomp de mon choix uniquement!!!

J'ai voulu réaliser un outil pour piloter à la fois mon HOTONE AMPERO II STOMP et mon IK-MULTIMEDIA TONEX à partir de mon NEKTAR PACER ou tout autre pédalier MIDI.
Cet outil peut-être adapté à tous types d'instruments MIDI après quelques modifications du code source bien évidement!
On peut y retrouver dans DATA la base de donnée et la modifier à souhait aussi...*

J'ai voulu une interface simple, pour commencer : 
    1 écran LCD LiquidChrystal_I²C
    et 2 boutons rotatifs incrémentaux
    et 2 prises MIDI

Le bouton de gauche donne un accès rapide à 5 paramètres au choix (voir Amperoquick)!

Le bouton de gauche donne un accès rapide à 5 paramètres au choix (voir toneXquick)!

Quand on reste appuyé plus de 2s sur le bouton 1, on entre dans l'édition totale des paramètres, en commençant par ceux de l'AMPERO, et idem en restant appuyé sur le bouton 2, on entre dans cette même fenêtre, en commençant par les paramètres du TONEX!

Les Patchs sont pensés comme suit : les 6 pédales du PACER de 1 à 6 correspondent à des Patchs de 7 scènes, dont la 1ère est accessible par la pédale 1. Les autres le sont par les pédales de A à D et les FS 1-3. J'ai utilisé 6 banques de 6 Patchs pour un total de 36 Patchs!!! On peut dire qu'il y a 6 patchs accessibles par les pédales 1 à 6 du PACER et 6 scènes par patchs!

J'ai ajouté une fonction CopyPatch qui permet de copier chaque Patch de la banque en cours vers le Patch de son choix, quelque soit la banque en cours! Pour cela, double clic sur le bouton droit. On choisit le patch a copier avec le rotatif 1 et la destination avec le rotatif 2! La destination montre la banque puis le numéro de pédale à choisir!!!

J'ai aussi ajouté la fonction Copy Scene qui permet de copier n'importe quelle scène des 42 disponibles par banque vers n'importe quelle banque/patch/scène!!! Pour cela, double clic sur le bouton de gauche ou bouton 1, puis ce même bouton pertmet de choisir la scène désirée, par défaut celle active, puis de choisir sa destination avec le bonton droit.

En ce qui concerne le PACER, il envoit des 'ProgramChange' et des 'ControlChange' sur des canaux différents. Je me sers de ces valeurs envoyées pour déclencher mes propres évènements MIDI à destination de mes 2 appareils!

2 'ProgramChange' sont envoyés aux 2 appareils si besoin!!! Tous les messages MIDI envoyés par le PACER sont tous transmis tout le long de la chaîne MIDI, en passant par l'ESP32, puis vers l'Ampero et enfin vers le TonaX! Cela permet donc d'utiliser le PACER pour envoyer d'autres messages directement à ces 3 appareils!!!

Je donne un fichier Zip d'un circuit imprimé possible! Le code ne correspond pas exactement à ce circuit, mais il vous sera facile de l'adapter!!!
