library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity baudrate_generator is
    Port (
        clk         : in  STD_LOGIC;  -- Horloge système
        reset       : in  STD_LOGIC;  -- Reset asynchrone
        baud_pulse  : out STD_LOGIC   -- Impulsion de baudrate
    );
end baudrate_generator;

architecture comport of baudrate_generator is
    constant CLOCK_FREQ : integer := 50000000; -- Fréquence de l'horloge système (50 MHz)
    constant BAUD_RATE  : integer := 9600;     -- Vitesse de transmission (baudrate)
    constant BAUD_TICK_COUNT : integer := CLOCK_FREQ / BAUD_RATE; -- Nombre de cycles pour une impulsion

    signal counter : integer range 0 to BAUD_TICK_COUNT - 1 := 0; -- Compteur pour générer l'impulsion
    signal pulse   : STD_LOGIC := '0';
begin

    process(clk, reset)
    begin
        if reset = '1' then
            counter <= 0;
            pulse <= '0';
        elsif rising_edge(clk) then
            if counter < BAUD_TICK_COUNT - 1 then
                counter <= counter + 1;
                pulse <= '0';
            else
                counter <= 0;
                pulse <= '1'; -- Génère une impulsion
            end if;
        end if;
    end process;

    baud_pulse <= pulse;

end comport;
