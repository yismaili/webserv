# webserv

Software that receives requests through the HTTP protocol from clients such as web browsers and responds with static web pages
such as HTML documents.

<img width="738" alt="Screen Shot 2023-03-18 at 7 17 16 PM" src="https://user-images.githubusercontent.com/69278312/226126784-e79dbad1-25bd-4cb8-876f-61fc66945116.png">


# What is HTTP ?

HTTP is a request-response protocol that allows users to communicate data on the World Wide Web (WWW) and transfer hypertext.


# How It Works ?

Here’s how my server works in summary:

  - Listening for socket connections from clients
  - Creating a new thread for a new connection
  - Reading client’s request
  - Processing the request read from client
  - Constructing a response based on the request
  - Writing the response back to the client
  - Closing the connection
 
# What is a Socket ?

 .  software endpoint that establishes a communication link between two applications running on a network. whenever there is a need for inter process communication within the same machine, we use mechanism like signals or pipes(named or unnamed). Similarly, when we desire a communication between two applications possibly running on different machines, we need sockets

# What is TCP?

TCP, short for Transmission Control Protocol, a TCP server is a program that listens for incoming connections from TCP clients and provides services to them. An HTTP server is a type of TCP server that specifically handles HTTP requests and responses.

# HTTP requests

- Request line:
     .The request line is the first line in the request message. It consists of at least three items:
      A method. The method is a one-word command that tells the server what it should do with the resource. For example, the server could be asked      to send the resource to the client.
     .The path component of the URL for the request. The path identifies the resource on the server
     .The HTTP version number, showing the HTTP specification to which the client has tried to make the message comply.
     
                    GET /software/htp/cics/index.html HTTP/1.1
 - HTTP headers:
 
        
![1680902384854](https://user-images.githubusercontent.com/69278312/230799535-1c251fed-9171-4cf9-a17e-8b69533985c3.jpg)
<img width="1152" alt="Screen Shot 2023-03-25 at 10 19 40 PM" src="https://user-images.githubusercontent.com/69278312/236692418-7c7893f9-a8ed-44d4-9247-e61d6d18e7a4.png">
<img width="848" alt="Screen Shot 2023-03-30 at 10 31 06 PM" src="https://user-images.githubusercontent.com/69278312/236692423-67402497-5418-44aa-8de8-466c5a00ff2a.png">
<img width="1157" alt="Screen Shot 2023-03-25 at 6 42 08 PM" src="https://user-images.githubusercontent.com/69278312/236692425-9b63421c-fa49-49f9-9d2d-dcbfff074ad9.png">
