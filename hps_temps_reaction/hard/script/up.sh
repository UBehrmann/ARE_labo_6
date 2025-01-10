#!/bin/bash

# Program the FPGA
python3 pgm_fpga.py -s=../eda/output_files/DE1_SoC_top.sof

# Upload to HPS
python3 upld_hps.py -a=/media/sf_Partage_VM_Reds/ARE/ARE_labo_6/hps_temps_reaction/soft/proj/hps_temps_reaction/hps_temps_reaction/Debug/hps_temps_reaction.axf