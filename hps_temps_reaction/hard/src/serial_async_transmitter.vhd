LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

ENTITY serial_async_transmitter IS
    PORT (
        clk      : IN  STD_LOGIC;                                -- Horloge système
        reset    : IN  STD_LOGIC;                                -- Reset asynchrone
        tx_start : IN  STD_LOGIC;                                -- Signal pour démarrer la transmission
        data_in  : IN  STD_LOGIC_VECTOR (19 DOWNTO 0);           -- Données à transmettre (20 bits)
        tx       : OUT STD_LOGIC;                                -- Ligne de transmission série
        tx_busy  : OUT STD_LOGIC                                 -- Indicateur de transmission en cours
    );
END serial_async_transmitter;

ARCHITECTURE comport OF serial_async_transmitter IS
    CONSTANT CLOCK_FREQ       : INTEGER := 50000000; -- Fréquence de l'horloge système (50 MHz)
    CONSTANT BAUD_RATE        : INTEGER := 9600;     -- Vitesse de transmission (baudrate)
    CONSTANT BAUD_TICK_COUNT  : INTEGER := CLOCK_FREQ / BAUD_RATE; -- Nombre de cycles pour une impulsion

    SIGNAL clock_count        : INTEGER RANGE 0 TO BAUD_TICK_COUNT - 1 := 0;
    SIGNAL bit_count          : INTEGER RANGE 0 TO 22 := 0; -- Start bit + 20 data bits + Stop bit
    SIGNAL shift_register     : STD_LOGIC_VECTOR(21 DOWNTO 0) := (OTHERS => '1'); -- Données à transmettre avec start et stop bits
    SIGNAL tx_reg             : STD_LOGIC := '1'; -- Ligne de transmission
    SIGNAL busy               : STD_LOGIC := '0'; -- Transmission en cours
BEGIN

    PROCESS (clk, reset)
    BEGIN
        IF reset = '1' THEN
            clock_count     <= 0;
            bit_count       <= 0;
            shift_register  <= (OTHERS => '1');
            tx_reg          <= '1';
            busy            <= '0';
        ELSIF rising_edge(clk) THEN
            IF busy = '0' THEN
                IF tx_start = '1' THEN
                    busy <= '1';
                    -- Charger les données avec bits de start et stop
                    shift_register <= '0' & data_in & '1'; -- Start bit, données, Stop bit
                    bit_count <= 0;
                END IF;
            ELSE
                IF clock_count = BAUD_TICK_COUNT - 1 THEN
                    clock_count <= 0;
                    -- Transmission des bits
                    IF bit_count <= 21 THEN
                        tx_reg <= shift_register(21);          -- Transmettre le bit MSB
                        shift_register <= shift_register(20 DOWNTO 0) & '1'; -- Décalage à droite
                        bit_count <= bit_count + 1;
                    ELSE
                        busy <= '0'; -- Fin de transmission
                    END IF;
                ELSE
                    clock_count <= clock_count + 1;
                END IF;
            END IF;
        END IF;
    END PROCESS;

    -- Sorties
    tx      <= tx_reg;
    tx_busy <= busy;

END comport;
