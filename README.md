# uGamePad

## adding support for new gamepad
`lsusb`
```
Bus 001 Device 010: ID 0079:0011 DragonRise Inc. Gamepad
sudo usbhid-dump -s 001:010 -f -e stream
```

## testing/debug in linux with an xbox 360 controller
`lsusb`
```
Bus 003 Device 004: ID 24c6:550d ThrustMaster, Inc. Hori Gem Controller for Xbox 360
```
`sudo nano /etc/udev/rules.d/72-ugamepad.rules`
```
# Add hid raw access for this specific controllers
# ThrustMaster, Inc. Hori Gem Controller for Xbox 360

KERNEL=="hidraw*", ATTRS{idVendor}=="24c6", ATTRS{idProduct}=="550d", MODE="0660", TAG+="uaccess"
SUBSYSTEM=="usb", ATTRS{idVendor}=="24c6", ATTRS{idProduct}=="550d", MODE="0666"
```

## uGamePad v1.1 (USB-A)
Soldering order:
* 1 - 


## Nintendo Nes

Resources :
* 1 - [nesdev](https://www.nesdev.org/wiki/Controller_port_pinout)
* 2 - [tresi](https://tresi.github.io/nes/)

Controller pinout :

![nessnes-pinout](https://www.igorkromin.net/fp-content/images/snestones/NesSnesPinout.png)

[Nintendo Nes wire colors (aliexpress)](https://fr.aliexpress.com/item/33011370991.html?spm=a2g0o.order_list.order_list_main.6.6bf65e5bZjQmvi&gatewayAdapt=glo2fra) :
```
1   gnd     white
2   clock   green
3   latch   yellow
4   data    black
7   5v      red

```

[Nintendo Snes wire colors (aliexpress)](https://fr.aliexpress.com/item/33011370991.html?spm=a2g0o.order_list.order_list_main.6.6bf65e5bZjQmvi&gatewayAdapt=glo2fra) :
```
1   gnd     black 
2   clock   yellow
3   latch   white
4   data    red
7   5v      green

```

## Sega Genesis

Resources : 
* 1 - [plutiedev](https://plutiedev.com/io-pinout#md-port)
* 2 - [jonthysell](https://github.com/jonthysell/SegaController/wiki/How-To-Read-Sega-Controllers)
* 3 - [raspberryfield](https://www.raspberryfield.life/2019/02/15/sega-mega-drive-genesis-3-button-abc-controller/)
* 4 - [pinoutguide](https://pinoutguide.com/Game/genesiscontroller_pinout.shtml)

Controller pinout :

![genesis-pinout](https://plutiedev.com/img/md-port-pinout.png)

[Sega Genesis wire colors (aliexpress)](https://fr.aliexpress.com/item/1005002864827866.html?spm=a2g0o.order_detail.order_detail_item.13.22277d56hgr2Fe&gatewayAdapt=glo2fra) :
```
1   data0   red
2   data1   black
3   data2   grey
4   data3   orange
5   5v      brown
6   tl      green
7   th      white
8   gnd     blue
9   tr      yellow
```
