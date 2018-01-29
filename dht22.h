#ifndef DHT22_H_
#define DHT22_H_

#include <stdint.h>
#include <avr/io.h>

#define DHT22_DDRD DDRB
#define DHT22_PORT PORTB
#define DHT22_PIN PINB

#define DHT22_ERROR_CHECKSUM -3
#define DHT22_ERROR_NULL -2
#define DHT22_ERROR_NORESPONSE -1
#define DHT22_ERROR 0
#define DHT22_OK 1

// For 16MHz, this is ~30us for a while(read pin) count++;
#define DHT22_MAGIC_ONE 4

typedef struct
{
	uint8_t pin;
	uint8_t data[5]; // 16bit RH + 16bit T + 8bit checksum
} dht22;

void dht22_init(dht22 *sensor, const uint8_t pin);

uint8_t dht22_get_bytes(dht22 *sensor);

uint8_t dht22_read_data(dht22 *sensor, float *temp, float *hum);

#endif /* DHT22_H_ */
