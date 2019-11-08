# Water level sensors

## I am using something like this:
![water-level-sensor-white](../../assets/docs/img/water-level-sensor-white.jpg)
![water-level-sensor-black](../../assets/docs/img/water-level-sensor-black.jpg)

## Connecting
Common cable connecting all switches
we connect to a common ground. Sensor outputs
connect to the Arduino inputs specified in the file
`include/control/waterLevelSensor/devices.hpp`.

## Testing
To test the correctness of connections, enter into
`Main menu -> Settings -> Service mode -> Sensors`
and then manually turn on each sensor and check that
it corresponds to the corresponding sensor in the controller.