

#pragma once

#include <IIt.hpp>
#include "Pin_F4_Wrapper.hpp"

struct Packet{
uint8_t n;
uint16_t freq; // 100Hz - 20'000 Hz
};
# define SIZE_BUF 6000
inline const std::function<void()>* Generator_cb_ = nullptr;

class Generator{
 public: 
	Generator(TIM_HandleTypeDef *htim){	
		htim_ = htim;    
    Generator_cb_ = &cb_;    
    htim->PeriodElapsedCallback = [](TIM_HandleTypeDef* htim) {
      if (*Generator_cb_) (*Generator_cb_)();
    };	
	 }
   
  void setCallback(std::function<void()>&& cb) { cb_ = std::move(cb); }

  void togl(){   
/*
    if((cnt_imp ) >= signal[index_signal].n){      
       cnt_imp = 0;
       index_signal++;  
       htim_->Instance->CCR1 = (1000000/signal[index_signal].freq) / 2;
       htim_->Instance->ARR = 1000000/signal[index_signal].freq; 
       htim_->Instance->PSC =168;
     }
     if(index_signal >= size_signal){
      stop();
     }
     cnt_imp++;*/
  } 

  bool start() {
      if (running_) return false;   
      htim_->Instance->CCR1 = setbuf[2];
      htim_->Instance->ARR = setbuf[0]; 
      htim_->Instance->PSC =1680;
      if (HAL_TIM_PWM_Start(htim_, TIM_CHANNEL_1)==HAL_OK){
          if(HAL_TIM_DMABurst_MultiWriteStart(htim_, TIM_DMABASE_ARR, TIM_DMA_UPDATE, (uint32_t*)&setbuf[3], TIM_DMABURSTLENGTH_3TRANSFERS, cnt_step-3)==HAL_OK){
              running_ = true;
              return true;
          }else{
            HAL_TIM_PWM_Stop(htim_, TIM_CHANNEL_1);
            running_ = false;
            return false;
          }
      } else{
        running_ = false;
        return false;
      }
  }

  bool stop() {
    if (!running_) return false;
    if (HAL_TIM_PWM_Stop(htim_, TIM_CHANNEL_1) == HAL_OK) { 
      running_ = false;
      if (HAL_TIM_DMABurst_ReadStop(htim_, TIM_DMA_UPDATE)==HAL_OK){
         running_ = false;
         return true;
      } else{
        return false;
      }         
    } else {
      return false;
    }
  }

  void add(std::span<Packet> data){
    uint8_t i, j;    
    cnt_step=0;
    for(i=0; i<data.size(); i++){       
       signal[i] = data[i];
       for(j=0; j<signal[i].n; j++ ){          
          if (cnt_step > SIZE_BUF - 3){return;}
          setbuf[cnt_step] = 100000/signal[i].freq;           
          cnt_step++;
          setbuf[cnt_step] = 0;         
          cnt_step++;
          setbuf[cnt_step] = (100000/signal[i].freq) / 2;
          cnt_step++;
          
       }
    }

   
  }  
 
   
 private:  
	bool running_ = false;	
	TIM_HandleTypeDef *htim_;
  std::array <Packet,10> signal;      
  uint8_t  index_signal, cnt_imp, size_signal;
  PinWrapper *pinOut_;
  std::function<void()> cb_;
  uint16_t setbuf[SIZE_BUF];
  uint16_t cnt_step = 0;
};

