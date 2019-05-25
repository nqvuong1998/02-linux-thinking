FROM gcc:7.4.0
MAINTAINER  vuongnguyen
RUN mkdir -p /home/data
WORKDIR /home/data
COPY ./data .
RUN apt-get update
CMD chmod +x ./*
ENTRYPOINT ["./run-server.sh"]
ENV PATH /home/data:$PATH