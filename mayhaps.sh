#!/bin/bash
docker run -it --rm \
  -u $(id -u):$(id -g) \
  -e DISPLAY=$DISPLAY \
  -e XAUTHORITY=/tmp/.Xauthority \
  -v $XAUTHORITY:/tmp/.Xauthority \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v "$(pwd)":/project \
  malloc-container
