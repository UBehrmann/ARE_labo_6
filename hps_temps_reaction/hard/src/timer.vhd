-------------------------------------------------------------------------------
-- HEIG-VD, Haute Ecole d'Ingenierie et de Gestion du canton de Vaud
-- Institut REDS, Reconfigurable & Embedded Digital Systems
--
-- Fichier      : timer.vhd
-- Auteur       : Etienne Messerli, le 05.05.2016
-- 
-- Description  : Detection d'un clic et double clic
--                Projet repris du labo Det_Clic_DblClic 2012
-- 
-- Utilise      : Labo SysLog2 2016
--| Modifications |------------------------------------------------------------
-- Ver   Date      Qui         Description
-- 1.0   05.05.16  EMI         version initiale
-- 1.1   19.11.20  SMS         remplacement des constantes par des g�n�riques
-- 1.2   20.12.24  UBN         adaptation pour le labo 6
-------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY timer IS
    GENERIC (
        T1_g : NATURAL RANGE 1 TO 8191 := 2);
    PORT (
        clock_i : IN STD_LOGIC;
        reset_i : IN STD_LOGIC;
        start_i : IN STD_LOGIC;
        trigger_o : OUT STD_LOGIC
    );
END timer;

ARCHITECTURE comport OF timer IS

    CONSTANT ZEROS : STD_LOGIC_VECTOR(12 DOWNTO 0) := (OTHERS => '0');

    SIGNAL timer_pres : STD_LOGIC_VECTOR(12 DOWNTO 0);
    SIGNAL timer_fut : STD_LOGIC_VECTOR(12 DOWNTO 0);

    SIGNAL t1 : STD_LOGIC_VECTOR(12 DOWNTO 0);

BEGIN

    t1 <= STD_LOGIC_VECTOR(to_unsigned(T1_g, t1'length));

    timer_fut <= t1 WHEN (start_i = '1') ELSE
        STD_LOGIC_VECTOR(unsigned(timer_pres) - 1);

    PROCESS (reset_i, clock_i)
    BEGIN
        IF reset_i = '1' THEN
            timer_pres <= (OTHERS => '0');
        ELSIF rising_edge(clock_i) THEN
            timer_pres <= timer_fut;
        END IF;
    END PROCESS;

    trigger_o <= '1' WHEN (timer_pres = ZEROS) ELSE
        '0';

END comport;