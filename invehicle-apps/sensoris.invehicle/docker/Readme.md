# Docker

## Build prerequisites

1. Enable docker experimental feature.

Create a daemon.json file in /etc/docker/ and add 

{
 
  "experimental" : true

}

and restart docker daemon using  "systemctl restart docker"

https://github.com/docker/docker-ce/blob/master/components/cli/experimental/README.md

2. install  qemu-user-static package on the host machine

 use "sudo apt-get install qemu-user-static"

## Build

Build Kuksa-compatible dockers from this app. The Dockerfile is supposed to 
be run through the magic build.sh wrapper.

To build for am64 do

`./build.sh amd64`

to build for a Pi

`/build.sh arm32v6`

If you try playing with the docker file directly, note that the build context
needs to be the toplevel of kuksa.invehicle

# Running the container
This app needs Hono credentials. It uses the following environment variables

| Variable             | What                               | 
| ---------------------|------------------------------------| 
| VEHICLE_CLOUD_HOST   | Sensoris vehicle cloud IP          |
| VEHICLE_CLOUD_PORT   | Sensoris vehicle cloud port        |
| SESSION_ID           | Sensoris session ID                |
| VEHICLE_ID           | Sensoris vehicle ID                |
| W3C_SERVER_HOST      | W3C vis server host IP             |
| W3C_SERVER_PORT      | W3C vis server port                |
| W3C_JWT_TOKEN        | W3C JWT security token             |


Example commandline
 `docker run --net=host -e VEHICLE_CLOUD_HOST=127.0.0.1 -e VEHICLE_CLOUD_PORT=9000 -e SESSION_ID=1234 -e VEHICLE_ID=1234567 -e  W3C_SERVER_HOST=localhost -e W3C_SERVER_PORT=8090 -e W3C_JWT_TOKEN=eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiJFeGFtcGxlIEpXVCIsImlzcyI6IkVjbGlwc2Uga3Vrc2EiLCJhZG1pbiI6dHJ1ZSwiaWF0IjoxNTE2MjM5MDIyLCJleHAiOjE2MDkzNzI4MDAsInczYy12c3MiOnsiVmVoaWNsZS5PQkQuKiI6InJ3IiwiVmVoaWNsZS5DYWJpbi5JbmZvdGFpbm1lbnQuTmF2aWdhdGlvbi5DdXJyZW50TG9jYXRpb24uKiI6InJ3In19.QYs78CwwyOPnzXUZdHYbWzD7hwMWuNJWuNBTNYIBQqvkM7Df5CsIV_CvfiozSQybSPwlthnm4yJ7xgyC2yQTLh6vKKEIAlNjFcOep39b3fGEDDrRHkOyMJQuxYD5ELALKEPbakITi4QfqWozcZQNY9AdyYROKJqmTPlaYZmT0_mAQQFCIk7ji-udxtqTGtO6-WW5cDPkp0cqaDy1ZJusVUsRJGUkd942jgqbcVp98n7j4o8NvSQhZ6wGrG8Nc1tpTMUdXQmuMPA3Iv2P3cSMuBrHAY5I8511OFgIpB51gydKF4PaqTpirxCswm85iZq6so9CnMwh9I7VBO5GI6Nz_lnFfyeg1NEdRmSTEI6ucqLhjx54R2cT-fa6KIHcDKw63ZrXGT-dNyhX2px0Q5zM2zuy8phwLj6cXujCS8-uOmlysvmxwgh3oIUmtmP47x5ZLkFqaYuU2Ue-kiSr1lyleaEhQxSGykCJr6OibQ4gA6b5VIXn3mfkMWF8fse3Xm1D8jJ6UyE9zl55GhgvcAX9AlQYuqRTcaSvhZbzDmSO4pMda0wuHyHnB1DBiFcV9AtbPQsLraP2VVNGlKiWQkiJp66H0fkj1NPmQvCbbQ9v8YT-_DmAEvwnY9Mcx26FYodzC055lny4ThRXTCSmnJdxA05Av-BCjAGxPTinYxKCA1c  amd64/sensoris-invehicle:0.0.1`
 


