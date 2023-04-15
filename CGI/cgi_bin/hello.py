#!/usr/bin/python
# Import modules for CGI handling
import cgi
# Create instance of FieldStorage
form = cgi.FieldStorage()

first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')

print ("Content-type:text/html\r\n")
print ("\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello py cgi</title>")
print ("</head>")
print ("<body>")
print ("<h1>Hello %s %s</h1>" % (first_name, last_name))
print ("</body>")
print ("</html>")