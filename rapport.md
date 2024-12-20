<div align="justify" style="margin-right:25px;margin-left:25px">

# Laboratoire 06 - Mesure du temps de réaction <!-- omit from toc -->

## Etudiants

- Rodrigo Lopez Dos Santos
- Urs Behrmann

# Table des matières

- [Table des matières](#table-des-matières)
- [Introduction](#introduction)
- [Analyse](#analyse)
  - [Plan d’adressage](#plan-dadressage)
  - [Emetteur série asynchrone](#emetteur-série-asynchrone)
- [Implémentation](#implémentation)
- [Tests](#tests)
- [Conclusion](#conclusion)

# Introduction

# Analyse

## Plan d’adressage



| Address (offset) | Read                      | Write               |
| ---------------- | ------------------------- | ------------------- |
| 0x00             | [31..0] Interface user ID | reserved            |
| 0x04             | [31..4] "0..0"            | reserved            |
|                  | [3..0] buttons            |                     |
| 0x08             | [31..10] "0..0"           | reserved            |
|                  | [9..0] switches           |                     |
| 0x0C             | [31..10] "0..0"           | [31..10] reserved   |
|                  | [9..0] leds               | [9..0] leds         |
| 0x10             | [31..28] reserved         | [31..28] reserved   |
|                  | [27..21] hex3             | [27..21] hex3       |
|                  | [20..14] hex2             | [20..14] hex2       |
|                  | [13..7] hex1              | [13..7] hex1        |
|                  | [6..0] hex0               | [6..0] hex0         |
| 0x14             | [31..0] Compteur          | [31..2] reserved    |
|                  | [1] reset compteur        |                     |
|                  | [0] enable compteur       |                     |
| 0x18             | [31..2] reserved          | [31..1] reserved    |
|                  | [1] status rising edge    |                     |
|                  | [0] interrupt             | [0] reset interrupt |
| 0x1C             | [31..1] reserved          | [31..1] reserved    |
|                  | [0] interrupt mask        | [0] interrupt mask  |
| 0x20             | [31..0] reserved          | [31..20] reserved   |
|                  |                           | [19..0] data max10  |
| 0x24             | [31..3] reserved          | [31..0] reserved    |
|                  | [2..1] status max10       |                     |
|                  | [0] ready max10           |                     |

## Emetteur série asynchrone

Pour l'émetteur série asynchrone, nous avons dû utilsé une baudrate de 9600. Mais la vitesse de l'horloge étant de 50 MHz, nous avons dû utiliser un timer pour diviser cette fréquence. Nous avons donc utilisé un timer qui compte jusqu'à 5208 pour obtenir une fréquence de environ 9600 Hz.

# Implémentation

# Tests

# Conclusion

