/*
 * usbconf.h
 *
 *  Created on: Jul 15, 2018
 *      Author: lem
 */

#ifndef MAIN_USBCONF_H_
#define MAIN_USBCONF_H_

// PINS which connected to MCU_spi <-> MAX3421 in host mode
#define USB_HOST_PIN_CLK  	21
#define USB_HOST_PIN_MOSI 	22
#define USB_HOST_PIN_MISO 	19
#define USB_HOST_PIN_CS   	23

// PINS which connected to MCU_spi <-> MAX3421 in device mode
#define USB_DEVICE_CLK   	25
#define USB_DEVICE_MOSI 	26
#define USB_DEVICE_MISO 	32
#define USB_DEVICE_CS   	33

#define MAX_BUF_USB_TX		1024
#define MAX_BUF_USB_RX		1024

#endif /* MAIN_USBCONF_H_ */


