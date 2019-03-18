#Building everything
FROM arm64v8/alpine:3.8 AS build-env

COPY remoteAccess/docker/qemu-aarch64-static /usr/bin/qemu-aarch64-static

RUN apk update && apk add cmake alpine-sdk boost-dev boost-system boost-thread openssl-dev libstdc++

ADD . /app
WORKDIR /app/remoteAccess

RUN rm -rf build && mkdir build && cd build && cmake ../
RUN cd build && make



#Transferring build artifacts to minimal docker
FROM arm64v8/alpine:3.8
#For debugging uncomment, and replae copy with real install to see what is missing... 
#RUN apk update && apk add openssl boost-system boost-thread libstdc++ 
COPY --from=build-env /lib/libssl.so.1.0.0   /lib/
COPY --from=build-env /lib/libcrypto.so.1.0.0  /lib/
COPY --from=build-env /usr/lib/libstdc++.so.6  /usr/lib/
COPY --from=build-env /usr/lib/libgcc_s.so.1 /usr/lib/


COPY --from=build-env /app/remoteAccess/build/remoteAccess /
COPY --from=build-env /app/remoteAccess/docker/dockerentry.sh /
COPY --from=build-env /app/remoteAccess/build/CA.pem /
COPY --from=build-env /app/remoteAccess/build/Client.key /
COPY --from=build-env /app/remoteAccess/build/Client.pem /

WORKDIR /
CMD /bin/sh /dockerentry.sh
