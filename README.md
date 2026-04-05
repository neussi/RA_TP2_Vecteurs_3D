# Vecteurs 3D & Projection

## Description
Implémentation complète des classes vectorielles (Vec2d, Vec3d, Vec4d) et des opérations fondamentales pour le pipeline 3D de réalité augmentée.

Ce projet fait partie du cursus de Réalité Augmentée From Scratch (Semestre 1). L'implémentation est réalisée entièrement en C++17 sans bibliothèque tierce (Zero STL), en suivant les directives de développement moteur Jenga/Nkentseu.

## Fonctionnalités implémentées
- Opérateurs arithmétiques et géométriques (Dot, Cross).
- Orthonormalisation de Gram-Schmidt.
- Déhomogénéisation des coordonnées (Vec4d vers Vec3d).
- Projection perspective simple d'un cube filaire.

## Installation et Compilation
Le projet utilise le système de build **Jenga**.

1. Assurez-vous d'avoir Jenga installé sur votre système.
2. Clonez le dépôt :
   ```bash
   git clone git@github.com:neussi/RA_TP2_Vecteurs_3D.git
   cd RA_TP2_Vecteurs_3D
   ```
3. Compilez le projet :
   ```bash
   jenga build
   ```
4. Exécutez le programme :
   ```bash
   jenga run TP2
   ```

## Résultats
Le programme génère des sorties dans la console et, le cas échéant, des fichiers images (.ppm) illustrant les concepts mathématiques et graphiques abordés.

## Auteur
**NEUSSI NJIETCHEU PATRICE EUGNE**
Matricule : 24P820
