------------------------------------------------------------------------------------------
-- HEIG-VD ///////////////////////////////////////////////////////////////////////////////
-- Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
-- School of Business and Engineering in Canton de Vaud
------------------------------------------------------------------------------------------
-- REDS Institute ////////////////////////////////////////////////////////////////////////
-- Reconfigurable Embedded Digital Systems
------------------------------------------------------------------------------------------
--
-- File                 : avl_user_interface.vhd
-- Author               : 
-- Date                 : 04.08.2022
--
-- Context              : Avalon user interface
--
------------------------------------------------------------------------------------------
-- Description : 
--   
------------------------------------------------------------------------------------------
-- Dependencies : 
--   
------------------------------------------------------------------------------------------
-- Modifications :
-- Ver    Date        Engineer    Comments
-- 0.0    See header              Initial version

------------------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    
entity avl_user_interface is
  port(
    -- Avalon bus
    avl_clk_i           : in  std_logic;
    avl_reset_i         : in  std_logic;
    avl_address_i       : in  std_logic_vector(13 downto 0);
    avl_byteenable_i    : in  std_logic_vector(3 downto 0);
    avl_write_i         : in  std_logic;
    avl_writedata_i     : in  std_logic_vector(31 downto 0);
    avl_read_i          : in  std_logic;
    avl_readdatavalid_o : out std_logic;
    avl_readdata_o      : out std_logic_vector(31 downto 0);
    avl_waitrequest_o   : out std_logic;
    avl_irq_o           : out std_logic;
    -- User interface
    button_i            : in  std_logic_vector(3 downto 0);
    switch_i            : in  std_logic_vector(9 downto 0);
    led_o               : out std_logic_vector(9 downto 0);
    hex0_o              : out std_logic_vector(6 downto 0);
    hex1_o              : out std_logic_vector(6 downto 0);
    hex2_o              : out std_logic_vector(6 downto 0);
    hex3_o              : out std_logic_vector(6 downto 0);
    -- Con 80p interface
    serial_data_o       : out std_logic;
    con_80p_status_i    : in  std_logic_vector(1 downto 0)
  );
end avl_user_interface;

