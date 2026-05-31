//
// Created by cbenesch on 5/31/26.
//
#include <i2cdevice.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>

I2CDevice::I2CDevice() {
  devicePath_.clear();
  deviceAddress_ = 0;
  error_ = false;
  errorString_.clear();
  isValid_ = false;
  fd_ = -1;
}

I2CDevice::I2CDevice(const std::string& devicePath, unsigned int deviceAddress):
  I2CDevice()
{
  devicePath_ = devicePath;
  deviceAddress_ = deviceAddress;
}

I2CDevice::~I2CDevice() {
  if (fd_ >= 0) {
    ::close(fd_);
  }
}

bool I2CDevice::open() {
  struct stat s{};
  if (stat(devicePath_.c_str(), &s) < 0) {
    error_ = true;
    errorString_ = std::string(std::strerror(errno));
    isValid_ = false;
    return false;
  }
  fd_ = ::open(devicePath_.c_str(), O_RDWR);
  if (fd_ < 0) {
    error_ = true;
    errorString_ = std::string(std::strerror(errno));
    isValid_ = false;
    return false;
  }
  if (ioctl(fd_, I2C_SLAVE, deviceAddress_) < 0) {
    error_ = true;
    errorString_ = std::string(std::strerror(errno));
    ::close(fd_);
    fd_ = -1;
    isValid_ = false;
    return false;
  }
  isValid_ = true;
  return true;
}
