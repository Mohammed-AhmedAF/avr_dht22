# avr_dht22
Small lib to interface with DHT22 sensor with AVR Atmega

##

## Example
```c
float temp = 0.0f;
float hum = 0.0f;
uint8_t error_code = 0;

dht22 sensor;
dht22_init(&sensor, PINB0);

error_code = dht22_read_data(&sensor, &temp, &hum);
  if ( error_code < 1 ) {
    // Handle error
  } else {
   // Sensor data in temp and hum
  }
```
