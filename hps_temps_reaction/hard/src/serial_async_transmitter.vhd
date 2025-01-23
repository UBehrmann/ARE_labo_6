LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_ARITH.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;


ENTITY serial_async_transmitter IS
	PORT (
		clk 		: IN STD_LOGIC; 								-- Horloge système
		reset 	: IN STD_LOGIC; 								-- Reset asynchrone
		tx_start : IN STD_LOGIC; 								-- Signal pour démarrer la transmission
		data_in 	: IN STD_LOGIC_VECTOR (19 DOWNTO 0);	-- Données à transmettre (20 bits)
		tx 		: OUT STD_LOGIC; 								-- Ligne de transmission série
		tx_busy 	: OUT STD_LOGIC 								-- Indicateur de transmission en cours
	);
END serial_async_transmitter;


ARCHITECTURE comport OF serial_async_transmitter IS
	-- États de la machine à états
	TYPE state_type IS (	IDLE, START_BIT, 
								DATA_BIT_19,
								DATA_BIT_18,
								DATA_BIT_17,
								DATA_BIT_16,
								DATA_BIT_15,
								DATA_BIT_14,
								DATA_BIT_13,
								DATA_BIT_12,
								DATA_BIT_11,
								DATA_BIT_10,
								DATA_BIT_9,
								DATA_BIT_8,
								DATA_BIT_7,
								DATA_BIT_6,
								DATA_BIT_5,
								DATA_BIT_4,
								DATA_BIT_3,
								DATA_BIT_2,
								DATA_BIT_1,
								DATA_BIT_0,
								STOP_BIT, END_COM);
	-- Signaux d'état
	SIGNAL state 			: state_type := IDLE;
	SIGNAL state_fut 		: state_type := IDLE; -- Prochain état
	-- Signaux internes
	SIGNAL baud_pulse 	: STD_LOGIC; -- Impulsion du générateur de baudrate
	SIGNAL tx_reg 			: STD_LOGIC := '1';
	SIGNAL data_buffer 	: STD_LOGIC_VECTOR (19 DOWNTO 0);
	SIGNAL start_reg 		: STD_LOGIC := '0';
	-- Instanciation du générateur de baudrate
	COMPONENT baudrate_generator IS
		PORT (
			clk 				: IN STD_LOGIC; -- Horloge système
			reset 			: IN STD_LOGIC; -- Reset asynchrone
			baud_pulse 		: OUT STD_LOGIC -- Impulsion de baudrate
		);
	END COMPONENT;
	FOR ALL : baudrate_generator USE ENTITY work.baudrate_generator;

BEGIN
	-------------------------------------------------------------------
	-- Instancier le générateur de baudrate
	-------------------------------------------------------------------
	baudrate_gen : baudrate_generator
	PORT MAP(
		clk			=> clk,
		reset 		=> reset,
		baud_pulse 	=> baud_pulse
	);

	-------------------------------------------------------------------
	-- Processus pour la gestions :
	-- - start_reg 	: Start de la machine d'état (IDLE -> START_BIT)
	-- - tx_busy		: Indicateur que une communication est en cours
	-- - data_buffer	: Buffer des datas à communiquer
	-- - state			: Etat de la machine d'état (MSS)55
	-------------------------------------------------------------------
	PROCESS (clk, reset)
	BEGIN
		IF reset = '1' THEN
			start_reg <= '0';
			tx_busy <= '0';
			data_buffer <= (OTHERS => '0');
			state <= IDLE;
		ELSIF rising_edge(clk) THEN
			IF tx_start = '1' THEN
				start_reg <= '1';
				tx_busy <= '1';
				data_buffer <= data_in; -- Charger les données à transmettre
			ELSIF baud_pulse = '1' THEN
				state <= state_fut;
			ELSIF state = END_COM THEN
				start_reg <= '0';
				tx_busy <= '0';
			END IF;
		END IF;
	END PROCESS;
	-------------------------------------------------------------------
	-- Processus pour la MSS
	-------------------------------------------------------------------
	PROCESS (state, start_reg)
	BEGIN
		CASE state IS
			WHEN IDLE =>
				tx_reg <= '1';
				IF start_reg = '1' THEN
					state_fut <= START_BIT; -- Pas de condition supplémentaire, car SYNC est géré par le processus d'état
				END IF;
			WHEN START_BIT =>
				tx_reg <= '0'; -- Bit de start
				state_fut <= DATA_BIT_19;
         WHEN DATA_BIT_19 =>
             tx_reg <= data_buffer(19);
             state_fut <= DATA_BIT_18;
         WHEN DATA_BIT_18 =>
             tx_reg <= data_buffer(18);
             state_fut <= DATA_BIT_17;
         WHEN DATA_BIT_17 =>
             tx_reg <= data_buffer(17);
             state_fut <= DATA_BIT_16;
         WHEN DATA_BIT_16 =>
             tx_reg <= data_buffer(16);
             state_fut <= DATA_BIT_15;
         WHEN DATA_BIT_15 =>
             tx_reg <= data_buffer(15);
             state_fut <= DATA_BIT_14;
         WHEN DATA_BIT_14 =>
             tx_reg <= data_buffer(14);
             state_fut <= DATA_BIT_13;
         WHEN DATA_BIT_13 =>
             tx_reg <= data_buffer(13);
             state_fut <= DATA_BIT_12;
         WHEN DATA_BIT_12 =>
             tx_reg <= data_buffer(12);
             state_fut <= DATA_BIT_11;
         WHEN DATA_BIT_11 =>
             tx_reg <= data_buffer(11);
             state_fut <= DATA_BIT_10;
         WHEN DATA_BIT_10 =>
             tx_reg <= data_buffer(10);
             state_fut <= DATA_BIT_9;
         WHEN DATA_BIT_9 =>
             tx_reg <= data_buffer(9);
             state_fut <= DATA_BIT_8;
         WHEN DATA_BIT_8 =>
             tx_reg <= data_buffer(8);
             state_fut <= DATA_BIT_7;
         WHEN DATA_BIT_7 =>
             tx_reg <= data_buffer(7);
             state_fut <= DATA_BIT_6;
         WHEN DATA_BIT_6 =>
             tx_reg <= data_buffer(6);
             state_fut <= DATA_BIT_5;
         WHEN DATA_BIT_5 =>
             tx_reg <= data_buffer(5);
             state_fut <= DATA_BIT_4;
         WHEN DATA_BIT_4 =>
             tx_reg <= data_buffer(4);
             state_fut <= DATA_BIT_3;
         WHEN DATA_BIT_3 =>
             tx_reg <= data_buffer(3);
             state_fut <= DATA_BIT_2;
         WHEN DATA_BIT_2 =>
             tx_reg <= data_buffer(2);
             state_fut <= DATA_BIT_1;
         WHEN DATA_BIT_1 =>
             tx_reg <= data_buffer(1);
             state_fut <= DATA_BIT_0;
         WHEN DATA_BIT_0 =>
             tx_reg <= data_buffer(0);
             state_fut <= STOP_BIT;
			WHEN STOP_BIT =>
				tx_reg <= '1'; -- Bit de stop
				state_fut <= END_COM;
			WHEN END_COM =>
				state_fut <= IDLE;
		END CASE;
	END PROCESS;

	-- Mise à jour de la sortie de communication
	tx <= tx_reg;

END comport;