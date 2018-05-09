# usbsniff
Develop hardware usb 2.0 sniffer

The main idea - to develop cheap hardware usb sniffer which possible to assemble at home :)

Current version based on MAX3421E

/pcb

This folder contain KiCad project of usb sniffer

## Limitation

Support follow EP:
* EP0: CONTROL (64 bytes)
* EP1: OUT, BULK or INTERRUPT
* EP2: IN, BULK or INTERRUPT
* EP3: IN, BULK or INTERRUPT


Thank you for "x8-999-github" for some good advices and idea.
