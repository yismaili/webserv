# webserv
Webserv is a C++ 98 HTTP server that implements the Hypertext Transfer Protocol (HTTP) standards. It allows storing, processing, and delivering web pages to clients while adhering to the specified requirements. The server is designed to be non-blocking, ensuring optimal performance and compatibility with web browsers. In addition to the mandatory features, there is also an opportunity to implement bonus features such as cookies and session management.

# Requirements
      
# Description
   - The server should be a fully functional HTTP server implemented in C++ 98.
   - The program should take a configuration file as an argument or use a default path if none is provided.
   - The server must not block, and the client should be handled appropriately if necessary.
   - Non-blocking I/O operations should be implemented using a single poll() or equivalent function.
   - The poll() or equivalent function should check both read and write operations simultaneously.
   - All read and write operations should go through the poll() or equivalent function, and checking the value of errno after a read or write operation is strictly forbidden.
   - Reading the configuration file does not require using poll() or equivalent function.
   - Macros and defines such as FD_SET, FD_CLR, FD_ISSET, and FD_ZERO can be used.
   - Requests to the server should not hang indefinitely.
   - The server should be compatible with a chosen web browser, and its HTTP response status codes should be accurate.
   - Default error pages should be provided if none are specified.
   - The use of fork should be limited to CGI (Common Gateway Interface) implementations, such as PHP or Python.
   - The server should be able to serve a fully static website.
   - The server should support the GET, POST, and DELETE methods.
   - Stress testing of the server is required to ensure its availability under heavy load.
   - The server should be able to listen on multiple ports as specified in the configuration file.
      
# For macOS Only
   - Since macOS implements write() differently from other Unix operating systems, the use of fcntl() is allowed.
   - File descriptors should be set to non-blocking mode using the following code: fcntl(fd, F_SETFL, O_NONBLOCK). Any other flag for fcntl() is forbidden.
      
# Configuration File
   - The configuration file should resemble the 'server' part of an NGINX configuration file.
   - The configuration file should allow the following:
   - Specifying the port and host for each 'server'.
   - Setting up server names.
   - Assigning a default server for a host:port combination.
   - Configuring default error pages.
   - Limiting client body size.
   - Setting up routes with the following rules/configurations (regular expressions are not used):
   - Defining a list of accepted HTTP methods for the route.
   - Setting up HTTP redirection.
   - Specifying a directory or file to search for requested files.
   - Enabling or disabling directory listing.
   - Setting a default file to serve if the request is for a directory.
   - Executing CGI based on file extensions.
   - Supporting POST and GET methods.
   - Allowing routes to accept uploaded files and configuring the storage location.
      
  # Bonus Part
   The following bonus features can be added:

   - Support for cookies and session management.
   - Handling multiple CGI implementations.
