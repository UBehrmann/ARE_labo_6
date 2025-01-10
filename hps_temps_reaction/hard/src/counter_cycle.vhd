library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity counter_cycle is
	Generic (
		CLK_FREQ 		: integer := 50000000; -- Fréquence de l'horloge en Hz (par défaut 50 MHz)
		TIME_TARGET 	: integer := 3      -- Temps cible en secondes (par défaut 3 s)
	);
	Port (
		clk    			: in  STD_LOGIC; -- Signal d'horloge
		reset  			: in  STD_LOGIC; -- Signal de remise à zéro
		enable 			: in  STD_LOGIC; -- Permet de contrôler l'incrémentation
		count  			: out STD_LOGIC_VECTOR(31 downto 0) -- La valeur du compteur en binaire
	);
end counter_cycle;

architecture comport of counter_cycle is
	-- Calcul automatique de la limite maximale du compteur
	constant COUNTER_MAX	: integer := CLK_FREQ * TIME_TARGET - 1;
	-- Signal interne pour stocker la valeur du compteur
	signal counter_reg 	: INTEGER range 0 to COUNTER_MAX := 0;
	
begin
	process(clk)
	begin
	if rising_edge(clk) then
		if reset = '1' then
			counter_reg <= 0; -- Remise à zéro
		elsif enable = '1' then
			if counter_reg = COUNTER_MAX then
				counter_reg <= 0; -- Retour à 0 après avoir atteint la limite
			else
				counter_reg <= counter_reg + 1; -- Incrémentation
			end if;
		end if;
	end if;
	end process;
	
   count <= std_logic_vector(to_unsigned(counter_reg, 32)); -- Conversion de 32 bits
	
end comport;
