/**
 * @file src/platform/linux/pipewire_host_buffer.h
 * @brief Lifetime-safe host buffers for PipeWire capture frames.
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

namespace pipewire {
  /**
   * @brief Host-visible pixels owned by one or more capture images.
   */
  using host_buffer_t = std::vector<std::uint8_t>;

  /**
   * @brief Shared ownership handle for host-visible capture pixels.
   */
  using host_buffer_ptr = std::shared_ptr<host_buffer_t>;

  /**
   * @brief Copy a PipeWire frame into an exclusively writable host buffer.
   *
   * @details Reuses the supplied allocation when no captured image still owns
   *          it. A new allocation is created while an encoder retains the old
   *          frame, preserving that frame's lifetime without an extra copy.
   *
   * @param buffer Recyclable buffer from the capture pipeline.
   * @param source Frame bytes supplied by PipeWire.
   * @return Buffer containing an independent copy of `source`.
   */
  auto inline write_host_buffer(host_buffer_ptr buffer, std::span<const std::uint8_t> source) -> host_buffer_ptr {
    if (!buffer || !buffer.unique()) {
      buffer = std::make_shared<host_buffer_t>();
    }
    buffer->assign(source.begin(), source.end());
    return buffer;
  }

  /**
   * @brief Allocate an all-zero host frame for encoder validation.
   *
   * @param size Number of bytes in the frame.
   * @return Owned zero-filled frame storage.
   */
  auto inline make_zeroed_host_buffer(std::size_t size) -> host_buffer_ptr {
    return std::make_shared<host_buffer_t>(size, std::uint8_t {0});
  }
}  // namespace pipewire
