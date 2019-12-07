#Building everything
FROM arm64v8/alpine:3.9 AS build-env

COPY sensoris.invehicle/docker/qemu-aarch64-static /usr/bin/qemu-aarch64-static

RUN apk update && apk add curl cmake alpine-sdk openssl-dev libstdc++ protobuf-dev boost-dev boost-static libstdc++


ADD . /app
WORKDIR /app/sensoris.invehicle

RUN rm -rf build && export  && mkdir build && cd build && cmake ..
RUN cd build && make



#Transferring build artifacts to minimal docker
FROM arm64v8/alpine:3.9 
#For debugging uncomment, and replae copy with real install to see what is missing...  
COPY --from=build-env /usr/lib/libssl.so.*   /lib/
COPY --from=build-env /usr/lib/libcrypto.so.*  /lib/
COPY --from=build-env /usr/lib/libstdc++.so.*  /usr/lib/
COPY --from=build-env /usr/lib/libgcc_s.so.* /usr/lib/
COPY --from=build-env /usr/lib/libprotobuf.so.* /usr/lib/


COPY --from=build-env /app/sensoris.invehicle/build/sensoris-invehicle /
COPY --from=build-env /app/sensoris.invehicle/docker/dockerentry.sh /
COPY --from=build-env /app/sensoris.invehicle/build/CA.pem /
COPY --from=build-env /app/sensoris.invehicle/build/Client.key /
COPY --from=build-env /app/sensoris.invehicle/build/Client.pem /

WORKDIR /
CMD /bin/sh /dockerentry.sh
