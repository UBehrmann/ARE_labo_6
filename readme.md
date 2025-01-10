# ARE Laboratoire 6

# Simulation

Dans le dossier ../hps_acquis_string/hard/sim/

> vsim -do ../script/run_avalon_sim.tcl

# Code pour lancer le preloader

Dans le dossier ../hps_acquis_string/hard/script/

```bash

python3 pgm_fpga.py -s=../eda/output_files/DE1_SoC_top.sof
python3 upld_hps.py

```

# Fonctionnement

bouton 0 : start le count down et affiche une croix sur la max10
bouton 1 : stop le count et affiche un petit carré sur la max10, si on a appuyé sur le bouton quand l'affichage est passé sur un grand carré. Si on a appuyé sur le bouton avant que l'affichage ne passe sur un grand carré, l'affichage ne change pas et une erreur est comptée.

Switchs 0-3 à zero : affiche le temps du dernier essaie ou que des 0 si aucun essaie n'a été fait
switch 0 à 1 : affiche le temps le plus court
switch 1 à 1 : affiche le temps le plus long
switch 2 à 1 : affiche le nombre d'erreur
switch 3 à 1 : affiche le nombre d'essaie