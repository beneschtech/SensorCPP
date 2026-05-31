//
// Created by cbenesch on 5/31/26.
//
#include <iostream>
#include <json/json.h>
#include <sensors/AHT10.h>

int main(int argc, char* argv[]) {
  unsigned int addr = 0x38;
  std::string i2cdev = "/dev/i2c-0";

  if (argc >= 2) {
    i2cdev = argv[1];
  }
  if (argc == 3) {
    addr = std::stoul(argv[2], nullptr, 16);
  }
  I2C::AHT10 sensor(i2cdev, addr);
  if (!sensor.open()) {
    std::cerr << "AHT10 sensor on " << i2cdev << " at address 0x" << std::hex << addr << "\n";
    std::cerr << sensor.errorString() << std::endl;
    return 1;
  }
  if (!sensor.initialize()) {
    std::cerr << "AHT10 sensor on " << i2cdev << " at address 0x" << std::hex << addr << "\n";
    std::cerr << sensor.errorString() << std::endl;
    return 1;
  }
  if (!sensor.trigger_measurement()) {
    std::cerr << "AHT10 sensor on " << i2cdev << " at address 0x" << std::hex << addr << "\n";
    std::cerr << sensor.errorString() << std::endl;
    return 1;
  }

  Json::Value json;
  json["temperatureC"] = sensor.temperatureC();
  json["humidity"] = sensor.humidity();
  json["temperatureF"] = sensor.temperatureF();
  json["status"] = sensor.status();
  std::cout << json << std::endl;
  return 0;
}