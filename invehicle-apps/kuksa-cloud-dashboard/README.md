## Kuksa-Dashboard app

This is a kuksa demo-app which connects to the w3c-visserver service via Websocket and talks to the Eclipse Hono MQTT adapter and sends telemetry data to Hono. This app reads the RPM, Speed, Fuel status and also custom DTC drom the w3c-visserver. This app also requires the following signal in the w3c-vis server tree.

`Signal.OBD.DTC1`

`Signal.OBD.DTC2`

`Signal.OBD.DTC3`

`Signal.OBD.DTC4`

`Signal.OBD.DTC5`

To add this signal to the vss-tree, please modify the vss_rel_1.0.json file used in the w3c-vis-server.


### Build docker image

use the instructions available in the docker folder to build the docker image.

use the kuksa-publisher to publish the app in the kuksa-appstore.


