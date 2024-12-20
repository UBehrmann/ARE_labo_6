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

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY avl_user_interface IS
  PORT (
    -- Avalon bus
    avl_clk_i : IN STD_LOGIC;
    avl_reset_i : IN STD_LOGIC;
    avl_address_i : IN STD_LOGIC_VECTOR(13 DOWNTO 0);
    avl_byteenable_i : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    avl_write_i : IN STD_LOGIC;
    avl_writedata_i : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    avl_read_i : IN STD_LOGIC;
    avl_readdatavalid_o : OUT STD_LOGIC;
    avl_readdata_o : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    avl_waitrequest_o : OUT STD_LOGIC;
    avl_irq_o : OUT STD_LOGIC;
    -- User interface
    button_i : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    switch_i : IN STD_LOGIC_VECTOR(9 DOWNTO 0);
    led_o : OUT STD_LOGIC_VECTOR(9 DOWNTO 0);
    hex0_o : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
    hex1_o : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
    hex2_o : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
    hex3_o : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
    -- Con 80p interface
    serial_data_o : OUT STD_LOGIC;
    con_80p_status_i : IN STD_LOGIC_VECTOR(1 DOWNTO 0)
  );
END avl_user_interface;

ARCHITECTURE rtl OF avl_user_interface IS

  --| Components declaration |--------------------------------------------------------------

  COMPONENT compteur IS
    PORT (
      clock_i : IN STD_LOGIC;
      reset_i : IN STD_LOGIC;
      enable_i : IN STD_LOGIC;
      value_o : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
    );
  END COMPONENT;
  FOR ALL : compteur USE ENTITY work.compteur;

  COMPONENT timer IS
    GENERIC (
      T1_g : NATURAL RANGE 1 TO 8191 := 5208);
    PORT (
      clock_i : IN STD_LOGIC;
      reset_i : IN STD_LOGIC;
      start_i : IN STD_LOGIC;
      trigger_o : OUT STD_LOGIC
    );
  END COMPONENT;
  FOR ALL : timer USE ENTITY work.timer;

  --| Constants declarations |--------------------------------------------------------------

  CONSTANT INTERFACE_ID_C : STD_LOGIC_VECTOR(31 DOWNTO 0) := x"12345678";
  CONSTANT OTHERS_VAL_C : STD_LOGIC_VECTOR(31 DOWNTO 0) := x"00000000";
  CONSTANT ID_ADDRESS : INTEGER := 0;
  CONSTANT BOUTTON_ADDRESS : INTEGER := 1;
  CONSTANT SWITCH_ADDRESS : INTEGER := 2;
  CONSTANT LED_ADDRESS : INTEGER := 3;
  CONSTANT HEX0_4_ADDRESS : INTEGER := 4;
  CONSTANT COMPTEUR_ADDRESS : INTEGER := 5;
  CONSTANT INTERRUPT_RESET_ADDRESS : INTEGER := 6;
  CONSTANT INTERRUPT_STATUS_ADDRESS : INTEGER := 6;
  CONSTANT INTERRUPT_MASK_ADDRESS : INTEGER := 7;
  CONSTANT MAX10_DATA_ADDRESS : INTEGER := 8;
  CONSTANT MAX10_STATUS_ADDRESS : INTEGER := 9;

  --| Signals declarations   |--------------------------------------------------------------   

  SIGNAL led_reg_s : STD_LOGIC_VECTOR(9 DOWNTO 0);
  SIGNAL boutton_s : STD_LOGIC_VECTOR(3 DOWNTO 0);
  SIGNAL switches_s : STD_LOGIC_VECTOR(9 DOWNTO 0);

  SIGNAL readdatavalid_next_s : STD_LOGIC;
  SIGNAL readdatavalid_reg_s : STD_LOGIC;
  SIGNAL readdata_next_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL readdata_reg_s : STD_LOGIC_VECTOR(31 DOWNTO 0);

  SIGNAL hex0_4_s : STD_LOGIC_VECTOR(27 DOWNTO 0);
  SIGNAL reset_compteur_s : STD_LOGIC;
  SIGNAL reset_compteur_CPU_s : STD_LOGIC;
  SIGNAL enable_s : STD_LOGIC;
  SIGNAL value_s : STD_LOGIC_VECTOR(31 DOWNTO 0);

  SIGNAL max10_data_s : STD_LOGIC_VECTOR(31 DOWNTO 0);
  SIGNAL max10_ready_s : STD_LOGIC;
  SIGNAL cs_write_to_max10_s : STD_LOGIC;

  SIGNAL inter_reset_s : STD_LOGIC;
  SIGNAL inter_key0_s : STD_LOGIC;
  SIGNAL inter_s : STD_LOGIC;
  SIGNAL inter_mask_s : STD_LOGIC;

  SIGNAL button_0_s : STD_LOGIC;

  SIGNAL start_timer_s : STD_LOGIC;
  SIGNAL us_done_s : STD_LOGIC;

  SIGNAL position_to_send_s : UNSIGNED(4 DOWNTO 0);

  --| Types |----------------------------------------------------------------
  TYPE state_t IS (
    --General state
    WAIT_FOR_DATA,
    INIT_TRANSMIT,
    WRITE_BYTE,
    WAIT_FOR_NEXT_BYTE
  );
  SIGNAL e_pres, e_fut_s : state_t;

