#include <gtest/gtest.h>

#include "cpu_container.cpu"
#include <algorithm>
#include <numeric>

class CpuTestFixture : public testing::Test
{
public:
  void SetUp() override { ActiveComponent::s_components.clear(); }
};

TEST_F(CpuTestFixture, TestReadBurst)
{
  auto c = std::make_shared<cpu_container>();

  constexpr auto BASE_A = 1;
  constexpr auto BASE_B = 100000;
  constexpr auto DELAY = 10;

  std::iota(std::begin(c->DRAM_A.storage), std::end(c->DRAM_A.storage), BASE_A);
  std::iota(std::begin(c->DRAM_B.storage), std::end(c->DRAM_B.storage), BASE_B);

  c->memory_dispatcher.subcontroller_A_cmd_set.test_set_delay(DELAY);
  c->memory_dispatcher.subcontroller_A_cmd_type.test_set_delay(DELAY);
  c->memory_dispatcher.subcontroller_A_cmd_addr.test_set_delay(DELAY);

  const auto ADDRESS = 42;

  for (int i = 0; i < 50; i++)
    {
      switch (i)
        {
          case 1: {
            c->mem_controller_command_address.push(ADDRESS);
            c->mem_controller_command_type.push(DRAM_command_type::READ32);
            c->mem_controller_command_set.push(true);
            break;
          }

          default: {
            c->mem_controller_command_address.push(ADDRESS);
            c->mem_controller_command_type.push(DRAM_command_type::DES);
            c->mem_controller_command_set.push(true);
            break;
          }
        }

      tick_all_active_components();
    }

  ASSERT_TRUE(c->mem_controller_command_done.value());

  for (int i = 0; i < DRAM_STREAM_SIZE; i++)
    {
      ASSERT_EQ(c->mem_controller_command_rw.value().data[i],
                c->DRAM_B.storage[ADDRESS + i]) << "at index " << i;
    }
}

TEST_F(CpuTestFixture, TestWriteBurst)
{
  auto c = std::make_shared<cpu_container>();

  constexpr auto BASE_A = 1;
  constexpr auto BASE_B = 100000;

  std::iota(std::begin(c->DRAM_A.storage), std::end(c->DRAM_A.storage), BASE_A);
  std::iota(std::begin(c->DRAM_B.storage), std::end(c->DRAM_B.storage), BASE_B);

  const auto   ADDRESS = 42;
  MemoryPacket pkt{0, 1, 2, 3, 4, 5, 6, 7};

  for (int i = 0; i < 16; i++)
    {
      switch (i)
        {
          case 1: {
            c->mem_controller_command_address.push(ADDRESS);
            c->mem_controller_command_type.push(DRAM_command_type::WRITE32);
            c->mem_controller_command_rw.push(pkt);
            c->mem_controller_command_set.push(true);
            break;
          }
          case 3: {

          }
        }
      tick_all_active_components();
    }

  ASSERT_TRUE(c->mem_controller_command_done.value());

  for (int i = 0; i < DRAM_STREAM_SIZE; i++)
    {
      ASSERT_EQ(pkt.data[i], c->DRAM_B.storage[ADDRESS + i]);
    }
}