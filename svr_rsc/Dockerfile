FROM gcc:4.9

COPY . /usr/src/http-server
WORKDIR /usr/src/http-server

RUN mkdir -p build/ && cd build/ && rm -rf * 
RUN cd build/ && gcc -I.. -o http_server ../main.cpp -lstdc++

#change this to whatever port you choose to run your server on
EXPOSE 8080 