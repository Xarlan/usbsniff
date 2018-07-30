/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/spi_master.h"

#include "usbconf.h"
#include "max3421e.h"


void send_msg_spi(spi_device_handle_t hwnd_spi, const uint8_t data);
void send_msg_spi2(spi_device_handle_t hwnd_spi, const uint8_t *data, int len_tx_msg);

void rxtx_spi(spi_device_handle_t hwnd_spi, const uint8_t *data, int len_tx_msg);


void vBlinkLed(void * pvParameters);


void app_main()
{
	printf("Hardware USB sniffer\n");

	spi_device_handle_t hwnd_usb_host;				// handle of SPI where connected MAX3421E in "usb host mode"
	spi_bus_config_t usb_host_buscfg;				// config SPI bus
	spi_device_interface_config_t usb_host_devcfg;	// config attached device (MAX3421)

	spi_device_handle_t hwnd_usb_device;				// handle of SPI where connected MAX3421E in "usb device mode"
	spi_bus_config_t usb_device_buscfg;					// config SPI bus
	spi_device_interface_config_t usb_device_devcfg;	// config attached device MAX3421 on "usb device mode"

	init_max3421e_host(&hwnd_usb_host, &usb_host_buscfg, &usb_host_devcfg);
	init_max3421e_device(&hwnd_usb_device, &usb_device_buscfg, &usb_device_devcfg);


	static xHwndSpi hwndMAX3421E;

	hwndMAX3421E.spi_host = hwnd_usb_host;
	hwndMAX3421E.spi_device = hwnd_usb_device;

	BaseType_t xReturned;

	xReturned = xTaskCreate(vBlinkLed,
							"BlinkLed",
							1536,						// stack size
							(void*)	&hwndMAX3421E,
							1,
							NULL);

	if ( xReturned == pdPASS)
	{
		printf("BlinkLed was created\n");
	}

}


// Init spi bus on ESP32
// ESP32 <---spi---> MAX3421 in host mode
void init_max3421e_host(spi_device_handle_t *hwnd_spi,
						   spi_bus_config_t *buscfg,
						   spi_device_interface_config_t *devcfg)
{
	// the configuration of SPI bus
	buscfg->mosi_io_num 	= USB_HOST_PIN_MOSI;
	buscfg->miso_io_num 	= USB_HOST_PIN_MISO;
	buscfg->sclk_io_num 	= USB_HOST_PIN_CLK;
	buscfg->quadwp_io_num 	= -1;
	buscfg->quadhd_io_num 	= -1;
	buscfg->max_transfer_sz = 0;
	buscfg->flags			= SPICOMMON_BUSFLAG_MASTER;

	// Initialize the SPI bus
	ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, buscfg, 0));
//	ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, buscfg, 1));
	printf("HSPI_HOST was successfully initialized\n");

	// the configuration of attached device (MAX3421 on "usb host mode")
	devcfg->address_bits		= 0;
	devcfg->command_bits		= 8;
	devcfg->dummy_bits			= 0;
	devcfg->mode				= 0;             		 //SPI mode 0
	devcfg->duty_cycle_pos		= 0;
	devcfg->cs_ena_posttrans	= 0;
	devcfg->cs_ena_pretrans		= 0;
	devcfg->clock_speed_hz		= SPI_CLK_MAX3421E_HOST;    		//Clock out
	devcfg->spics_io_num		= USB_HOST_PIN_CS;     	//CS pin
	devcfg->flags				= 0;
	devcfg->queue_size			= 3;              		//We want to be able to queue 7 transactions at a time
	devcfg->pre_cb				= NULL;
	devcfg->post_cb				= NULL;

	// Attach the MAX3421E (usb host mode) the SPI bus
	ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, devcfg, hwnd_spi));
	printf("MAX3421E in 'host' mode was successfully added\n");
}


// Init spi bus on ESP32
// ESP32 <---spi---> MAX3421 in device mode
void init_max3421e_device(spi_device_handle_t *hwnd_spi,
						spi_bus_config_t *buscfg,
						spi_device_interface_config_t *devcfg )
{
	buscfg->sclk_io_num 		= USB_DEVICE_CLK;
	buscfg->mosi_io_num 		= USB_DEVICE_MOSI;
	buscfg->miso_io_num 		= USB_DEVICE_MISO;
	buscfg->quadwp_io_num 		= -1;
	buscfg->quadhd_io_num 		= -1;
	buscfg->max_transfer_sz 	= 0;
	buscfg->flags				= SPICOMMON_BUSFLAG_MASTER;

	// Initialize the SPI bus
	ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, buscfg, 0));
