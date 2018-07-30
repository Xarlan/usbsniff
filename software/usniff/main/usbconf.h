/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

/*
 * usbconf.h
 *
 *  Created on: Jul 15, 2018
 *      Author: lem
 */

#ifndef MAIN_USBCONF_H_
#define MAIN_USBCONF_H_

// PINS which connected to MCU_spi <-> MAX3421 in host mode
#define USB_HOST_PIN_CLK  	25
#define USB_HOST_PIN_MOSI 	26
#define USB_HOST_PIN_MISO 	32
#define USB_HOST_PIN_CS   	33

// PINS which connected to MCU_spi <-> MAX3421 in device mode
#define USB_DEVICE_CLK   	21
#define USB_DEVICE_MOSI 	22
#define USB_DEVICE_MISO 	19
#define USB_DEVICE_CS   	23

#define MAX_BUF_USB_TX		64
#define MAX_BUF_USB_RX		64

#define SPI_CLK_MAX3421E_DEVICE		18*1000*1000			// attached to VSPI
#define SPI_CLK_MAX3421E_HOST		18*1000*1000			// attached to HSPI


typedef struct {
    			uint8_t 	cmd;
    			uint8_t		tx_data[MAX_BUF_USB_TX];
    			uint16_t 	tx_len;
    			uint8_t 	rx_data[MAX_BUF_USB_RX];
    			uint16_t 	rx_len;
} max3421e_msg;

typedef struct {
			uint8_t		reg;
			uint8_t		rw;
			uint8_t		tx_data[MAX_BUF_USB_TX];
    			uint16_t 	tx_len;
    			uint8_t 	rx_data[MAX_BUF_USB_RX];
    			uint16_t 	rx_len;
} MAX3421Emsg;

typedef struct {
	spi_device_handle_t spi_host;
	spi_device_handle_t spi_device;
} xHwndSpi;


/*
 * Configure SPI bus and attach MAX3421E (usb-host mode) to ESP32 via SPI
 */

void init_max3421e_host(spi_device_handle_t *hwnd_spi,
						spi_bus_config_t *buscfg,
						spi_device_interface_config_t *devcfg );


/*
 * Configure SPI bus and attach MAX3421E (usb-device mode) to ESP32 via SPI
 */

void init_max3421e_device(spi_device_handle_t *hwnd_spi,
						spi_bus_config_t *buscfg,
						spi_device_interface_config_t *devcfg );

/*
 * Transmit and receive data from MAX3421E via SPI bus
 */
//void max3421e_rx_tx(spi_device_handle_t hwnd_spi, const max3421e_msg *data);
void max3421e_rx_tx(spi_device_handle_t hwnd_spi, max3421e_msg *data);



#endif /* MAIN_USBCONF_H_ */


