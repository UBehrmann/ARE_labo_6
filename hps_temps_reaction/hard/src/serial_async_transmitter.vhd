library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity serial_async_transmitter is
    Port (
        clk       : in  STD_LOGIC;  -- Horloge système
        reset     : in  STD_LOGIC;  -- Reset asynchrone
        tx_start  : in  STD_LOGIC;  -- Signal pour démarrer la transmission
        data_in   : in  STD_LOGIC_VECTOR (19 downto 0); -- Données à transmettre (20 bits)
        tx        : out STD_LOGIC;  -- Ligne de transmission série
        tx_busy   : out STD_LOGIC   -- Indicateur de transmission en cours
    );
end serial_async_transmitter;

architecture comport of serial_async_transmitter is
	-- Constantes
   constant TOTAL_BITS		: integer := 22; -- 1 bit de start + 20 bits de données + 1 bit de stop
   -- États de la machine à états
   type state_type is 		(IDLE, START_BIT, DATA_BITS, STOP_BIT, END_COM);
   signal state       		: state_type := IDLE;
   signal state_fut   		: state_type := IDLE; -- Prochain état
   -- Signaux internes
   signal baud_pulse  		: STD_LOGIC; -- Impulsion du générateur de baudrate
   signal bit_index   		: integer range 0 to TOTAL_BITS-1 := 0;
   signal tx_reg      		: STD_LOGIC := '1';
   signal data_buffer 		: STD_LOGIC_VECTOR (19 downto 0);
	signal start_reg			: STD_LOGIC := '0';
   -- Instanciation du générateur de baudrate
   component baudrate_generator is
		Port (
			clk        : in  STD_LOGIC;  -- Horloge système
			reset      : in  STD_LOGIC;  -- Reset asynchrone
			baud_pulse : out STD_LOGIC   -- Impulsion de baudrate
		);
   end component;
	for all : baudrate_generator use entity work.baudrate_generator;

begin
	-- Instancier le générateur de baudrate
   baudrate_gen : baudrate_generator
   port map (
		clk => clk,
		reset => reset,
      baud_pulse => baud_pulse
   );

	-- Processus pour gérer les transitions d'états
	process(clk, reset)
	begin
   if reset = '1' then
		start_reg <= '0';
		tx_busy <= '0';
		data_buffer <= (OTHERS => '0');
   elsif rising_edge(clk) then
		if tx_start = '1' then
			start_reg <= '1';
			tx_busy <= '1';
			data_buffer <= data_in; -- Charger les données à transmettre
		elsif state = END_COM then
			start_reg <= '0';			
		end if;
		
   end if;
	end process;
	
	process(baud_pulse, reset)
	begin
	if reset = '1' then	
		state <= IDLE;
	elsif rising_edge(baud_pulse) then
		 state <= state_fut;
	end if;
	end process;

	-- Processus pour la logique des états
	process(baud_pulse)
	begin
	if rising_edge(baud_pulse) then
		case state is
			when IDLE =>
				tx_reg <= '1';
				bit_index <= 0;
				if start_reg = '1' then
					state_fut <= START_BIT; -- Pas de condition supplémentaire, car SYNC est géré par le processus d'état
				end if;
			when START_BIT =>
				tx_reg <= '0'; -- Bit de start
				state_fut <= DATA_BITS;
			when DATA_BITS =>
				tx_reg <= data_buffer(bit_index); -- Envoyer le bit courant
				if bit_index < 19 then
					bit_index <= bit_index + 1;
				else
					state_fut <= STOP_BIT;
				end if;
			when STOP_BIT =>
				tx_reg <= '1'; -- Bit de stop
				state_fut <= END_COM;
			when END_COM =>
				state_fut <= IDLE;
		end case;
	end if;
	end process;

	tx <= tx_reg;
		
end comport;
