#!/usr/bin/python
# Import modules for CGI handling
import cgi, os
# Create instance of FieldStorage
form = cgi.FieldStorage()
# Get data from fields
filename = form.getvalue('filename')

print ("Content-type:text/html\r\n")
print ("\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello py cgi</title>")
print ("</head>")
print ("<body>")
if filename:
    try:
        os.remove(filename)
        print ("<h1>File %s deleted.</h1>" % filename)
    except:
        print ("<h1>Sorry, file %s not found.</h1>" % filename)
else:
    print("<h1>No file specified.</h1>")
print ("</body>")
print ("</html>")