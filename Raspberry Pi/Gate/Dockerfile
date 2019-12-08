# Building --------------
FROM ubuntu:latest AS build

# Downloading dependencies
RUN apt-get update && \
    apt-get install -y \
		libmosquitto-dev \
		cmake \
		g++

# Configuring working environment
RUN mkdir -p /app
COPY CMakeLists.txt /app
COPY src /app/src


RUN mkdir -p /app/build
WORKDIR /app/build

# Building
RUN cmake .. && \
	cmake --build . --target install

# Running ---------------
FROM ubuntu:latest

# Copying shared libraries
COPY --from=build /usr/lib/x86_64-linux-gnu/libmosquitto.so.1 /usr/lib/x86_64-linux-gnu/
COPY --from=build /usr/lib/x86_64-linux-gnu/libssl.so.1.1 /usr/lib/x86_64-linux-gnu/
COPY --from=build /usr/lib/x86_64-linux-gnu/libcrypto.so.1.1 /usr/lib/x86_64-linux-gnu/
COPY --from=build /usr/lib/x86_64-linux-gnu/libcares.so.2 /usr/lib/x86_64-linux-gnu/

EXPOSE 1883

WORKDIR /app
COPY --from=build /app/bin/gate .
ENTRYPOINT [ "./gate" ]
