# panahon-iot
An IoT project that runs on ESP8266 which sends Temperature &amp; Humidity data via HTTP

# Hardware
1. ESP8266 (ESP-12)
2. DHT22 (Temperature & Humidity Sensor)

# Toolchain
Firmware was written using ESP8266 in Arduino (https://github.com/esp8266/Arduino)

# Pinout for uploading firmware
1. VCC   -> 3.3V (External power source)
2. GND   -> GND  (External power source)
3. CH_PD -> 3.3V (External power source)
4. GPIO0 -> GND  (External power source)
5. TXD   -> RXD  (USB TTL Serial)
6. RXD   -> TXD  (USB TTL Serial)
* USB TTL Serial GND must also be connected to the external power source's GND

# Pinout for normal operation
1. VCC   -> 3.3V (External power source)
2. GND   -> GND  (External power source)
3. CH_PD -> 3.3V (External power source)
4. GPIO2 -> Pin4 of DHT22
* DHT22 Pin1 to 3.3V and Pin2 to GND

# Panahon IoT Sever runs on NodeJS & MongoDB
See the project at https://github.com/armanvp/panahon
