# Terrarium relay

ESPHome config for terrarium relays, which can provide light, heat and humidifier:

* Relay, connected to Ultraviolet lamp, provides light
* Relay, connected to heat lamp, provides heat.
* Relay, connected to humidifier, provides humidity.

You can also use polyphonic alarm or telegram notifications to alarm if something went wrong (for example, heat lamp broke).

Later I replaced it with **terrarium-dimmer** project because dimmer is more reliable then relay.

This project is just a simple multichannel relay config with additional lambda function to self disable relay in case Home Assistant crashed.

Generally automation should do the following:

1. Get temperature and humidity from any kind of sensors. For exampe, I used Aqara zigbee sensors.
2. If temperature is too low - enable heating relay, otherwise disable it.
3. Same for humidity
4. If it is daytime - enable daylight, otherwise disable it.

You can use automation sample from terrarium-dimmer project for relays as well.
