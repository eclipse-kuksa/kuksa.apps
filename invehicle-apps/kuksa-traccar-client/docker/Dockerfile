FROM arm64v8/python:3.7-alpine AS build-env

COPY kuksa-traccar-client/docker/qemu-aarch64-static /usr/bin/qemu-aarch64-static


ADD ./kuksa-traccar-client/ /app
WORKDIR /app
RUN pip install --target=./ --no-cache-dir -r requirements.txt


FROM arm64v8/python:3.7-alpine
COPY --from=build-env /app /app

ENV PYTHONUNBUFFERED=yes
WORKDIR /app
CMD ["/usr/local/bin/python", "traccar-client.py"]
