# Docker support
If you want to compile the project using different compilers and
operating systems then the development environment can be prepared using
[Docker](https://www.docker.com/) and the following commands:

    # Build a docker image
    docker build -t compute-samples:dev -f ubuntu_18_04.dockerfile .
    # Configure compute-samples inside a docker container
    docker run -v path/to/compute-samples:/opt/src -w /opt/src/build compute-samples:dev cmake ..
    # Build compute-samples inside a docker container
    docker run -v path/to/compute-samples:/opt/src -w /opt/src/build compute-samples:dev cmake --build .

If you want to configure Docker to use a proxy server then please check the [documentation](https://docs.docker.com/network/proxy/).

To reduce compilation time you can also use an image with preinstalled dependencies:

    docker build -t compute-samples:dev -f ubuntu_18_04_with_dependencies.dockerfile .
    docker run -v path/to/compute-samples:/opt/src -w /opt/src/build compute-samples:dev cmake .. -DBUILD_BOOST=OFF -DBUILD_PNG=OFF -DBUILD_OPENCL=OFF
    docker run -v path/to/compute-samples:/opt/src -w /opt/src/build compute-samples:dev cmake --build .
