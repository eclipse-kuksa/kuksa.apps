FROM python:3.5-slim AS build-env
ADD . /app
WORKDIR /app
COPY requirements.txt ./
RUN pip install --target=./ --no-cache-dir -r requirements.txt
RUN ls /usr/local/lib
WORKDIR /app

FROM gcr.io/distroless/python3
COPY --from=build-env /app /app
#COPY --from=build-env /usr/local/lib/python3.5/site-packages /usr/local/lib/python3.5/site-packages
ENV PYTHONPATH=/usr/local/lib/python3.5/site-packages
ENV PYTHONUNBUFFERED=yes
WORKDIR /app
CMD ["traccar-client.py"]
