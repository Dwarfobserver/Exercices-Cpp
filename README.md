
# Exercices du cours de C++ 

Ce répertoire contient les exercices du cours de C++ disponible ici :
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

Je peux vous ajouter comme collaborateurs, afin que vous puissez tenter
les exercices sur votre propre branche. Ainsi, lorsque vous allez push,
Travis et Appveyor vont lancer les tests sur Windows (avec MSVC) et Linux
(avec GCC) automatiquement.
