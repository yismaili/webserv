#!/usr/bin/env python
from http import cookies
import datetime, os

values = { 'a': 'a', 'b': 'b' }
expires = datetime.datetime.now() + datetime.timedelta(days=365)

client_cookie = cookies.SimpleCookie()
string_cookie = os.environ.get('HTTP_COOKIE')
# Load the client's cookies into the cookie object
if string_cookie:
    client_cookie.load(string_cookie)

cookie = cookies.SimpleCookie()
for key in values:
    cookie[key] = values[key]
    cookie[key]['expires'] = expires.strftime("%a, %d-%b-%Y %H:%M:%S PST")
    cookie[key]['path'] = '/'
    cookie[key]['HttpOnly'] = True;


# Output the HTTP message containing the cookie
print (cookie)
print ('Content-type: text/html\r\n')
print ('\r\n')

print ('<html><body>')
if len(client_cookie) == 0:
    print ('<p>cookie: is not set</p>')
else:
    for key in values:
        print ('<p>Cookie: %s = %s</p>' % (key, values[key]))
print ('</body></html>')

