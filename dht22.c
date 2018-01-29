#include "dht22.h"
#include <util/delay.h>

void dht22_init(dht22 *sensor, const uint8_t pin)
{
	if (sensor)
	{
		sensor->pin = pin;
		DHT22_DDRD |= (1<<sensor->pin); // First it is output
		DHT22_PORT |= (1<<sensor->pin); // Give it high
		_delay_ms(1000); // Wait for the sensor
	}
}

uint8_t dht22_get_bytes(dht22 *sensor)
{
	if (!sensor)
		return DHT22_ERROR_NULL;
	
	uint8_t i = 0, mask = 128, count = 0, byte = 0, sum = 0;
	
	for(i = 5; i; --i)
		sensor->data[i-1] = 0;
		
	// Start signal
	DHT22_DDRD |= (1<<sensor->pin); // Output pin
	DHT22_PORT |= (1<<sensor->pin); // High
	_delay_ms(1000);
	DHT22_PORT &= ~(1<<sensor->pin); // Low
	_delay_ms(1);
	DHT22_PORT |= (1<<sensor->pin); // High
	_delay_us(40);
	DHT22_DDRD &= ~(1<<sensor->pin); // Input pin
	// Response
	while ( !(DHT22_PIN & (1<<sensor->pin)) );
	while ( (DHT22_PIN & (1<<sensor->pin)) );
	// Begin data transmit
	for (i = 40; i; --i)
	{
		// Wait for incoming bit
		while ( !(DHT22_PIN & (1<<sensor->pin)) );
		// Measure bit length
		while ((DHT22_PIN & (1<<sensor->pin)))
			++count;
		// Put it into data
		if ( count > DHT22_MAGIC_ONE ) // magic
			sensor->data[byte] |= mask;
		
		mask = mask >> 1;
		if (mask == 0)
		{
			mask = 128;
			byte++;
		}
		count = 0;
	}
	
	// Checksum
	for (i = 4; i; i--)
		sum += sensor->data[i-1];
	if ( (sum >> 8) != sensor->data[4] )
		return DHT22_ERROR_CHECKSUM;

	// No Humidity error
	if (sensor->data[0])
		return DHT22_OK;
	
	return DHT22_ERROR;
}

uint8_t dht22_read_data(dht22 *sensor, float *temp, float *hum)
{
	uint8_t error_code =DHT22_ERROR;
	if (!sensor)
		return DHT22_ERROR_NULL;
	error_code = dht22_get_bytes(sensor);
	if (error_code < 1)
		return error_code;
		
	*hum = (float)(sensor->data[0] << 8 | sensor->data[1]);
	*hum = (float)((*hum)/10);
	
	*temp = (float)(sensor->data[2] << 8 | sensor->data[3]);
	*temp = (float)((*temp)/10);
	
	return DHT22_OK;	
}
