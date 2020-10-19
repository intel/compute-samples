FROM ubuntu:18.04

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

COPY . /opt/src

RUN cd /opt/src && scripts/install/install_ubuntu_18_04.sh

RUN mkdir /opt/src/build && cd /opt/src/build && cmake .. -DCMAKE_BUILD_TYPE=RELEASE && cmake --build .

CMD ["/bin/bash"]

