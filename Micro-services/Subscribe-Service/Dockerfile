# Building --------------
FROM ubuntu:latest AS build

# Name of the service folder
ARG SERVICE=Subscribe-Service

# Downloading dependencies
RUN apt-get update && \
    apt-get install -y \
		cmake \
		g++

# Configuring working environment
RUN mkdir -p /app
COPY ${SERVICE}/CMakeLists.txt /app
COPY ${SERVICE}/src /app/src

# Copying libs
COPY /libs/HTTPHandler /libs/HTTPHandler
COPY /libs/HTTPServer /libs/HTTPServer
COPY /libs/json /libs/json
COPY /libs/Logger /libs/Logger
COPY src/libs/database /libs/database
COPY /libs/ConfigReader /libs/ConfigReader

RUN mkdir -p /app/build
WORKDIR /app/build

# Building
RUN cmake .. && \
	cmake --build . --target install

# Running ---------------
FROM ubuntu:latest

WORKDIR /app
COPY --from=build /app/bin/SubscribeService .
ENTRYPOINT [ "./SubscribeService" ]
