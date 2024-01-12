
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
![Capture d'écran 2024-01-12 135323](https://github.com/ESN2024/roche_lab1/assets/116710033/0fc5e62b-74dd-4033-aab1-e6cadecc4444)

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

## Langage C (main.c)
Le code C se divise en trois parties :
1. `irqhandler_bouton_key1()` : Routine d’interruption du bouton KEY1, le flag d’interruption n’est jamais remis à 0. C’est-à-dire que le programme boucle infiniment sur cette routine au déclenchement du bouton. Cette routine écrit dans le PIO_O_1 la données data qui est décalé à chaque occurrence de cette routine ou mis à 0x01 si data = 0x08. Ceci décrit le fonctionnement du chenillards. La vitesse du chenillards est géré par la fonction de pause **usleep()** suivant la valeur de speed. Quand speed est petit (=1) la vitesse du chenillard est lente et inversement.

2. `irqhandler_bouton_key1()` : Routine d’interruption des 4 premiers switchs. Cette fois ci le flag d’interruption est réinitialiser pour que la routine s’execute à chaque changement d’état des switchs. La variable speed prend la valeur des 4 switch en rajoutant 1. Cela permet d’éviter que speed soit égale à 0 et que cela viennent créer des problème à l’appelle de usleep(400000/speed).
Cette interruption à un niveau de priorité supérieur à celle du chenillards.

3. `main()` : Programme principal conçu pour enregistrer les interruptions du bouton et des switchs permettant le bon fonctionnement du système.
   eddeded
   - ***IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_2_BASE, 0x0F);*** cette ligne permet de configurer un masque sur le PIO en utilisant uniquement les 4 bits de poids faible des switchs.
   - ***IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0x0F);*** cette ligne permet de configurer le type d’évènement qui declenchera l’interruption (ici un changement d’etat d’au moins un des 4 switchs)
   - ***alt_irq_register()*** Permet d’enregister les interruptions dans le gestionnaire d’interruption.

## Conclusion
Discussion sur les problèmes de priorité des interruptions et des flags `cap edge` à 1.
