# 🧮 Evalexpr - Évaluateur d'expressions arithmétiques

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Shell](https://img.shields.io/badge/Testing-Shell-green.svg)
![EPITA](https://img.shields.io/badge/School-EPITA-red.svg)

Il s'agit du développement d'un programme complet permettant de parser et d'évaluer dynamiquement des expressions arithmétiques complexes depuis l'entrée standard.

## 📝 Description

L'objectif principal de ce projet est de lire une expression mathématique sous forme de chaîne de caractères, de l'analyser, puis d'afficher le résultat exact du calcul sur la sortie standard. Le code a été rédigé en respectant la norme C99 et doit compiler sans erreur avec les flags `-Werror`, `-Wall`, `-Wextra`, `-Wvla` et `-pedantic`.

Les fonctionnalités et contraintes principales de notre implémentation sont :

- **Double notation supportée** : Le programme est capable d'évaluer des expressions en notation standard (infixe) ainsi qu'en Notation Polonaise Inversée (RPN, via l'argument `-rpn`).
- **Opérations avancées** : Prise en charge des opérateurs binaires classiques (`+`, `-`, `*`, `/`), du modulo (`%`), de l'exponentiation (`^`), ainsi que des opérateurs unaires (`+`, `-`) et des parenthèses imbriquées.
- **Conversion algorithmique** : Utilisation de l'algorithme de la gare de triage (*Shunting-yard*) pour transformer proprement les expressions standards en notation RPN à l'aide de piles (Stack) et de files (FIFO).
- **Gestion rigoureuse des erreurs** : Le programme ne plante jamais et retourne des codes d'erreur précis selon le standard UNIX :
  - `0` : Succès
  - `1` : Erreur lexicale (caractère invalide)
  - `2` : Erreur syntaxique (expression mal formée)
  - `3` : Erreur arithmétique (division/modulo par zéro, puissance négative)
  - `4` : Autre erreur (ex : mauvais arguments)

## 🏗️ Architecture du Projet

Le code source est modulaire et sépare la gestion des structures de données de la logique mathématique :

- 📂 `src/` : Contient le code source principal (`main.c`), l'implémentation des algorithmes (`rpn.c`, `stdn.c`) et les structures de données personnalisées (`stack.c`, `fifo.c`, `token.h`).
- 📂 `tests/` : Contient la suite de tests fonctionnels (`check.sh`) permettant de valider le comportement du programme via des scripts shell.
- 📄 `Makefile` : Le fichier d'orchestration situé à la racine pour automatiser la compilation et les tests.

## ⚙️ Prérequis

Pour compiler et tester ce projet, votre environnement (idéalement Linux / UNIX) doit disposer des outils suivants :

- Compilateur **GCC** ou **Clang**
- **Make**
- Un interpréteur de commandes **Shell** (sh/bash) pour lancer la suite de tests.

Installation des dépendances (Ubuntu/Debian) :

```bash
sudo apt update
sudo apt install build-essential
```

## 🚀 Compilation & Exécution (Makefile)

Le projet utilise un `Makefile` principal situé à la racine pour faciliter l'intégration et la compilation.

### 1. Compilation simple

Pour générer l'exécutable final `evalexpr` à la racine du projet :

```bash
make
```

### 2. Lancement de la suite de tests

Pour exécuter automatiquement la suite de tests fonctionnels (vérifiant les cas valides et les différentes erreurs) :

```bash
make check
```

> **Note :** Le script de test valide silencieusement les sorties standards et les codes de retour (exit status) de chaque opération.

### 3. Nettoyage du projet

Pour supprimer tous les fichiers produits par `make` lors de la compilation (`.o`, exécutables) afin de repartir sur un répertoire propre :

```bash
make clean
```

## 🖱️ Notice d'utilisation

Une fois le programme compilé, vous pouvez lui passer des expressions via l'entrée standard (par exemple en utilisant `echo` et un pipe `|`).

### 1. Évaluation en notation standard (infixe)

Appelez simplement le programme sans argument. La priorité des opérateurs et les parenthèses sont automatiquement respectées.

```bash
echo "5 * (2^2 + 3)" | ./evalexpr
# Affiche : 35
```

### 2. Évaluation en Notation Polonaise Inversée (RPN)

Utilisez l'argument `-rpn` pour indiquer que l'expression est fournie au format RPN.

```bash
echo "5 2 2 ^ 3 + *" | ./evalexpr -rpn
# Affiche : 35
```

### 3. Récupération des codes d'erreur

Si une expression est invalide, rien n'est imprimé sur la sortie standard, mais le code de retour (`$?`) indique la nature de l'erreur.

```bash
echo "1 / 0" | ./evalexpr
echo $?
# Affiche : 3 (Erreur arithmétique)
```

## 👥 Auteur

Projet réalisé par :

- Antoine Ramstein
