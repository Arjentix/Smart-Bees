# Building --------------
FROM ubuntu:latest AS build

# Name of the service folder
ARG SERVICE=Subscribe-Service

# Downloading dependencies
RUN apt-get update && \
    apt-get install -y \
		cmake \
		g++ \
		mysql-client \
		mysql-client-5.7 \
		mysql-common \
		mysql-server \
		mysql-server-5.7 \
		libmysqlclient-dev

# Configuring working environment
RUN mkdir -p /app
COPY ${SERVICE}/CMakeLists.txt /app
COPY ${SERVICE}/src /app/src

# Copying libs
COPY /libs/HTTPHandler /libs/HTTPHandler
COPY /libs/HTTPServer /libs/HTTPServer
COPY /libs/json /libs/json
COPY /libs/Logger /libs/Logger
COPY /Subscribe-Service/src/database /Subscribe-Service/src/database
COPY /libs/ConfigReader /libs/ConfigReader
COPY /libs/SimpleSQL /libs/SimpleSQL

RUN mkdir -p /app/build
WORKDIR /app/build

# Building
RUN cmake .. && \
	cmake --build . --target install

# Running ---------------
FROM ubuntu:latest
# Copying lib
COPY --from=build /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20 /usr/lib/x86_64-linux-gnu/

# Creating new user cause Docker uses root by default which is not good
#RUN groupadd -r sample && useradd -r -g sample sample
#USER sample

WORKDIR /app
COPY --from=build /app/bin/SubscribeService .
ENTRYPOINT [ "./SubscribeService" ]
