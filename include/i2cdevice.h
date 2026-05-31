//
// Created by cbenesch on 5/31/26.
//

#ifndef SENSORCPP_I2CDEVICE_H
#define SENSORCPP_I2CDEVICE_H

#include <string>
#include <sys/ioctl.h>
#include <fcntl.h>

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

/**
 * @class I2CDevice
 * @brief Represents an I2C (Inter-Integrated Circuit) device and provides functionality for communication with it.
 *
 * @AI
 * This class provides methods to manage and communicate with an I2C device
 * through its file descriptor, device path, and address. It also handles error states and
 * provides relevant error information for debugging purposes.
 */
class I2CDevice {
public:
  /**
   * Default constructor, initializes device with default values
   * and allows integrating devices into container classes, ie map/vector/etc..
   */
  I2CDevice();

  /**
   * Constructor with device path and address, calls default constructor
   * @param devicePath The path to the I2C device file
   * @param deviceAddress The 7-bit I2C device address
   */
  I2CDevice(const std::string& devicePath, unsigned int deviceAddress);

  /**
   * Destructor, closes the I2C device file descriptor
   * inherited classes should explicitly call this
   * to ensure proper cleanup
   */
  ~I2CDevice();

  /**
   * Open the I2C device file descriptor and set the address
   * via ioctl. Sets valid, error, and errorString if an error occurs
   * @return whether the device was successfully opened
   */
  bool open();

  /**
   * @return if this object had an error during initialization or operation
   */
  [[nodiscard]] bool error() const { return error_; }

  /**
   * @return a string describing the last error that occurred
   */
  [[nodiscard]] std::string errorString() const { return errorString_; }

  /**
   * access to the integer file descriptor
   * @return -1 if invalid, or the file descriptor
   */
  [[nodiscard]] int fd() const { return fd_; }

  /**
   * Sets the path to the I2C device file, does not attempt to open the device
   * @param devicePath the path to the I2C device file
   */
  void setDevicePath(const std::string& devicePath) { devicePath_ = devicePath; }

  /**
   * Sets the device address, does not attempt to open the device
   * @param deviceAddress the 7-bit I2C device address
   */
  void setDeviceAddress(const unsigned int deviceAddress) { deviceAddress_ = deviceAddress; }

  /**
   * @return the device path if set, a blank string if not
   */
  [[nodiscard]] std::string devicePath() const { return devicePath_; }

  /**
   * @return the device address if set, 0 if not
   */
  [[nodiscard]] unsigned int deviceAddress() const { return deviceAddress_; }

  /**
   * @return if this object is valid, meaning it has a valid device path and address
   * and has been opened successfully
   */
  [[nodiscard]] bool isValid() const { return isValid_; }

private:
  bool isValid_;
  std::string devicePath_;
  unsigned int deviceAddress_;
  bool error_;
  std::string errorString_;
  int fd_;
};

#endif //SENSORCPP_I2CDEVICE_H
