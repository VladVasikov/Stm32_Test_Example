#pragma once
#include <IAdcDmaCircularReader.h>

#include "adc.h"
#include "tim.h"

inline const std::function<void(std::span<volatile uint16_t>)>
    *Adc_1_half_conv_cb_ = nullptr;
inline const std::function<void(std::span<volatile uint16_t>)> *Adc_1_conv_cb_ =
    nullptr;

inline std::span<volatile uint16_t> Adc_1_data_;

class Adc_1 final : public m::ifc::mcu::IAdcDmaCircularReader<uint16_t> {
 public:
  Adc_1(const std::function<void(std::span<volatile uint16_t> first_half)>
            &half_conv_cb,
        const std::function<void(std::span<volatile uint16_t> second_half)>
            &conv_cb)
      : IAdcDmaCircularReader(half_conv_cb, conv_cb) {
    Adc_1_half_conv_cb_ = &half_conv_cb;
    Adc_1_conv_cb_ = &conv_cb;

    hadc1.ConvHalfCpltCallback = [](ADC_HandleTypeDef *hadc) {
      (*Adc_1_half_conv_cb_)(Adc_1_data_.first(Adc_1_data_.size() / 2));
    };
    hadc1.ConvCpltCallback = [](ADC_HandleTypeDef *hadc) {
      (*Adc_1_conv_cb_)(
          Adc_1_data_.last(Adc_1_data_.size() - Adc_1_data_.size() / 2));
    };
  }

  bool start(std::span<volatile uint16_t> data) override {
    if (running_) return false;

    Adc_1_data_ = data;

    if (!HAL_ADC_Start_DMA(
            &hadc1,
            reinterpret_cast<uint32_t *>(const_cast<uint16_t *>(data.data())),
            data.size()) == HAL_OK)
      return false;
    if (!HAL_TIM_Base_Start(&htim8) == HAL_OK) return false;

    running_ = true;

    return true;
  }

  bool running() override { return running_; }

  bool stop() override {
    if (!running_) return false;
    if (!HAL_ADC_Stop_DMA(&hadc1) == HAL_OK) return false;
    if (!HAL_TIM_Base_Stop(&htim8) == HAL_OK) return false;

    running_ = false;

    return true;
  }

 private:
  bool running_ = false;
};
