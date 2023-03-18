# webserv

Software that receives requests through the HTTP protocol from clients such as web browsers and responds with static web pages
such as HTML documents.

<img width="738" alt="Screen Shot 2023-03-18 at 7 17 16 PM" src="https://user-images.githubusercontent.com/69278312/226126784-e79dbad1-25bd-4cb8-876f-61fc66945116.png">


# What is HTTP ?

HTTP or Hypertext Transfer Protocol is a protocol for distributed systems to communicate and transfer data.

# How It Works ?

Here’s how my server works in summary:

  - Listening for socket connections from clients
  - Creating a new thread for a new connection
  - Reading client’s request
  - Processing the request read from client
  - Constructing a response based on the request
  - Writing the response back to the client
  - Closing the connection
 