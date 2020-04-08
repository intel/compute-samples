# Docker support
If you want to compile the project using different compilers and
operating systems then the development environment can be prepared using
[Docker](https://www.docker.com/). Examples below:

    # Build using Ubuntu 16.04 image
    docker build -t compute-samples:dev -f docker/ubuntu_16_04.dockerfile .

    # Build using Ubuntu 18.04 image
    docker build -t compute-samples:dev -f docker/ubuntu_18_04.dockerfile .

    # Build using Ubuntu 20.04 image
    docker build -t compute-samples:dev -f docker/ubuntu_20_04.dockerfile .

If you want to configure Docker to use a proxy server then please check the [documentation](https://docs.docker.com/network/proxy/).
