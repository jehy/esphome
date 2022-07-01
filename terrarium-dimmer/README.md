# Terrarium dimmer


ESPHome config for terrarium dimmer and relay - relay provides light and dimmer provides heat.

## How it works

It works on computed HA sensors:

1. "Gecko Day" sensor. We define what time will be day for terrarium.
2. "Gecko Goal Temperature" sensor. We define temerature based on current "Gecko day" value.
3. "Gecko needed heat" sensor. We define how much heat we should give to dimmer to make goal temparature.

After it we need two simple automations:
1. Turn on and of lamp relay based on "Gecko day" value.
2. Set dimmer value to the value of "Gecko needed heat" sensor.

ESPHome yaml also has a lambda which automatically turns of dimmer if there was no change on it's state for 10 minutes.
It is necessary to avoid frying terrarium in case when HA crashed while dimmer was on.


## Computed sensors for Home Assistant:

```yaml
template:
  - binary_sensor:
      - name: "Gecko Day"
        unique_id: binary_sensor.gecko_day
        state: >
          {{ ((now() > today_at("05:00")) and (now() < today_at("17:00"))) }}

  - sensor:
      - name: "Gecko Temperature"
        unique_id: sensor.gecko_temperature
        device_class: temperature
        unit_of_measurement: "ºC"

        state: >
          {% if not is_state('sensor.0x00158d0006ebf8b0_temperature', 'unavailable') %}
            {{ states('sensor.0x00158d0006ebf8b0_temperature') }}
          {% elif not is_state('sensor.0x00158d0006ed8a58_temperature', 'unavailable') %}
            {{ states('sensor.0x00158d0006ed8a58_temperature') }}
          {% else %}
            30
          {% endif %}
  - sensor:
      - name: "Gecko Goal Temperature"
        unique_id: sensor.gecko_goal_temperature
        device_class: temperature
        unit_of_measurement: "ºC"

        state: >
          {% if is_state('binary_sensor.gecko_day', 'on') %}
            27
          {% else %}
            25
          {% endif %}

  - sensor:
      - name: "Gecko Needed heat"
        unique_id: sensor.gecko_need_heat
        device_class: temperature
        unit_of_measurement: "ºC"

        state: >
          {% set goal = states('sensor.gecko_goal_temperature') | float %}
          {% set current = states('sensor.gecko_temperature') | float %}
          {% set heatSpeed = 255 | float %}
          {% set level = (goal - current) * heatSpeed %}
          {% if level < 1 %}
            0
          {% else %}
            {{ [level, 255] | min | round(1, 'ceil') }}
          {% endif %}
```

## Automation yaml:

```yaml
- id: '1655931673366'
  alias: 'Gecko heat V2'
  description: ''
  trigger:
  - platform: time_pattern
    minutes: /1
  condition: []
  action:
  - service: light.turn_on
    entity_id: light.gecko_heat
    data_template:
      brightness: '{{ states(''sensor.gecko_needed_heat'') | int }}

        '
  mode: single
- id: '1656018565513'
  alias: 'Gecko light V2'
  description: ''
  trigger:
  - platform: time_pattern
    minutes: /2
  condition: []
  action:
  - service_template: "{% if is_state('binary_sensor.gecko_day', 'on') %}\n  homeassistant.turn_on\n\
      {% else %}\n  homeassistant.turn_off\n{% endif %}\n"
    entity_id: switch.gecko_day_lamp
  mode: single
  ```
