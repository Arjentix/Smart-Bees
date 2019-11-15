# Active Connections Service

## Using Docker Compose

**It is recommended to use Docker Compose instead of Docker.**

### Running

Simply run next command from this service directory:

```bash
docker-compose up
```

### Building

To rebuild app run:

```bash
docker-compose build
```


## Using Docker

**There aren't any real reason to use `docker` command.**

All actions should be done from the **parent directory** -- **Micro-services**.

This is used to configure right context for *Docker container*, which is using some libraries. 

### Building

```bash
docker build -t active-connections-service -f Active-Connections-Service/Dockerfile .
```

### Running

```bash
docker run -it -p 2525:2525 -p 4551:4551 -v "$(pwd)/app/active-connections-service/log":/app/log active-connections-service
```
