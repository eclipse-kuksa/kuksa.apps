## Kuksa-Dashboard app

This is a kuksa demo-app which connects to the w3c-visserver service via Websocket and talks to the Eclipse Hono MQTT adapter and sends telemetry data to Hono. This app reads the RPM, Speed, Fuel status and also custom DTC drom the w3c-visserver. This app also requires the following signal in the w3c-vis server tree.

`Vehicle.OBD.DTC1`

`Vehicle.OBD.DTC2`

`Vehicle.OBD.DTC3`

`Vehicle.OBD.DTC4`

`Vehicle.OBD.DTC5`

To add this signal to the vss-tree, please modify the vss_rel_2.0.json file used in the w3c-vis-server.


### Build docker image

use the instructions available in the docker folder to build the docker image.

use the kuksa-publisher to publish the app in the kuksa-appstore.


### Publish app on kuksa-appstore

use the kuksa app-publisher to publish the app on the kuksa-appstore. The configuration for the publisher is found in kuksa-dashboard.yaml file.
Check the docs of app-publisher for more info.


