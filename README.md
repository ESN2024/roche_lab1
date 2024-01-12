
# ESN11 - Lab1

## Objectif
Implémentation d'un chenillard sur la carte DE-10 Lite en utilisant Quartus et un système NIOS II.

## Introduction
Ce rapport décrit l'implémentation d'un chenillard qui s'active lorsque le bouton KEY1 de la carte est actionné (déclenché par interruption). On peut changer la vitesse du chenillard avec les 4 bits des premiers switchs.

Le programme utilise deux types d'interruptions :
1. Une déclenche le chenillard à l'action du bouton.
2. Une change la vitesse du chenillard, activée par le changement d'état des switchs.

## Création d'un système NIOS II basique

### Schéma Fonctionnel
![Capture d'écran 2024-01-12 133606](https://github.com/ESN2024/roche_lab1/assets/116710033/fe3e2bd7-4579-452e-a734-5ded8e070ccc)
### Platform Designer
- Après la création d'un nouveau projet sur Quartus, ouvrir le Platform Designer pour ajouter tous les éléments nécessaires.
- **Mémoire** : RAM avec un stockage de 40 Mo.
- **PIO_O_1** : Correspond aux LEDS de la carte, taille de 8 bits en sortie.
- **PIO_O_2** : Correspond au bouton, taille de 1 bit en entrée. Ajouter une interruption de type IRQ avec une priorité de 2.
- **PIO_O_3** : Correspond aux switchs, taille de 8 bits en entrée. Ajouter une interruption avec les mêmes caractéristiques que pour le bouton mais avec une priorité de 2.

### Quartus
- Vérifier l'absence d'erreurs et générer le VHDL.
- Relier les E/S avec les PIN correspondants sur la carte en utilisant l'outil Pin Planner de Quartus.
- Compiler le projet Quartus et programmer la carte avec USB-Blaster.

## Flux Logiciel du NIOS II
- Ouvrir un terminal NIOS II et générer le BSP en lien avec les spécifications de la carte DE-10.
- Générer le Makefile du projet.

## Langage C
Le code C se divise en trois parties :
1. `irqhandler_bouton_key1()` : Routine d’interruption du bouton KEY1, le flag d’interruption n’est jamais remis à 0. C’est-à-dire que le programme boucle infiniment sur cette routine au déclenchement du bouton. Cette routine écrit dans le PIO_O_1 la données data qui est décalé à chaque occurrence de cette routine ou mis à 0x01 si data = 0x08. Ceci décrit le fonctionnement du chenillards

La vitesse du chenillards est géré par la fonction de pause usleep() suivant la valeur de speed. Quand speed est petit (=1) la vitesse du chenillard est lente et inversement.

2. Gestion de la vitesse du chenillard avec la fonction `usleep()` selon la valeur de `speed`.
3. `irqhandler_switch()` : Routine d'interruption des 4 premiers switchs.

## Conclusion
Discussion sur les problèmes de priorité des interruptions et des flags `cap edge` à 1.
