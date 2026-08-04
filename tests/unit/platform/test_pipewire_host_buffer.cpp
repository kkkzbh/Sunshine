/**
 * @file tests/unit/platform/test_pipewire_host_buffer.cpp
 * @brief Test lifetime-safe PipeWire host buffers.
 */
#include "../../tests_common.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <span>
#include <src/platform/linux/pipewire_host_buffer.h>

TEST(PipeWireHostBufferTests, ReusesExclusiveBuffer) {
  auto buffer = std::make_shared<pipewire::host_buffer_t>();
  auto *original = buffer.get();
  auto source = std::array<std::uint8_t, 4> {1, 2, 3, 4};

  buffer = pipewire::write_host_buffer(std::move(buffer), source);

  EXPECT_EQ(buffer.get(), original);
  EXPECT_EQ(*buffer, pipewire::host_buffer_t(source.begin(), source.end()));
}

TEST(PipeWireHostBufferTests, PreservesRetainedFrame) {
  auto first = std::array<std::uint8_t, 3> {1, 2, 3};
  auto second = std::array<std::uint8_t, 2> {8, 9};
  auto buffer = pipewire::write_host_buffer({}, first);
  auto retained = buffer;

  buffer = pipewire::write_host_buffer(std::move(buffer), second);

  EXPECT_NE(buffer.get(), retained.get());
  EXPECT_EQ(*retained, pipewire::host_buffer_t(first.begin(), first.end()));
  EXPECT_EQ(*buffer, pipewire::host_buffer_t(second.begin(), second.end()));
}

TEST(PipeWireHostBufferTests, CreatesZeroedValidationFrame) {
  auto buffer = pipewire::make_zeroed_host_buffer(16);

  ASSERT_EQ(buffer->size(), 16);
  EXPECT_TRUE(std::ranges::all_of(*buffer, [](auto value) {
    return value == 0;
  }));
}
