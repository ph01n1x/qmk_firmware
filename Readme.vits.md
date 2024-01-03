# QMK MSYS

# To start MSYS in specified directory.

Add the following to .bash_profile located in your user's home directory

cd <Startup_Directory>

# Update the package database and core system packages with:

pacman -Syu  

# If needed, close QMK MSYS, run it again from Start menu. Update the rest with:

pacman -Su

# Customized Keyboard Firmware

## For converting image to Arduino C code

https://javl.github.io/image2cpp/

For OLED module, black and white picture. Set to

Image Settings: Invert image color
Code output format: Plain Bytes
Draw mode: Vertical - 1 bit per pixel.

## For RP2040 Build
add -e CONVERT_TO=helios

qmk flash -j 6 -kb aleblazer/zodiark -km vits  -e CONVERT_TO=helios

## [Zodiark](https://www.splitlogic.xyz/) 
[Build Guide](https://www.splitlogic.xyz/buildguides/zodiark-build-guide)

qmk compile -j 6 -kb aleblazer/zodiark -km vits  

## [Sofle](https://josefadamcik.github.io/SofleKeyboard/)

[v1/v2 Build Guide](https://josefadamcik.github.io/SofleKeyboard/build_guide.html)

[RGB Build Guide](https://josefadamcik.github.io/SofleKeyboard/build_guide_rgb.html)

qmk compile -j 6 -kb sofle/rev1/vits -km programming  
qmk compile -j 6 -kb sofle/rev1/vits -km rgb  

## [Knobgoblin](https://knob-goblin.com/)
qmk compile -j 6 -kb knobgoblin/vits -km numpad  
qmk compile -j 6 -kb knobgoblin/vits -km programming  

## [Tukey](https://keeb.io/products/tukey-1x2-big-switch-macropad)

qmk compile -j 6 -kb keebio/tukey/vits -km rgb  

## Notes
SAFE_RANGE = 0x5d62 // Sofle, Zodiark
 
## Unused Graphics
```
// 'mouse_cursor', 17x12px
0x00, 0x00, 0xf8, 0xc8, 0x72, 0x41, 0x44, 0x22, 0x10, 0x10, 0x90, 0x90, 0x20, 0x62, 0x8c, 0x30, 
0xc0, 0x0c, 0x0f, 0x08, 0x0d, 0x0c, 0x0c, 0x0a, 0x08, 0x0c, 0x0f, 0x0c, 0x08, 0x08, 0x08, 0x09, 
0x06, 0x01
```
