#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#include <vector>

#ifdef USE_ESP32

namespace esphome {
namespace xiaomi_miscale {

struct ParseResult {
  int version;
  optional<float> weight;
  optional<int> heart_rate;
  optional<float> impedance;
  //optional<float> impedance_250;
};

class XiaomiMiscale : public Component, public esp32_ble_tracker::ESPBTDeviceListener {
 public:
  void set_address(uint64_t address) { address_ = address; };

  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void set_weight(sensor::Sensor *weight) { weight_ = weight; }
  void set_heart_rate(sensor::Sensor *heart_rate) { heart_rate_ = heart_rate; }
  void set_impedance(sensor::Sensor *impedance) { impedance_ = impedance; }
  void set_clear_impedance(bool clear_impedance) { clear_impedance_ = clear_impedance; }
  //void set_impedance_250(sensor::Sensor *impedance_250) { impedance_250_ = impedance_250; }
  //void set_clear_impedance_250(bool clear_impedance_250) { clear_impedance_250_ = clear_impedance_250; }

 protected:
  uint64_t address_;
  sensor::Sensor *weight_{nullptr};
  sensor::Sensor *heart_rate_{nullptr};
  sensor::Sensor *impedance_{nullptr};
  //sensor::Sensor *impedance_250_{nullptr};
  bool clear_impedance_{false};
  //bool clear_impedance_250_{false};

  optional<ParseResult> parse_header_(const esp32_ble_tracker::ServiceData &service_data);
  bool parse_message_(const std::vector<uint8_t> &message, ParseResult &result);
  bool parse_message_v1_(const std::vector<uint8_t> &message, ParseResult &result);
  bool parse_message_v2_(const std::vector<uint8_t> &message, ParseResult &result);
  bool parse_message_s400_(const std::vector<uint8_t> &message, ParseResult &result);
  bool report_results_(const optional<ParseResult> &result, const std::string &address);
};

}  // namespace xiaomi_miscale
}  // namespace esphome

#endif
