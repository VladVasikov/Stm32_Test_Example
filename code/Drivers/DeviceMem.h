#pragma once

#include <IMemory.h>
#include <PY25Q128HA.h>

class DeviceMem final : public m::ifc::IMemory {
 public:
  DeviceMem(m::ic::PY25Q128HA<uint32_t>& ic) : ic_(ic) {}

  std::size_t size() override { return ic_.size(); }

  bool write(std::size_t addr, std::span<uint8_t const> data) override {
    return ic_.write(addr, data);
  }

  bool read(std::size_t addr, std::span<uint8_t> data) override {
    return ic_.read(addr, data);
  }

 private:
  m::ic::PY25Q128HA<uint32_t>& ic_;
};