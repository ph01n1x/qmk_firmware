# Customized Keyboard Firmware

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