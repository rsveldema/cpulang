-- fpga4student.com: FPGA projects, Verilog projects, VHDL projects,
-- Generate clock enable signal instead of creating another clock domain
-- Assume that the input clock : clk_50MHz
signal clock_1KHz_enable  : std_logic;
signal counter : std_logic_vector(15 downto 0):=x"0000";
constant DIVISOR: std_logic_vector(15 downto 0):= x"C34F";
-- Generate the slow enable signal instead of creating another clock of 1KHz
--
process(clk_50MHz)
begin
  if(rising_edge(clk_50MHz)) then
    if(counter = DIVISOR) then
      counter <= x"0000";
      clock_1KHz_enable <= '1';
    else
      clock_1KHz_enable <= '0';
      counter <= counter + x"0001";
    end if;
  end if;
end process;
-- Use the same clock and the slow clock enable signal above
-- to drive another part of the design to avoid domain crossing issues
process(clk_50MHz)
begin
  if(rising_edge(clk_50MHz)) then
    if(clock_1KHz_enable = '1') then
      -- Add your logic here
      -- It will be executed like a process of 1Khz clock
    end if;
  end if;
end process;

