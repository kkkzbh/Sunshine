/**
 * @file tests/unit/platform/test_linux_input.cpp
 * @brief Test Linux input capability advertisement.
 */
#if defined(__linux__)
  #include <src/platform/linux/input/inputtino_common.h>
#endif

#include "../../tests_common.h"

#if defined(__linux__)

TEST(LinuxInputCapabilitiesTest, NativePenTouchFollowsConfiguration) {
  auto original_native_pen_touch = config::input.native_pen_touch;
  auto original_gamepad = config::input.gamepad;
  config::input.gamepad = "x360";

  config::input.native_pen_touch = false;
  EXPECT_EQ(platf::get_capabilities() & platf::platform_caps::pen_touch, 0u);

  config::input.native_pen_touch = true;
  EXPECT_EQ(platf::get_capabilities() & platf::platform_caps::pen_touch, platf::platform_caps::pen_touch);

  config::input.native_pen_touch = original_native_pen_touch;
  config::input.gamepad = original_gamepad;
}

TEST(LinuxInputCapabilitiesTest, DisabledNativePenTouchCreatesNoNativeDevices) {
  auto original_native_pen_touch = config::input.native_pen_touch;
  config::input.native_pen_touch = false;
  auto input = platf::input();
  auto context = platf::allocate_client_input_context(input);
  auto *raw_context = static_cast<platf::client_input_raw_t *>(context.get());

  EXPECT_FALSE(raw_context->touch);
  EXPECT_FALSE(raw_context->pen);
  config::input.native_pen_touch = original_native_pen_touch;
}
#endif
