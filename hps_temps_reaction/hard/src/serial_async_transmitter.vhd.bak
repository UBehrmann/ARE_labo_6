LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY serial_async_transmitter IS
	PORT (
		clk : IN STD_LOGIC; -- Horloge système
		reset : IN STD_LOGIC; -- Reset asynchrone
		tx_start : IN STD_LOGIC; -- Signal pour démarrer la transmission
		data_in : IN STD_LOGIC_VECTOR (19 DOWNTO 0); -- Données à transmettre (20 bits)
		tx : OUT STD_LOGIC; -- Ligne de transmission série
		tx_busy : OUT STD_LOGIC -- Indicateur de transmission en cours
	);
END serial_async_transmitter;

ARCHITECTURE comport OF serial_async_transmitter IS
	-- Constantes
	CONSTANT TOTAL_BITS : INTEGER := 22; -- 1 bit de start + 20 bits de données + 1 bit de stop
	-- États de la machine à états
	TYPE state_type IS (IDLE, START_BIT, DATA_BITS, STOP_BIT, END_COM);
	SIGNAL state : state_type := IDLE;
	SIGNAL state_fut : state_type := IDLE; -- Prochain état
	-- Signaux internes
	SIGNAL baud_pulse : STD_LOGIC; -- Impulsion du générateur de baudrate
	SIGNAL bit_index : INTEGER RANGE 0 TO TOTAL_BITS - 1 := 0;
	SIGNAL tx_reg : STD_LOGIC := '1';
	SIGNAL data_buffer : STD_LOGIC_VECTOR (19 DOWNTO 0);
	SIGNAL start_reg : STD_LOGIC := '0';
	SIGNAL bit_start : STD_LOGIC := '0';
	SIGNAL bit_finished : STD_LOGIC := '0';
	-- Instanciation du générateur de baudrate
	COMPONENT baudrate_generator IS
		PORT (
			clk : IN STD_LOGIC; -- Horloge système
			reset : IN STD_LOGIC; -- Reset asynchrone
			baud_pulse : OUT STD_LOGIC -- Impulsion de baudrate
		);
	END COMPONENT;
	FOR ALL : baudrate_generator USE ENTITY work.baudrate_generator;

BEGIN
	-- Instancier le générateur de baudrate
	baudrate_gen : baudrate_generator
	PORT MAP(
		clk => clk,
		reset => reset,
		baud_pulse => baud_pulse
	);

	-- Processus pour gérer les transitions d'états
	PROCESS (clk, reset)
	BEGIN
		IF reset = '1' THEN
			start_reg <= '0';
			tx_busy <= '0';
			data_buffer <= (OTHERS => '0');
		ELSIF rising_edge(clk) THEN
			IF tx_start = '1' THEN
				start_reg <= '1';
				tx_busy <= '1';
				data_buffer <= data_in; -- Charger les données à transmettre
			ELSIF state = END_COM THEN
				start_reg <= '0';
			END IF;

		END IF;
	END PROCESS;

	PROCESS (
		baud_pulse,
		reset
		)
	BEGIN
		IF reset = '1' THEN
			state <= IDLE;
		ELSIF rising_edge(baud_pulse) THEN
			state <= state_fut;
		END IF;
	END PROCESS;

	-- Processus pour la logique des états
	PROCESS (
		state,
		start_reg,
		bit_finished,
		bit_index
		)
	BEGIN
		CASE state IS
			WHEN IDLE =>
				tx_reg <= '1';
				IF start_reg = '1' THEN
					state_fut <= START_BIT; -- Pas de condition supplémentaire, car SYNC est géré par le processus d'état
				END IF;
			WHEN START_BIT =>
				tx_reg <= '0'; -- Bit de start
				state_fut <= DATA_BITS;
			WHEN DATA_BITS =>
				tx_reg <= data_buffer(bit_index); -- Envoyer le bit courant
				bit_start <= '1'; -- Signal pour incrémenter l'index
				IF bit_finished = '1' THEN
					state_fut <= STOP_BIT;
					bit_start <= '0';
				END IF;
			WHEN STOP_BIT =>
				tx_reg <= '1'; -- Bit de stop
				state_fut <= END_COM;
			WHEN END_COM =>
				state_fut <= IDLE;
		END CASE;
	END PROCESS;

	PROCESS (
		baud_pulse,
		reset
		)
	BEGIN
		IF reset = '1' THEN
			bit_index <= 19;
		ELSIF rising_edge(baud_pulse) THEN

			bit_finished <= '0';

			IF bit_start = '0' THEN
				bit_index <= 19;
			elsif bit_index = 0 THEN
				bit_finished <= '1';
			else
				bit_index <= bit_index - 1;
			END IF;
			
		END IF;
	END PROCESS;

	tx <= tx_reg;

END comport;