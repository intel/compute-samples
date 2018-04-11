# Docker support
If you want to compile the project using different compilers and
operating systems then the development environment can be prepared using
[Docker](https://www.docker.com/) and the following commands:

    # Build a docker image
    docker build -t compute-samples:dev .
    # Configure compute-samples inside a docker container
    docker run -v path/to/compute-samples:/opt/src -w /opt/src/build compute-samples:dev cmake ..
    # Build compute-samples inside a docker container
    docker run -v path/to/compute-samples:/opt/src -w /opt/src/build compute-samples:dev cmake --build .

If you want to configure Docker to use a proxy server then please check the [documentation](https://docs.docker.com/network/proxy/).
