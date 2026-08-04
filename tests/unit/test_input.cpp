/**
 * @file tests/unit/test_input.cpp
 * @brief Tests for session keyboard modifier reconciliation.
 */

#include "../tests_common.h"

#include <src/input.h>

namespace {
  constexpr std::uint8_t modifier_shift = 0x01;
  constexpr std::uint8_t modifier_ctrl = 0x02;
  constexpr std::uint8_t modifier_alt = 0x04;
}  // namespace

TEST(InputModifierTest, LostAltReleaseIsReconciledBeforeCtrlSpace) {
  auto delta = input::modifier_delta(modifier_alt, modifier_ctrl);

  EXPECT_EQ(delta.release, modifier_alt);
  EXPECT_EQ(delta.press, modifier_ctrl);
}

TEST(InputModifierTest, NextNeutralPacketReleasesReconciledControl) {
  auto delta = input::modifier_delta(modifier_ctrl, 0);

  EXPECT_EQ(delta.release, modifier_ctrl);
  EXPECT_EQ(delta.press, 0);
}

TEST(InputModifierTest, MatchingModifierMaskProducesNoEvents) {
  auto desired = static_cast<std::uint8_t>(modifier_shift | modifier_ctrl);
  auto delta = input::modifier_delta(desired, desired);

  EXPECT_EQ(delta.release, 0);
  EXPECT_EQ(delta.press, 0);
}
