-------------------------------------------------------------------------------
-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
-- Institut REDS, Reconfigurable & Embedded Digital Systems
--
-- Fichier      : compteur.vhd
-- Auteur       : Urs Behrmann
-- 
-- Description  : Compteur 32 bits
-- 
-- Utilise      : Labo SysLog2 2016
--| Modifications |------------------------------------------------------------
-- Ver   Date      Qui         Description
-- 1.0   20.12.24  UBN         version initiale
-------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY compteur IS
    PORT (
        clock_i : IN STD_LOGIC;
        reset_i : IN STD_LOGIC;
        enable_i : IN STD_LOGIC;
        value_o : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
END compteur;

ARCHITECTURE comport OF compteur IS

    SIGNAL compteur_pres : STD_LOGIC_VECTOR(31 DOWNTO 0);
    SIGNAL compteur_fut : STD_LOGIC_VECTOR(31 DOWNTO 0);

BEGIN

    compteur_fut <= STD_LOGIC_VECTOR(unsigned(compteur_pres) + 1) WHEN (enable_i = '1') ELSE
        compteur_pres;

    PROCESS (reset_i, clock_i)
    BEGIN
        IF reset_i = '1' THEN
            compteur_pres <= (OTHERS => '0');
        ELSIF rising_edge(clock_i) THEN
            compteur_pres <= compteur_fut;
        END IF;
    END PROCESS;

    value_o <= compteur_pres;

END comport;