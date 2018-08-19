/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

/*
 * spiMAX3421E.h
 *
 *  Created on: Aug 13, 2018
 *      Author: xarlan
 */

#ifndef MAIN_SPIMAX3421E_H_
#define MAIN_SPIMAX3421E_H_





/******************************************************************************
* Init HSPI_HOST on ESP32														  *
* ESP32 <---spi---> MAX3421 in host mode									  *
*******************************************************************************/
void init_spi_Husb(spi_device_handle_t *hwnd_spi,
						   spi_bus_config_t *buscfg,
						   spi_device_interface_config_t *devcfg);


/******************************************************************************
* Init VSPI_HOST on ESP32													  *
* ESP32 <---spi---> MAX3421 in host mode									  *
*******************************************************************************/
void init_spi_Pusb(spi_device_handle_t *hwnd_spi,
						spi_bus_config_t *buscfg,
						spi_device_interface_config_t *devcfg );



/******************************************************************************
* Write <val> to <reg> in MAX3421E											  *
*******************************************************************************/
void w_reg(spi_device_handle_t hwnd_spi, uint8_t reg, uint8_t val);


/******************************************************************************
* Read 1 <reg> in MAX3421E											  		  *
*******************************************************************************/
uint8_t r_reg(spi_device_handle_t hwnd_spi, uint8_t reg);

#endif /* MAIN_SPIMAX3421E_H_ */
