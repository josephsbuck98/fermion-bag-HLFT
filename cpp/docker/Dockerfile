# Use an official lightweight Ubuntu image
FROM ubuntu:22.04

# Install build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside the container
WORKDIR /app

# Copy project files into container
COPY . .

# Create build directory and build project
RUN mkdir build && cd build && cmake .. && make

# Run executable OR set entrypoint
## CMD [?????]
