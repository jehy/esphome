# Came Gates ESPHome controller

Open CAME gates using Arduino and 433 Mhz transmitter, based on [this sketch](https://gist.github.com/superyarik/3eb4da9da728466c072e716532d732ef).

You can obtain your gate's code using [this sketch](https://github.com/jehy/arduino-came-reader).

## Installation:
1. Extract `came-gates-controller.h`, `came-gates-controller.yaml` and `settings.sample.h` yo your ESPHome directory.
2. Copy `settings.sample.h` to `settings.h` and tweak it (provide gate code and transmitter pin).
3. Upload integration from esphome dashboard. 
