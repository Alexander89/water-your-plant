# πͺ΄ Plant water sensor

This project is configured for an ESP32Dev board and a capacitive water sensor.

The water sensor is mostly in deep sleep. He awakes once every 10 min and stays awake until Prometheus queries its state.

Use [PlatformIO](https://platformio.org/) to build and deploy

## π‘ WLAN settings

`src/wlan.h` set your local credentials here.

## πΈοΈ IP

The IP is published via the Serial port, or check your DHCP server π€―

## π WebUi

The esp32 is configured to publish a webUI on port 80.

Here you can discover the current state and the Prometheus metrics data.

## π§ LINUX build fix

 1. Open `.pio/libdeps/esp32dev/esp32_https_server/src/HTTPConnection.hpp`
 2. Find `#include <hwcrypto/sha.h>`
 3. Replace it with `#include <sha/sha_parallel_engine.h>`

Or `git apply http-connection.patch`

## βΆοΈ Run Prometheus

`docker run -it -u $UID -v $(pwd)/prometheus/data:/prometheus -v $(pwd)/prometheus/config:/etc/prometheus --network=host prom/prometheus`
