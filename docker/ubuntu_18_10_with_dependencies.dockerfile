FROM ubuntu:18.10

LABEL maintainer="lukasz.towarek@intel.com"

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libx11-dev \
    libva-dev \
    wget \
    libpng-dev \
    ocl-icd-opencl-dev
