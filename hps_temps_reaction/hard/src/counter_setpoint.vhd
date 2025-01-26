library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity counter_setpoint is
	Port (
		clk    			: in  STD_LOGIC; -- Signal d'horloge
		reset  			: in  STD_LOGIC; -- Signal de remise à zéro
		start				: in  STD_LOGIC;
		stop				: in  STD_LOGIC;
		setpoint  		: in STD_LOGIC_VECTOR(31 downto 0);
		finished  		: out STD_LOGIC
	);
end counter_setpoint;

architecture comport of counter_setpoint is
    -- Signal interne pour le compteur
    signal counter      : unsigned(31 downto 0) := (others => '0');
    signal counting     : STD_LOGIC := '0';
    signal finished_s   : STD_LOGIC := '0';
begin
    -- PROCESSUS PRINCIPAL
    process(clk, reset)
    begin
        if reset = '1' then
            -- Réinitialisation
            counter 		<= (others => '0');
            counting 	<= '0';
            finished_s 	<= '0';
        elsif rising_edge(clk) then
            -- Logique du compteur
            if start = '1' then
                counting <= '1'; -- Activer le comptage
                finished_s <= '0'; -- Réinitialiser l'indicateur de fin
					 counter 		<= (others => '0');
            elsif stop = '1' then
                counting <= '0'; -- Désactiver le comptage
					 finished_s <= '0'; -- Indiquer que la cible est atteinte
					 counter 		<= (others => '0');
            end if;
				
            if counting = '1' then
                if counter < unsigned(setpoint) then
                    counter <= counter + 1; -- Incrémenter le compteur
                else
                    finished_s <= '1'; -- Indiquer que la cible est atteinte
                    counting <= '0'; -- Arrêter le comptage
                end if;
            end if;
				
        end if;
    end process;

    -- Assignation de la sortie
    finished <= finished_s;
	
end comport;