architecture rtl of avl_user_interface is
   --| Components declaration |--------------------------------------------------------------
	-- Serial asynchrone transmitter
	COMPONENT serial_async_transmitter IS
		PORT (
			clk       : in  STD_LOGIC;  -- Horloge système
			reset     : in  STD_LOGIC;  -- Reset asynchrone
			tx_start  : in  STD_LOGIC;  -- Signal pour démarrer la transmission
			data_in   : in  STD_LOGIC_VECTOR (19 downto 0); -- Données à transmettre (20 bits)
			tx        : out STD_LOGIC;  -- Ligne de transmission série
			tx_busy   : out STD_LOGIC   -- Indicateur de transmission en cours
		);
	END COMPONENT;
	FOR ALL : serial_async_transmitter USE ENTITY work.serial_async_transmitter;
	-- Counter reaction
	COMPONENT counter_reaction IS
		PORT (
			clk       : in  STD_LOGIC;  -- Horloge système
			reset     : in  STD_LOGIC;  -- Reset asynchrone
			start     : in  STD_LOGIC;  -- Signal pour démarrer le comptage
			stop      : in  STD_LOGIC;  -- Signal pour arrêter le comptage
			delta     : out STD_LOGIC_VECTOR (31 downto 0);  -- Temps écoulé entre start et stop
			error_cnt : out STD_LOGIC_VECTOR (31 downto 0)   -- Nombre d'erreurs
		);
	END COMPONENT;
	FOR ALL : counter_reaction USE ENTITY work.counter_reaction;
	-- Counter cycle (0...3s)
	COMPONENT counter_cycle IS
		PORT (
			clk   	: in  STD_LOGIC; -- Signal d'horloge
			reset 	: in  STD_LOGIC; -- Signal de remise à zéro
			enable	: in  STD_LOGIC; -- Permet de contrôler l'incrémentation
			count 	: out STD_LOGIC_VECTOR(31 downto 0) -- La valeur du compteur en binaire
		);
	END COMPONENT;
	FOR ALL : counter_cycle USE ENTITY work.counter_cycle;
	
   --| Constants declarations |--------------------------------------------------------------
	-- VALUES
	CONSTANT INTERFACE_ID_C					: STD_LOGIC_VECTOR(31 DOWNTO 0) := x"12345678";
	CONSTANT OTHERS_VAL_C 					: STD_LOGIC_VECTOR(31 DOWNTO 0) := x"00000000";
	-- ADDRESS (READ ONLY)
	CONSTANT ID_ADDRESS 						: INTEGER := 0;
	CONSTANT BOUTTON_ADDRESS 				: INTEGER := 1;
	CONSTANT SWITCH_ADDRESS 				: INTEGER := 2;
	CONSTANT INTERRUPT_STATUS_ADDRESS 	: INTEGER := 5;
	CONSTANT MAX10_STATUS_ADDRESS 		: INTEGER := 7;
	CONSTANT MAX10_TX_BUSY_ADDRESS		: INTEGER := 8;
	CONSTANT CNT_DELTA_ADDRESS				: INTEGER := 13;
	CONSTANT CNT_ERROR_CNT_ADDRESS		: INTEGER := 14;
	CONSTANT CNT_CYC_COUNT_ADDRESS		: INTEGER := 15;
	-- ADDRESS (WRITE ONLY)
	CONSTANT INTERRUPT_CLEAR_ADDRESS 	: INTEGER := 5;
	CONSTANT CNT_START_ADDRESS				: INTEGER := 11;
	CONSTANT CNT_STOP_ADDRESS				: INTEGER := 12;
	-- ADDRESS (READ/WRITE)
	CONSTANT LEDS_ADDRESS 					: INTEGER := 3;
	CONSTANT HEX0_4_ADDRESS 				: INTEGER := 4;
	CONSTANT MAX10_CS_ADDRESS 				: INTEGER := 9;
	CONSTANT MAX10_DATA_ADDRESS 			: INTEGER := 10;
	CONSTANT INTERRUPT_MASK_ADDRESS 		: INTEGER := 6;



	
	--| Signals declarations   |--------------------------------------------------------------
	--ID
	SIGNAL cs_r_id_s							: STD_LOGIC;
	--LEDS
	SIGNAL led_reg_s 							: STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL cs_r_leds_s						: STD_LOGIC;
	SIGNAL cs_w_leds_s						: STD_LOGIC;
	--BOUTTON
	SIGNAL boutton_reg_s 					: STD_LOGIC_VECTOR(3 DOWNTO 0);
	SIGNAL cs_r_boutton_s					: STD_LOGIC;
	--SWITCHES
	SIGNAL switches_reg_s 					: STD_LOGIC_VECTOR(9 DOWNTO 0);
	SIGNAL cs_r_switches_s					: STD_LOGIC;
	--HEX
	SIGNAL hex_0_4_reg_s 					: STD_LOGIC_VECTOR(27 DOWNTO 0);
	SIGNAL cs_r_hex_0_4_s					: STD_LOGIC;
	SIGNAL cs_w_hex_0_4_s					: STD_LOGIC;
	--AVALON
	SIGNAL readdatavalid_next_s 			: STD_LOGIC;
	SIGNAL readdatavalid_reg_s 			: STD_LOGIC;
	SIGNAL readdata_next_s 					: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL readdata_reg_s 					: STD_LOGIC_VECTOR(31 DOWNTO 0);
	--Interruption 
	SIGNAL itp_s								: STD_LOGIC;
	SIGNAL itp_src_s							: STD_LOGIC;
	--Interruption (status)
	SIGNAL itp_status_s						: STD_LOGIC_VECTOR(1 DOWNTO 0);
	signal cs_r_itp_status_s				: STD_LOGIC;
	--Interruption mask
	SIGNAL itp_mask_s							: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL cs_r_itp_mask_s					: STD_LOGIC;
	SIGNAL cs_w_itp_mask_s					: STD_LOGIC;
	--Interruption clear
	SIGNAL cs_w_itp_clear_s					: STD_LOGIC;
	--Asynchrone serial com
	SIGNAL ascom_clk_s						: STD_LOGIC;
	SIGNAL ascom_data_in_s					: STD_LOGIC_VECTOR(19 DOWNTO 0);
	SIGNAL ascom_tx_busy						: STD_LOGIC;
	--Max10
	SIGNAL max10_status_s					: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL cs_r_max10_status_s				: STD_LOGIC;
	SIGNAL max10_tx_busy_s					: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL cs_r_max10_tx_busy_s			: STD_LOGIC;
	SIGNAL max10_cs_s							: STD_LOGIC_VECTOR(3 DOWNTO 0);
	
	SIGNAL cs_w_max10_cs_s					: STD_LOGIC;
	SIGNAL cs_r_max10_cs_s					: STD_LOGIC;
	SIGNAL max10_cs_reg_s					: STD_LOGIC_VECTOR(3 DOWNTO 0);
	SIGNAL cs_w_max10_cs_reg_s				: STD_LOGIC;
	SIGNAL max10_data_s						: STD_LOGIC_VECTOR(15 DOWNTO 0);
	
	SIGNAL cs_w_max10_data_s				: STD_LOGIC;
	SIGNAL cs_r_max10_data_s				: STD_LOGIC;
	SIGNAL max10_data_reg_s					: STD_LOGIC_VECTOR(15 DOWNTO 0);
	SIGNAL cs_w_max10_data_reg_s			: STD_LOGIC;
	--Counteur
	SIGNAL cs_w_cnt_start_s					: STD_LOGIC;
	SIGNAL cs_w_cnt_stop_s					: STD_LOGIC;
	SIGNAL cnt_delta_s						: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL cs_r_cnt_delta_s					: STD_LOGIC;
	SIGNAL cnt_error_cnt_s					: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL cs_r_cnt_error_cnt_s			: STD_LOGIC;
	SIGNAL cnt_cyc_count_s					: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL cs_r_cnt_cyc_count_s			: STD_LOGIC;
	

