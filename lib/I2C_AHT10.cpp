//
// Created by cbenesch on 5/31/26.
//

#include <cmath>
#include <string.h>
#include <unistd.h>
#include <sensors/AHT10.h>

#include "util.h"

static unsigned char AHT10_INIT_CMD = 0b11100001;
static unsigned char AHT10_TRIGMEAS_CMD = 0b10101100;
static unsigned char AHT10_READ_VALUE = 0b110011;

namespace I2C {
  AHT10::AHT10() {
    errorString_.clear();
    i2cDevice_ = I2CDevice();
    humidity_ = NAN;
    temperatureC_ = NAN;
    status_ = 0xff;
  }

  AHT10::AHT10(const std::string& i2cBus,unsigned int address) :
    AHT10()
  {
    i2cDevice_.setDevicePath(i2cBus);
    i2cDevice_.setDeviceAddress(address);
  }

  std::string AHT10::errorString() const {
    if (!errorString_.empty())
      return errorString_;
    return i2cDevice_.errorString();
  }

  bool AHT10::valid() const {
    if (errorString_.empty())
      return i2cDevice_.isValid();
    return false;
  }

  bool AHT10::open() {
    if (!i2cDevice_.open())
      return false;
    return true;
  }

  bool AHT10::open(const std::string &i2cBus, const unsigned int address) {
    i2cDevice_.setDevicePath(i2cBus);
    i2cDevice_.setDeviceAddress(address);
    return this->open();
  }

  bool AHT10::isConnected() const {
    return (i2cDevice_.fd() >= 0);
  }

  bool AHT10::initialize() const {
    const int fd = i2cDevice_.fd();
    if (fd < 0)
      return false;
    if (i2c_smbus_write_byte(fd, AHT10_INIT_CMD) == -1)
      return false;
    usleep(50000);
    return true;
  }

  bool AHT10::trigger_measurement() {
    const int fd = i2cDevice_.fd();
    if (fd < 0)
      return false;
    errno = 0;
    if (i2c_smbus_write_word_data(fd,AHT10_TRIGMEAS_CMD,AHT10_READ_VALUE) == -1) {
      errorString_ = handleError(errno, "Failed to trigger measurement");
      return false;
    }
    usleep(100000);
    unsigned char data[6] = { 0, };
    errno = 0;
    ssize_t nread = ::read(fd, data, sizeof(data));
    if (nread != sizeof(data)) {
      errorString_ = handleError(errno,"Failed to read measurement data from device");
      return false;
    }
    status_ = data[0];
    unsigned int humidval = (data[1] << 12) | (data[2] << 4) | (data[3] >> 4);
    unsigned int tempval = ((data[3] & 0xf) << 16) | (data[4] << 8) | data[5];
    humidity_ = 0;
    humidity_ += humidval;
    humidity_ /= (1 << 20);
    humidity_ *= 100.0f;
    temperatureC_ = 0;
    temperatureC_ += tempval;
    temperatureC_ /= (1 << 20);
    temperatureC_ *= 200.0f;
    temperatureC_ -= 50.0f;
    return true;
  }
} // I2C