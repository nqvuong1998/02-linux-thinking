FROM gcc:7.4.0
MAINTAINER  vuongnguyen
RUN mkdir -p /home/data
WORKDIR /home/data
COPY ./data .
RUN gcc server.c -o server -pthread 
RUN gcc client.c -o client
ENV PATH /home/data:$PATH