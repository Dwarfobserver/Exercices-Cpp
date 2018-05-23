
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

# Exercices du cours de C++ 

Ce répertoire contient des exercices pour le cours de C++ disponible ici :
https://cours-cpp.gitbook.io/resources/

Pour faire ces exercices, vous pouvez cloner le dépôt et compléter le
code source contenu dans les dossiers 'src' et 'include'.

Les tests peuvent être lancés avec les commandes suivantes :

```bash

mkdir build
cd build
cmake ..
cmake --build .
ctest -V

```

Je peux vous ajouter comme collaborateurs afin que vous puissez tenter
les exercices sur votre propre branche. Ainsi, lorsque vous allez push,
Travis et Appveyor vont lancer les tests sur Windows (avec MSVC) et Linux
(avec GCC) automatiquement :

 - Builds de Travis : https://travis-ci.org/Dwarfobserver/Exercices-Cpp
 - Builds d'AppVeyor : https://ci.appveyor.com/project/Dwarfobserver/Exercices-Cpp

Les tests sont exécutés avec Catch. Le programme peut prendre de nombreux
arguments, listés ici :
https://github.com/catchorg/Catch2/blob/master/docs/command-line.md#top .
Par exemple, exécuter "tests.exe [tuples]" ne va lancer que les tests
concernant les tuples.

Il y a actuellement deux exercices :

 - 'slot_map' nécessite l'implémentation d'une structure de données.
 - 'tuples' contient plusieurs algorithmes devant s'exécuter à la compilation.
