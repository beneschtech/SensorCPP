//
// Created by cbenesch on 5/31/26.
//

#ifndef SENSORCPP_AHT10_H
#define SENSORCPP_AHT10_H

#include <i2cdevice.h>
namespace I2C {
  class AHT10 {
  public:
    AHT10();

    explicit AHT10(const std::string& i2cBus,unsigned int address = 0x38);
    ~AHT10() = default;
    [[nodiscard]] std::string errorString() const;
    [[nodiscard]] bool isConnected() const;
    [[nodiscard]] bool valid() const;
    [[nodiscard]] bool open();
    bool open(const std::string& i2cBus,unsigned int address = 0x38);
    [[nodiscard]] bool initialize() const;
    bool trigger_measurement();
    [[nodiscard]] float humidity() const { return humidity_; }
    [[nodiscard]] float temperatureC() const { return temperatureC_; }
    [[nodiscard]] float temperatureF() const { return (temperatureC_ * 9.0f / 5.0f) + 32.0f; }
    [[nodiscard]] unsigned char status() const { return status_; }

  private:
    I2CDevice i2cDevice_;
    std::string errorString_;
    float humidity_;
    float temperatureC_;
    unsigned char status_;
  };

} // I2C

#endif //SENSORCPP_AHT10_H
