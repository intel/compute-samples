FROM ubuntu:16.04

LABEL maintainer="lukasz.towarek@intel.com"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libx11-dev \
    libva-dev \
    libpng-dev \
    wget \
    unzip

RUN wget https://cmake.org/files/v3.8/cmake-3.8.2.tar.gz && tar xvf cmake-3.8.2.tar.gz && cd cmake-3.8.2 && cmake . && cmake --build . --target install

COPY . /opt/src

RUN cd /opt/src && scripts/install/install_ubuntu_16_04.sh

RUN mkdir /opt/src/build && cd /opt/src/build && cmake .. -DCMAKE_BUILD_TYPE=RELEASE && cmake --build .

CMD ["/bin/bash"]

