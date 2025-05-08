FROM ubuntu:latest

# Install required packages
RUN apt-get update && \
    apt-get install -y \
    clang \
    libgmp-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the default C compiler to clang
ENV CC=clang
ENV CXX=clang++

# Verify installation
RUN clang --version
