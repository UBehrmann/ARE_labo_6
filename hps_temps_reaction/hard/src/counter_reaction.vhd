library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity counter_reaction is
	Port (
		clk       : in  STD_LOGIC;  -- Horloge système
      reset     : in  STD_LOGIC;  -- Reset asynchrone
      start     : in  STD_LOGIC;  -- Signal pour démarrer le comptage
      stop      : in  STD_LOGIC;  -- Signal pour arrêter le comptage
      delta     : out STD_LOGIC_VECTOR (31 downto 0);  -- Temps écoulé entre start et stop
		error_cnt : out STD_LOGIC_VECTOR (31 downto 0);   -- Nombre d'erreurs
		trys_cnt	 : out STD_LOGIC_VECTOR (31 downto 0)   -- Nombre d'essaies
	);
end counter_reaction;

architecture comport of counter_reaction is
	signal count      	: STD_LOGIC_VECTOR (31 downto 0) := (others => '0'); -- Compteur interne
   signal counting   	: STD_LOGIC := '0'; -- Indicateur de comptage en cours
	signal errors     	: STD_LOGIC_VECTOR (31 downto 0) := (others => '0'); -- Nombre d'erreurs
	signal trys				: STD_LOGIC_VECTOR (31 downto 0) := (others => '0'); -- Nombre d'erreurs
   constant MAX_COUNT	: STD_LOGIC_VECTOR (31 downto 0) := X"FFFFFFFF"; -- Valeur maximale (2^32 - 1)
begin

	process(clk, reset)
   begin
   if reset = '1' then
      counting 		<= '0';
		count 			<= (others => '0');
		errors 			<= (others => '0');
		trys 				<= (others => '0');
   elsif rising_edge(clk) then
		if start = '1' then
			counting		<= '1';
         count 		<= (others => '0'); -- Réinitialise le compteur à chaque démarrage
      elsif stop = '1' then
			if counting = '0' then
				errors 	<= errors + 1; -- Incrémente le compteur d'erreurs si stop est activé alors que le comptage est arrêté
			else
				counting <= '0'; -- Arrête le comptage
			end if;
			trys 			<= trys + 1; -- Incrémente le compteur d'erreurs si stop est activé alors que le comptage est arrêté
      end if;
      if counting = '1' and count < MAX_COUNT then
          count 		<= count + 1; -- Incrémente le compteur si la valeur maximale n'est pas atteinte
      end if;
   end if;
	end process;

   delta 				<= count; -- La valeur finale du compteur est sortie sur delta
	error_cnt 			<= errors; -- Le compteur d'erreurs est disponible en sortie
	trys_cnt				<= trys;
   -- Commentaire :
   -- Avec un compteur de 32 bits et une horloge à 50 MHz :
   -- Temps maximal mesurable = 2^32 / 50e6 secondes = ~85.899 secondes.
end comport;
