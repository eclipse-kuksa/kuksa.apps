# Docker

## Build prerequisites

1. Enable docker experimental feature.

Create a daemon.json file in /etc/docker/ and add 

```
{
 
  "experimental" : true

}
```

and restart docker daemon using  `sudo systemctl restart docker`

https://github.com/docker/docker-ce/blob/master/components/cli/experimental/README.md

2. install  qemu-user-static package on the host machine

 use 
 
 ```sudo apt-get install qemu-user-static```

## Build

Build Kuksa-compatible dockers from this app. The Dockerfile is supposed to 
be run through the magic build.sh wrapper.

To build for am64 do

`./build.sh amd64`

to build for a Pi

`/build.sh arm32v6`

If you try playing with the docker file directly, note that the build context
needs to be the toplevel of invehicle-apps

## Run build docker
To start it run e.g.

```
docker run amd64/kuksa-traccar:0.1.0 
```

For configuration refer to the main readme. Please note that currently the traccar demo is configured using a config file, so you need to configure it according to your setup before packaging the docker.

## Can't restart container on ARM64
it has been observed, that on certain ARM64 systems all application code (all .py files and packages) vanish. The result is, that the container can only be started once. if it is ever stopped,
it won't run again, because the application is missing. The root cause is unclear, however it does not happen in a single stage build. If you suffer from this error use the Dockerfile.singlestate

```
mv Dockerfile Dockerfile.multi
mv Dockerfile.singlestage Dockerfile
```

and build as described above.



