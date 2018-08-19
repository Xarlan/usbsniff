/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/
*/

/*
 * max3421e.h
 *
 *  Created on: Jul 15, 2018
 *      Author: lem
 */

#ifndef MAIN_MAX3421E_H_
#define MAIN_MAX3421E_H_

// MAX3421E register format:
// command byte format: rrrrr0wa, where
// - 'r' is register number R0 ... R31
// - 'w' is mode, 1 - write/ 0 - read
// - 'a' ACKSTAT, valid only in peripheral mode

// PERIPHERAL Mode MAX3421
#define EP0FIFO     0x0         // R0
#define EP1OUTFIFO  0x8         // R1
#define EP2INFIFO   0x10        // R2
#define EP3INFIFO   0x18        // R3
#define SUDFIFO     0x20        // R4
#define EP0BC       0x28        // R5
#define EP1OUTBC    0x30        // R6
#define EP2INBC     0x38        // R7
#define EP3INBC     0x40        // R8
#define EPSTALLS    0x48        // R9
#define CLRTOGS     0x50        // R10
#define EPIRQ       0x58        // R11
#define EPIEN       0x60        // R12
#define USBIRQ      0x68        // R13
#define USBIEN      0x70        // R14
#define USBCTL      0x78        // R15
#define CPUCTL      0x80        // R16
#define PINCTL      0x88        // R17
#define REVISION    0x90        // R18
#define FNADDR      0x98        // R19
#define IOPINS1     0xA0        // R20
#define IOPINS2     0xA8        // R21
#define GPINIRQ     0xB0        // R22
#define GPINIEN     0xB8        // R23
#define GPINPOL     0xC0        // R24
#define MODE        0xD8        // R27


// PERIPHERAL Mode MAX3421

#define RCVFIFO     0x8         // R1
#define SNDFIFO     0x10        // R2
#define SUDFIFO     0x20        // R4
#define RCVBC       0x30        // R6
#define SNDBC       0x38        // R7
#define USBIRQ      0x68        // R13
#define USBIEN      0x70        // R14
#define USBCTL      0x78        // R15
#define CPUCTL      0x80        // R16
#define PINCTL      0x88        // R17
#define REVISION    0x90        // R18
#define IOPINS1     0xA0        // R20
#define IOPINS2     0xA8        // R21
#define GPINIRQ     0xB0        // R22
#define GPINIEN     0xB8        // R23
#define GPINPOL     0xC0        // R24
#define HIRQ        0xC8        // R25
#define HIEN        0xD0        // R26
#define MODE        0xD8        // R27
#define PERADDR     0xE0        // R28
#define HCTL        0xE8        // R29
#define HXFR        0xF0        // R30
#define HRSL        0xF8        // R31

// Direction
#define DIR_WR      0x2
#define DIR_RD      0x0

/************************
	Register R11 - EPIRQ
	  HOST		Perip
	   -          +
************************/
#define bIN0BAVIRQ	0x01
#define bOUT0DAVIRQ 0x02
#define bOUT1DAVIRQ 0x04
#define bIN2BAVIRQ	0x08
#define bIN3BAVIRQ	0x10
#define bSUDAVIRQ	0x20

/************************
	Register R13 - USBIRQ
	  HOST		Perip
	   +          +
************************/
#define bOSCOKIRQ  0x01
#define bRWUDNIRQ  0x02
#define bBUSACTIRQ 0x04
#define bURESIRQ   0x08
#define bSUSPIRQ   0x10
#define bNOVBUSIRQ 0x20
#define bVBUSIRQ   0x40
#define bURESDNIRQ 0x80



/*
	Register R17 - PINCTL
	  HOST		Perip
	   +          +
*/
#define	bGPXA		0x1
#define bGPXB		0x2
#define bPOSINT		0x4
#define bINTLEVEL	0x8
#define bFDUPSPI	0x10
#define bEP1INAK	0x20
#define bEP2INAK	0x40
#define bEP3INAK	0x80


/*
	Register R25 - HIRQ
	  HOST		Perip
	   +          -
*/
#define	bBUSEVENTIRQ	0x1
#define bRWUIRQ			0x2
#define bRCVDAVIRQ		0x4
#define bSNDBAVIRQ		0x8
#define bSUSDNIRQ		0x10
#define bCONDETIRQ		0x20
#define bFRAMEIRQ		0x40
#define bHXFRDNIRQ		0x80


/*
	Register R27 - MODE
	  HOST		Perip
	   +          +
*/
#define	bHOST		0x1
#define bLOWSPEED	0x2
#define bHUBPRE		0x4
#define bSOFKAENAB	0x8
#define bSEPIRQ		0x10
#define bDELAYISO	0x20
#define bDMPULLDN	0x40
#define bDPPULLDN	0x80

/************************
	Register R29 - HCTL
	  HOST		Perip
	   +          -
************************/
#define bBUSRST        0x01
#define bFRMRST        0x02
#define bSAMPLEBUS     0x04
#define bSIGRSM        0x08
#define bRCVTOG0       0x10
#define bRCVTOG1       0x20
#define bSNDTOG0       0x40
#define bSNDTOG1       0x80


/************************
	Register R31 - HRSL
	  HOST		Perip
	   +          -
************************/
#define bRCVTOGRD   0x10
#define bSNDTOGRD   0x20
#define bKSTATUS    0x40
#define bJSTATUS    0x80 


#endif /* MAIN_MAX3421E_H_ */

