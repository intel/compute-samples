FROM ubuntu:18.04

LABEL maintainer="lukasz.towarek@intel.com"

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    libpng-dev \
    libssl-dev \
    libx11-dev \
    libva-dev \
    ocl-icd-opencl-dev \
    wget
