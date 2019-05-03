FROM arm64v8/python:3.6-alpine AS build-env

COPY kuksa-traccar-client/docker/qemu-aarch64-static /usr/bin/qemu-aarch64-static


ADD ./kuksa-traccar-client/ /app
WORKDIR /app
COPY kuksa-traccar-client/requirements.txt ./
RUN pip install --target=./ --no-cache-dir -r requirements.txt
RUN ls /usr/local/lib
RUN rm ./requirements.txt
RUN rm /usr/bin/qemu-aarch64-static
WORKDIR /app


FROM arm64v8/python:3.6-alpine
COPY --from=build-env /app /app
ENV PYTHONPATH=/usr/local/lib/python3.5/site-packages
ENV PYTHONUNBUFFERED=yes
WORKDIR /app
CMD python -u traccar-client.py
