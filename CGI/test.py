#!/usr/bin/env python

import cgi

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")

print()
print("<html><body>")

form = cgi.FieldStorage()

if "name" not in form or "email" not in form:
    print("<p>Error: Please enter a name and email.</p>")
else:
    name = form["name"].value
    email = form["email"].value
    print("<p>Thank you for your submission, {} ({})!</p>".format(name, email))

print("</body></html>")