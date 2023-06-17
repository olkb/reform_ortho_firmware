## Reform Ortho

![Reform Ortho](https://i.redd.it/teckwqi8zt871.png)

A MNT Reform-compatible keyboard that fits both the [laptop](https://shop.mntre.com/products/mnt-reform) and [standalone keyboard](https://shop.mntre.com/products/mnt-reform-usb-keyboard-standalone).

Build with [this version of QMK CLI](https://github.com/jackhumbert/qmk_cli/tree/exodus_experiments) (for the time being):
```
qmk compile -kb reform_ortho/reform_ortho -km default
```

By default, this board can be put into flashing mode by pressing `circle` + `F7`. Alternatively, flashing mode can be intiated by double pressing the physical DFU button on the PCB, or by pressing and holding the `BS` button prior to connecting the USB cable.

Once in flashing (DFU) mode, you can drag and drop the UF2 firmware onto the drive called `RPI-RP2`. Once the flashing process is complete, the drive will disappear and the keyboard will reboot itself with the new firmware.