# Plant water sensor

This project is configured for an ESP32Dev board and a capacitive water sensor.

## WLAN settings

`src/wlan.h` set your local credentials here.

## LINUX build fix

 1. Open `.pio/libdeps/esp32dev/esp32_https_server/src/HTTPConnection.hpp`
 2. Find `include hwcrypto/sha.h`
 3. Replace it with `#include <sha/sha_parallel_engine.h>`

## Run Prometheus

`docker run -it -u $UID -v $(pwd)/prometheus/data:/prometheus -v $(pwd)/prometheus/config:/etc/prometheus --network=host prom/prometheus`