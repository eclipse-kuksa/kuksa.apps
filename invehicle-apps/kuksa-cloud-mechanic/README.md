## Kuksa-Cloud-Mechanic app

This is a kuksa demo-app which connects to the w3c-visserver service via Websocket and talks to the Eclipse Hono MQTT adapter. Using this app commands can be sent to the vehicle remotely.
This app requires  minimum Hono 0.7 version. This app also requires the following signal in the w3c-vis server tree.

`Signal.Drivetrain.InternalCombustionEngine.ThrottleTest`.

To add this signal to the vss-tree, please modify the vss_rel_1.0.json file used in the w3c-vis-server.


### Build docker image

use the instructions available in the docker folder to build the docker image.

use the kuksa-publisher to publish the app in the kuksa-appstore.