//	ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, buscfg, 1));
	printf("VSPI_HOST was successfully initialized\n");

	// the configuration of attached device (MAX3421 on "usb device mode"
	devcfg->address_bits		= 0;
	devcfg->command_bits		= 8;
	devcfg->dummy_bits			= 0;
	devcfg->mode				= 0;              //SPI mode 0
	devcfg->duty_cycle_pos		= 0;
	devcfg->cs_ena_posttrans	= 0;
	devcfg->cs_ena_pretrans		= 0;
	devcfg->clock_speed_hz		= SPI_CLK_MAX3421E_DEVICE;    //Clock out
	devcfg->spics_io_num		= USB_DEVICE_CS;   //CS pin
	devcfg->flags				= 0;
	devcfg->queue_size			= 3;              //We want to be able to queue 7 transactions at a time
	devcfg->pre_cb				= NULL;
	devcfg->post_cb				= NULL;

	// Attach the MAX3421E (usb device mode) the SPI bus
	ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, devcfg, hwnd_spi));
	printf("MAX3421E in 'device' mode was successfully added\n");

}


void rxtx_spi(spi_device_handle_t hwnd_spi, const uint8_t *data, int len_tx_msg)
{
    spi_transaction_t msg_spi;

		msg_spi.addr		= 0;
		msg_spi.cmd			= 0;
		msg_spi.flags		= 0;
		msg_spi.length		= 8*len_tx_msg;
		msg_spi.tx_buffer	= data;
		msg_spi.rx_buffer	= NULL;
		msg_spi.rxlength	= 0;
		msg_spi.user		= NULL;

    ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg_spi));
}


void send_msg_spi(spi_device_handle_t hwnd_spi, const uint8_t data)
{

    spi_transaction_t msg_spi;

		msg_spi.addr		= 0;
		msg_spi.cmd			= 0xA2;
		msg_spi.flags		= 0;
		msg_spi.length		= 8;
		msg_spi.tx_buffer	= &data;
		msg_spi.rx_buffer	= NULL;
		msg_spi.user		= NULL;

    ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg_spi));
}


void send_msg_spi2(spi_device_handle_t hwnd_spi, const uint8_t *data, int len_tx_msg)
{
    spi_transaction_t msg_spi;

		msg_spi.addr		= 0;
		msg_spi.cmd			= 0;
		msg_spi.flags		= 0;
		msg_spi.length		= 8*len_tx_msg;
		msg_spi.tx_buffer	= data;
		msg_spi.rx_buffer	= NULL;
		msg_spi.rxlength	= 0;
		msg_spi.user		= NULL;

    ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg_spi));
}


void max3421e_rx_tx(spi_device_handle_t hwnd_spi, max3421e_msg *data)
{
    spi_transaction_t msg_spi;

		msg_spi.addr		= 0;
		msg_spi.cmd			= data->cmd > 0 ? data->cmd : 0;
		msg_spi.flags		= 0;
		msg_spi.length		= (data->tx_len > data->rx_len) ? 8 * data->tx_len : 8 * data->rx_len;
		msg_spi.tx_buffer	= (data->cmd & 0x2) == 0 ? NULL : data->tx_data;
		msg_spi.rx_buffer	= (data->rx_len) != 0 ? data->rx_data : NULL;
		msg_spi.rxlength	= data->rx_len;
		msg_spi.user		= NULL;

    ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg_spi));
}

void max3421e_rx_tx2(spi_device_handle_t hwnd_spi, max3421e_msg *data)
{
    spi_transaction_t msg_spi;

		msg_spi.addr		= 0;
		msg_spi.cmd		= 0;
		msg_spi.flags		= 0;
		msg_spi.length		= (data->tx_len > data->rx_len) ? 8 * data->tx_len : 8 * data->rx_len;
		msg_spi.tx_buffer	= data->rw ? data->tx_data : NULL;
		msg_spi.rx_buffer	= (data->rx_len) != 0 ? data->rx_data : NULL;
		msg_spi.rxlength	= data->rx_len;
		msg_spi.user		= NULL;

    ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg_spi));
}


/*
 *  This is test Task for blink led
 */
void vBlinkLed(void * pvParameters)
{
	static xHwndSpi *pxTaskParam;
	pxTaskParam = (xHwndSpi*) pvParameters;

	max3421e_msg *blink_on, *blink_off;


		blink_on = malloc(sizeof(max3421e_msg));
		if ( blink_on == NULL )
		{
			printf("	***\n");
			printf("Not enough memory\n");
			printf("	***\n");

		}

		blink_on->cmd = IOPINS1 | DIR_WR;
		blink_on->tx_data[0] = 0x1;
		blink_on->tx_len = 1;
		blink_on->rx_len = 0;

		blink_off = malloc(sizeof(max3421e_msg));
		if ( blink_off == NULL )
		{
			printf("	***\n");
			printf("Not enough memory for blink_off\n");
			printf("	***\n");

		}

		blink_off->cmd = IOPINS1 | DIR_WR;
		blink_off->tx_data[0] = 0x0;
		blink_off->tx_len = 1;
		blink_off->rx_len = 0;

		while ( 1 )
		{

			vTaskDelay(500 / portTICK_PERIOD_MS);
			max3421e_rx_tx(pxTaskParam->spi_device, blink_off);
			max3421e_rx_tx(pxTaskParam->spi_host, blink_on);


			vTaskDelay(500 / portTICK_PERIOD_MS);
			max3421e_rx_tx(pxTaskParam->spi_device, blink_on);
			max3421e_rx_tx(pxTaskParam->spi_host, blink_off);

		}

		vTaskDelete(NULL);

}
