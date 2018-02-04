# ESP8266 Remote Power Switch

ESP8266 app to provide a RESTful API to receivers supported by
[rc-switch](https://github.com/sui77/rc-switch).

## Configuration
Create a `wifi_config.h` containing your WiFi credentials. Then configure the
pin the transmitter is connected to in `ESP8266RemotePowerSwitch.ino`, and
optionally update the web UI in `ui.h`.

## Exposed API
```
PUT /<switch_group_no>/<switch_no>
Form data:
    state: on|off
```
