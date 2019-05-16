# Docker support
If you want to compile the project using different compilers and
operating systems then the development environment can be prepared using
[Docker](https://www.docker.com/). Examples below:

    # Build using default Ubuntu 16.04 image
    docker build -t compute-samples:dev -f ubuntu_16_04.dockerfile .

    # Build using default Ubuntu 18.04 image
    docker build -t compute-samples:dev -f ubuntu_18_04.dockerfile .

If you want to configure Docker to use a proxy server then please check the [documentation](https://docs.docker.com/network/proxy/).
