#pragma once
#include <IIt.hpp>

#include "tim.h"

inline const std::function<void()>* Tim_6_10Hz_cb_ = nullptr;

class Tim_6_10Hz final : public m::ifc::mcu::IIt {
 public:
  Tim_6_10Hz(const std::function<void()>& cb) : IIt(cb) {
    Tim_6_10Hz_cb_ = &cb;
    htim6.PeriodElapsedCallback = [](TIM_HandleTypeDef* htim) {
      (*Tim_6_10Hz_cb_)();
    };
  }

  bool start() override {
    if (running_) return false;
    if (HAL_TIM_Base_Start_IT(&htim6) == HAL_OK) {
      running_ = true;
      return true;
    } else {
      return false;
    }
  }

  bool running() override { return running_; }

  bool stop() override {
    if (!running_) return false;
    if (HAL_TIM_Base_Stop_IT(&htim6) == HAL_OK) {
      running_ = false;
      return true;
    } else {
      return false;
    }
  }

 private:
  bool running_ = false;
};