BEGIN

  -- Input signals

  -- Synchronis input signals
  sync_input : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN
    IF avl_reset_i = '1' THEN

      boutton_s <= (OTHERS => '0');
      switches_s <= (OTHERS => '0');

    ELSIF rising_edge(avl_clk_i) THEN

      boutton_s <= button_i;
      switches_s <= switch_i;

    END IF;
  END PROCESS;

  -- Output signals

  -- Hex display
  hex0_o <= hex0_4_s(6 DOWNTO 0);
  hex1_o <= hex0_4_s(13 DOWNTO 7);
  hex2_o <= hex0_4_s(20 DOWNTO 14);
  hex3_o <= hex0_4_s(27 DOWNTO 21);

  inter_s <= '1' WHEN (inter_key0_s = '1' AND inter_mask_s = '1') ELSE
    '0';

  -- Read access part
  read_decoder_p : PROCESS (
    avl_read_i,
    avl_address_i,
    boutton_s,
    switches_s,
    led_reg_s
    )
  BEGIN
    readdatavalid_next_s <= '0'; --valeur par defaut
    readdata_next_s <= (OTHERS => '0'); --valeur par defaut

    IF avl_read_i = '1' THEN
      readdatavalid_next_s <= '1';

      CASE (to_integer(unsigned(avl_address_i))) IS

        WHEN ID_ADDRESS =>
          readdata_next_s <= INTERFACE_ID_C;

        WHEN BOUTTON_ADDRESS =>
          readdata_next_s(3 DOWNTO 0) <= boutton_s;

        WHEN SWITCH_ADDRESS =>
          readdata_next_s(9 DOWNTO 0) <= switches_s;

        WHEN LED_ADDRESS =>
          readdata_next_s(9 DOWNTO 0) <= led_reg_s;

        WHEN COMPTEUR_ADDRESS =>
          readdata_next_s <= value_s;

        WHEN INTERRUPT_STATUS_ADDRESS =>
          readdata_next_s(1) <= inter_key0_s;
          readdata_next_s(0) <= inter_s;

        WHEN INTERRUPT_MASK_ADDRESS =>
          readdata_next_s(0) <= inter_mask_s;

        WHEN MAX10_STATUS_ADDRESS =>
          readdata_next_s(2 DOWNTO 1) <= con_80p_status_i;
          readdata_next_s(0) <= max10_ready_s;

        WHEN OTHERS =>
          readdata_next_s <= OTHERS_VAL_C;

      END CASE;
    END IF;
  END PROCESS;

  -- Read register process

  read_register_p : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN
    IF avl_reset_i = '1' THEN

      readdatavalid_reg_s <= '0';
      readdata_reg_s <= (OTHERS => '0');

    ELSIF rising_edge(avl_clk_i) THEN

      readdatavalid_reg_s <= readdatavalid_next_s;
      readdata_reg_s <= readdata_next_s;

    END IF;
  END PROCESS;

  -- Write access part

  write_register_p : PROCESS (
    avl_reset_i,
    avl_clk_i
    )
  BEGIN

    IF avl_reset_i = '1' THEN

      led_reg_s <= (OTHERS => '0');
      hex0_4_s <= (OTHERS => '0');
      enable_s <= '0';
      reset_compteur_CPU_s <= '0';

    ELSIF rising_edge(avl_clk_i) THEN

      reset_compteur_CPU_s <= '0';
      inter_reset_s <= '0';
      cs_write_to_max10_s <= '0';

      IF avl_write_i = '1' THEN

        CASE (to_integer(unsigned(avl_address_i))) IS

          WHEN LED_ADDRESS =>
            led_reg_s <= avl_writedata_i(9 DOWNTO 0);

          WHEN HEX0_4_ADDRESS =>
            hex0_4_s <= avl_writedata_i(27 DOWNTO 0);

          WHEN COMPTEUR_ADDRESS =>
            enable_s <= avl_writedata_i(0);
            reset_compteur_CPU_s <= avl_writedata_i(1);

          WHEN INTERRUPT_RESET_ADDRESS =>
            inter_reset_s <= '1';

          WHEN INTERRUPT_MASK_ADDRESS =>
            inter_mask_s <= avl_writedata_i(0);

          WHEN MAX10_DATA_ADDRESS =>
            IF (max10_ready_s = '1') THEN
              max10_data_s <= avl_writedata_i;
              cs_write_to_max10_s <= '1';
            END IF;

          WHEN OTHERS =>
            NULL;

        END CASE;
      END IF;
    END IF;
  END PROCESS;

  -- Compteur

  reset_compteur_s <= avl_reset_i OR reset_compteur_CPU_s;

  compteur_32 : compteur
  PORT MAP(
    clock_i => avl_clk_i,
    reset_i => reset_compteur_s,
    enable_i => enable_s,
    value_o => value_s
  );

  -- Timer

  timer_emitter : timer
  GENERIC MAP(T1_g => 50)
  PORT MAP(
    clock_i => avl_clk_i,
    reset_i => avl_reset_i,
    start_i => start_timer_s,
    trigger_o => us_done_s
  );

  -- Interrupt KEY0

  button_0_s <= boutton_s(0) XOR '1'; -- Invert button 0

  interrupt_key0 : PROCESS (
    inter_reset_s,
    button_i
    )
  BEGIN
    IF inter_reset_s = '1' THEN

      inter_key0_s <= '0';

    ELSIF rising_edge(button_0_s) THEN

      inter_key0_s <= '1';

    END IF;
  END PROCESS;

  -- Interface management

  -- State machine
  -- This process update the state of the state machine
  fsm_reg : PROCESS (avl_reset_i, avl_clk_i) IS
  BEGIN
    IF (avl_reset_i = '1') THEN
      e_pres <= WAIT_FOR_DATA;
    ELSIF (rising_edge(avl_clk_i)) THEN
      e_pres <= e_fut_s;
    END IF;
  END PROCESS fsm_reg;

  dec_fut_sort : PROCESS (
    e_pres,
    cs_write_to_max10_s
    ) IS
  BEGIN
    -- Default values for generated signal
    max10_ready_s <= '0';
    serial_data_o <= '1';
    start_timer_s <= '0';

    CASE e_pres IS
      WHEN WAIT_FOR_DATA =>
        max10_ready_s <= '1';

        IF (cs_write_to_max10_s = '1') THEN
          e_fut_s <= INIT_TRANSMIT;
        ELSE
          e_fut_s <= WAIT_FOR_DATA;
        END IF;

      WHEN INIT_TRANSMIT =>
        position_to_send_s <= to_unsigned(21, 5);

        e_fut_s <= WRITE_BYTE;

      WHEN WRITE_BYTE =>

        IF (position_to_send_s = 0) THEN
          e_fut_s <= WAIT_FOR_DATA;
        ELSE
          position_to_send_s <= position_to_send_s - 1;
          start_timer_s <= '1';
          e_fut_s <= WAIT_FOR_NEXT_BYTE;
        END IF;

      WHEN WAIT_FOR_NEXT_BYTE =>

        serial_data_o <= max10_data_s(to_integer(position_to_send_s));

        IF (us_done_s = '1') THEN
          e_fut_s <= WRITE_BYTE;
        ELSE
          e_fut_s <= WAIT_FOR_NEXT_BYTE;
        END IF;

      WHEN OTHERS =>
        e_fut_s <= WAIT_FOR_DATA;
    END CASE;
  END PROCESS dec_fut_sort;

END rtl;