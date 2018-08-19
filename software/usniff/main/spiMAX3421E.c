/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

/*
 * spiMAX3421E.c
 *
 *  Created on: Aug 13, 2018
 *      Author: xarlan
 */

#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/spi_master.h"


#include "spiMAX3421E.h"
#include "usbconf.h"
#include "max3421e.h"


/******************************************************************************
* Init spi bus on ESP32														  *
* ESP32 <---spi---> MAX3421 in host mode									  *
*******************************************************************************/
void init_spi_Husb(spi_device_handle_t *hwnd_spi,
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
	printf("HSPI_HOST was successfully initialized\n");

	// the configuration of attached device (MAX3421 on "usb host mode")
	devcfg->address_bits		= 8;
	devcfg->command_bits		= 0;
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


/******************************************************************************
* Init VSPI_HOST on ESP32													  *
* ESP32 <---spi---> MAX3421 in host mode									  *
*******************************************************************************/
void init_spi_Pusb(spi_device_handle_t *hwnd_spi,
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
	printf("VSPI_HOST was successfully initialized\n");

	// the configuration of attached device (MAX3421 on "usb device mode"
	devcfg->address_bits		= 8;
	devcfg->command_bits		= 0;
	devcfg->dummy_bits			= 0;
	devcfg->mode				= 0;              			//SPI mode 0
	devcfg->duty_cycle_pos		= 0;
	devcfg->cs_ena_posttrans	= 0;
	devcfg->cs_ena_pretrans		= 0;
	devcfg->clock_speed_hz		= SPI_CLK_MAX3421E_DEVICE;	//Clock out
	devcfg->spics_io_num		= USB_DEVICE_CS;   			//CS pin
	devcfg->flags				= 0;
	devcfg->queue_size			= 3;              			//We want to be able to queue 7 transactions at a time
	devcfg->pre_cb				= NULL;
	devcfg->post_cb				= NULL;

	// Attach the MAX3421E (usb device mode) the SPI bus
	ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, devcfg, hwnd_spi));
	printf("MAX3421E in 'device' mode was successfully added\n");

}


/******************************************************************************
* Write <val> to <reg> in MAX3421E											  *
*******************************************************************************/
void w_reg(spi_device_handle_t hwnd_spi, uint8_t reg, uint8_t val)
{
	spi_transaction_t msg;

	memset(&msg, 0, sizeof(msg));       //Zero out the transaction

	msg.cmd			= 0;
	msg.addr		= reg | DIR_WR;
	msg.flags		= 0;
	msg.length		= 8;
	msg.tx_buffer	= &val;
	ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg));
}


/******************************************************************************
* Read 1 <reg> in MAX3421E											  		  *
*******************************************************************************/
uint8_t r_reg(spi_device_handle_t hwnd_spi, uint8_t reg)
{
	spi_transaction_t msg;
	uint8_t	value;

	memset(&msg, 0, sizeof(msg));       //Zero out the transaction

	msg.cmd			= 0;
	msg.addr		= reg | DIR_RD;
	msg.flags		= 0;
	msg.length		= 8;
	msg.tx_buffer	= NULL;
	msg.rxlength	= 1;
	msg.rx_buffer	= &value;
	ESP_ERROR_CHECK(spi_device_transmit(hwnd_spi, &msg));

	return value;
}