begin	
	------------------------------------------------------
	-- INPUTS
	------------------------------------------------------
	-- Synchronis input signals
	sync_input : PROCESS (avl_reset_i,avl_clk_i)
	BEGIN
	IF avl_reset_i = '1' THEN
		boutton_reg_s						<= (OTHERS => '0');
      switches_reg_s 					<= (OTHERS => '0');
	ELSIF rising_edge(avl_clk_i) THEN
      boutton_reg_s 						<= button_i;
      switches_reg_s 					<= switch_i;
   END IF;
	END PROCESS;

	
	
	------------------------------------------------------
	-- OUTPUT
	------------------------------------------------------
	-- Signals affect
	hex0_o <= hex_0_4_reg_s(6 DOWNTO 0);
	hex1_o <= hex_0_4_reg_s(13 DOWNTO 7);
	hex2_o <= hex_0_4_reg_s(20 DOWNTO 14);
	hex3_o <= hex_0_4_reg_s(27 DOWNTO 21);
  
  
  
  	------------------------------------------------------
	-- INTERRUPT
	------------------------------------------------------
	-- Signals affect
	itp_s 			<= '1' WHEN (itp_src_s = '1' AND itp_mask_s(0) = '0') ELSE '0';
	itp_status_s	<= itp_s & itp_src_s;
	avl_irq_o		<= itp_s;
	------------------------------------------------------
	-- Read source
	read_itp_source_p : PROCESS (avl_reset_i, cs_w_itp_clear_s, boutton_reg_s(0))
	BEGIN
	IF avl_reset_i = '1' OR cs_w_itp_clear_s = '1' THEN
		itp_src_s 								<= '0';
	ELSIF falling_edge(boutton_reg_s(0)) THEN
		itp_src_s								<= '1';
	END IF;
	END PROCESS;
  
  
  
   ------------------------------------------------------
	-- MAX10
	------------------------------------------------------
	-- Signals affect
	max10_status_s 		<= con_80p_status_i;
	max10_tx_busy_s(0) 	<= ascom_tx_busy;
	------------------------------------------------------
	-- Synchronis input signals
	max10_capture : PROCESS (avl_reset_i,avl_clk_i)
	BEGIN
	IF avl_reset_i = '1' THEN
		max10_cs_reg_s							<= (OTHERS => '0');
      cs_w_max10_cs_reg_s 					<= '0';
		
		max10_data_reg_s						<= (OTHERS => '0');
		cs_w_max10_data_reg_s				<= '0';
	ELSIF rising_edge(avl_clk_i) THEN
		cs_w_max10_cs_reg_s					<= cs_w_max10_cs_s;
		cs_w_max10_data_reg_s				<= cs_w_max10_data_s;
		if cs_w_max10_cs_s = '1' then
			max10_cs_reg_s						<= max10_cs_s;
		end if;
		if cs_w_max10_data_s = '1' then
			max10_data_reg_s					<= max10_data_s;
		end if;
   END IF;
	END PROCESS;
	
	
	
  	------------------------------------------------------
	-- SERIAL ASYNCHRONE COMM
	------------------------------------------------------
	-- Signals affect
	ascom_data_in_s <= std_logic_vector(max10_cs_reg_s & max10_data_reg_s);
	------------------------------------------------------
	-- Convert data max10 to serial asynchrone transmitter
   cv_max10_to_serial : serial_async_transmitter
	PORT MAP (
        clk       	=> avl_clk_i,
        reset        => avl_reset_i,
		  tx_start		=> cs_w_max10_data_reg_s,
		  data_in		=> ascom_data_in_s,
		  tx				=> serial_data_o,
		  tx_busy		=> ascom_tx_busy
    );
  
  
  
   ------------------------------------------------------
	-- COUNTER
	------------------------------------------------------
	-- Signals affect
   cnt : counter_reaction
	PORT MAP (
      clk       		=> avl_clk_i,
      reset        	=> avl_reset_i,
		start     		=> cs_w_cnt_start_s,
		stop      		=> cs_w_cnt_stop_s,
		delta     		=> cnt_delta_s,
		error_cnt 		=> cnt_error_cnt_s
   );
   cnt_cyc : counter_cycle
	PORT MAP (
		clk   			=> avl_clk_i,
		reset 			=> avl_reset_i,
		enable			=> '1',
		count 			=> cnt_cyc_count_s
	); 
  
	------------------------------------------------------
	-- ADDRESS DECODER
	------------------------------------------------------
	-- READ
	read_address_decoder_p : PROCESS (avl_read_i,avl_address_i)
	BEGIN
   readdatavalid_next_s 		<= '0'; --valeur par defaut
	cs_r_id_s 						<= '0';
	cs_r_boutton_s 				<= '0';
	cs_r_switches_s 				<= '0';
	cs_r_leds_s 					<= '0';
	cs_r_hex_0_4_s 				<= '0';
	cs_r_itp_mask_s				<= '0';
	cs_r_itp_status_s				<= '0';
	cs_r_max10_status_s 			<= '0';
	cs_r_max10_tx_busy_s			<= '0';
	cs_r_cnt_delta_s				<= '0';
	cs_r_cnt_error_cnt_s			<= '0';
	cs_r_cnt_cyc_count_s			<= '0';
	cs_r_max10_cs_s 				<= '0';
	cs_r_max10_data_s 			<= '0';
	
	IF avl_read_i = '1' THEN
	   readdatavalid_next_s 	<= '1';
		CASE (to_integer(unsigned(avl_address_i))) IS
			WHEN ID_ADDRESS =>
				cs_r_id_s 				<= '1';
			WHEN BOUTTON_ADDRESS =>
				cs_r_boutton_s 		<= '1';
			WHEN SWITCH_ADDRESS =>
				cs_r_switches_s 		<= '1';
			WHEN LEDS_ADDRESS =>
				cs_r_leds_s 			<= '1';
			WHEN HEX0_4_ADDRESS =>
				cs_r_hex_0_4_s 		<= '1';
			WHEN INTERRUPT_MASK_ADDRESS =>
				cs_r_itp_mask_s		<= '1';
			WHEN INTERRUPT_STATUS_ADDRESS =>
				cs_r_itp_status_s		<= '1';
			WHEN MAX10_STATUS_ADDRESS =>
				cs_r_max10_status_s 	<= '1';
			WHEN MAX10_TX_BUSY_ADDRESS =>
				cs_r_max10_tx_busy_s	<= '1';
			WHEN CNT_DELTA_ADDRESS =>
				cs_r_cnt_delta_s		<= '1';
			WHEN CNT_ERROR_CNT_ADDRESS =>
				cs_r_cnt_error_cnt_s	<= '1';
			WHEN CNT_CYC_COUNT_ADDRESS =>
				cs_r_cnt_cyc_count_s	<= '1';
			WHEN MAX10_CS_ADDRESS =>
				cs_r_max10_cs_s 		<= '1';
			WHEN MAX10_DATA_ADDRESS =>
				cs_r_max10_data_s 	<= '1';
        WHEN OTHERS =>
          NULL;
		END CASE;
	END IF;
	END PROCESS;
	------------------------------------------------------
	-- WRITE
	write_address_decoder_p : PROCESS (avl_reset_i,avl_clk_i)
	BEGIN
	cs_w_leds_s 		<= '0';
	cs_w_hex_0_4_s 	<= '0';
	cs_w_itp_mask_s	<= '0';
	cs_w_itp_clear_s	<= '0';
	cs_w_max10_cs_s	<= '0';
	cs_w_max10_data_s	<= '0';
	cs_w_cnt_start_s	<= '0';
	cs_w_cnt_stop_s	<= '0';
	
	IF avl_write_i = '1' THEN
		CASE (to_integer(unsigned(avl_address_i))) IS
			WHEN LEDS_ADDRESS =>
				cs_w_leds_s 					<= '1';
			WHEN HEX0_4_ADDRESS =>
				cs_w_hex_0_4_s 				<= '1';
			WHEN INTERRUPT_MASK_ADDRESS =>
				cs_w_itp_mask_s				<= '1';
			WHEN INTERRUPT_CLEAR_ADDRESS =>
				cs_w_itp_clear_s				<= '1';
			WHEN MAX10_CS_ADDRESS =>
				cs_w_max10_cs_s				<= '1';
			WHEN MAX10_DATA_ADDRESS =>
				cs_w_max10_data_s				<= '1';
			WHEN CNT_START_ADDRESS =>
				cs_w_cnt_start_s				<= '1';
			WHEN CNT_STOP_ADDRESS =>
				cs_w_cnt_stop_s				<= '1';
			WHEN OTHERS =>
				NULL;
		END CASE;
	END IF;
	END PROCESS;
	
	
	
	------------------------------------------------------
	-- READ
	------------------------------------------------------
	readdata_next_s 	<= INTERFACE_ID_C                                					WHEN (cs_r_id_s = '1')     		ELSE
								std_logic_vector((31 downto 4 => '0') & boutton_reg_s) 		WHEN (cs_r_boutton_s = '1') 		ELSE
								std_logic_vector((31 downto 10 => '0') & switches_reg_s) 	WHEN (cs_r_switches_s = '1') 		ELSE
								std_logic_vector((31 downto 10 => '0') & led_reg_s) 			WHEN (cs_r_leds_s = '1') 			ELSE
								std_logic_vector((31 downto 28 => '0') & hex_0_4_reg_s) 		WHEN (cs_r_hex_0_4_s = '1') 		ELSE
								std_logic_vector((31 downto 2 => '0') & itp_mask_s) 			WHEN (cs_r_itp_mask_s = '1') 		ELSE
								std_logic_vector((31 downto 2 => '0') & itp_status_s) 		WHEN (cs_r_itp_status_s = '1')	ELSE
								cnt_delta_s																	WHEN (cs_r_cnt_delta_s = '1') 	ELSE
								cnt_error_cnt_s															WHEN (cs_r_cnt_error_cnt_s = '1')ELSE
								cnt_cyc_count_s															WHEN (cs_r_cnt_cyc_count_s = '1')ELSE
								std_logic_vector((31 downto 2 => '0') & max10_status_s) 		WHEN (cs_r_max10_status_s = '1')	ELSE
								std_logic_vector((31 downto 2 => '0') & max10_tx_busy_s) 	WHEN (cs_r_max10_tx_busy_s = '1')ELSE
								std_logic_vector((31 downto 16 => '0') & max10_data_reg_s)	WHEN (cs_r_max10_data_s = '1')	ELSE
								std_logic_vector((31 downto 4=> '0') & max10_cs_reg_s)		WHEN (cs_r_max10_cs_s = '1')		ELSE
								(OTHERS => '0');

								
								
	------------------------------------------------------
	-- WRITE
	------------------------------------------------------
	led_reg_s			<= avl_writedata_i(9 DOWNTO 0)			WHEN (cs_w_leds_s = '1') 			ELSE led_reg_s;
	hex_0_4_reg_s		<= avl_writedata_i(27 DOWNTO 0) 			WHEN (cs_w_hex_0_4_s = '1') 		ELSE hex_0_4_reg_s;
	itp_mask_s			<= avl_writedata_i(1 DOWNTO 0)			WHEN (cs_w_itp_mask_s = '1') 		ELSE itp_mask_s;
	max10_cs_s			<= avl_writedata_i(3 DOWNTO 0) 			WHEN (cs_w_max10_cs_s = '1') 		ELSE max10_cs_s;
	max10_data_s		<= avl_writedata_i(15 DOWNTO 0) 			WHEN (cs_w_max10_data_s = '1') 	ELSE max10_data_s;
	
	
	
	------------------------------------------------------
	-- AVALON
	------------------------------------------------------
	-- Signals affect
	avl_readdatavalid_o 						<= readdatavalid_reg_s;
	avl_readdata_o 							<= readdata_reg_s;
	------------------------------------------------------
	-- Synchronize
	read_register_p : PROCESS (avl_reset_i,avl_clk_i)
	BEGIN
	IF avl_reset_i = '1' THEN
		readdatavalid_reg_s 					<= '0';
      readdata_reg_s 						<= (OTHERS => '0');
   ELSIF rising_edge(avl_clk_i) THEN
      readdatavalid_reg_s 					<= readdatavalid_next_s;
      readdata_reg_s 						<= readdata_next_s;
   END IF;
	END PROCESS;

end rtl; 