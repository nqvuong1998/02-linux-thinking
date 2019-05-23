#!/bin/bash


sudo docker build -t gcc-image:v1 .
sudo docker network create backend-network
sudo docker run -it --net=backend-network -p 8888:8888 gcc-image --name gcc-server-container

sudo docker run -it --net=backend-network -p 8888:8888 gcc-image --name gcc-client-container




