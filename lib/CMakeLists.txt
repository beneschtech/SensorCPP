add_library(SensorCPP SHARED
    ${CMAKE_SOURCE_DIR}/include/i2cdevice.h
    ${CMAKE_SOURCE_DIR}/include/i2cdevice.h
    i2cdevice.cpp
    I2C_AHT10.cpp
        util.cpp
        util.h
)
target_link_libraries(SensorCPP PRIVATE ${SMBUS_LIBRARIES})