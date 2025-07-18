#!/bin/bash

# Image name
IMAGE_NAME="main_image"

# Relative path to Dockerfile from cpp/
DOCKERFILE_PATH="docker/Dockerfile"

# Build the image
echo "Building Docker image: $IMAGE_NAME..."
docker build -f "$DOCKERFILE_PATH" -t "$IMAGE_NAME" .

# Check buil success
if [ $? -ne 0 ]; then
  echo "Docker build failed."
  exit 1
fi

# Run the container
echo "Running container from image: $IMAGE_NAME"
docker run --rm "$IMAGE_NAME"
