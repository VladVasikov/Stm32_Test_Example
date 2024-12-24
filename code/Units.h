
#pragma once
#include <Celsius.h>
#include <Ms.h>
#include <Us.h>

inline constexpr Celsius<float> operator""_C(unsigned long long value) {
  return Celsius<float>(static_cast<float>(value));
}

inline constexpr Celsius<float> operator""_C(long double value) {
  return Celsius<float>(static_cast<float>(value));
}

static inline constexpr Us<uint32_t> operator""_Us(uint64_t value) {
  return Us{static_cast<uint32_t>(value)};
}

static inline constexpr Ms<uint32_t> operator""_Ms(uint64_t value) {
  return Ms{static_cast<uint32_t>(value)};
}