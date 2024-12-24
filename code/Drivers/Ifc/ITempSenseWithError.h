#pragma once
#include <Celsius.h>
#include <ITempSense.h>

class ITempSenseWithError
    : public m::ifc::ITempSense<std::optional<Celsius<float>>>,
      public m::ifc::ITempSenseError {
 public:
  virtual std::optional<Celsius<float>> value() = 0;

  virtual bool isShort() = 0;

  virtual bool isBreak() = 0;
};