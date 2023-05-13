import http.client

host = "127.0.0.1:8001"
body = "1" * 20000

def chunk_data(data, chunk_size):
    dl = len(data)
    ret = ""
    for i in range(dl // chunk_size):
        ret += "%s\r\n" % (hex(chunk_size)[2:])
        ret += "%s\r\n\r\n" % (data[i * chunk_size : (i + 1) * chunk_size])

    if len(data) % chunk_size != 0:
        ret += "%s\r\n" % (hex(len(data) % chunk_size)[2:])
        ret += "%s\r\n" % (data[-(len(data) % chunk_size):])

    ret += "0\r\n\r\n"
    return ret


# print(chunk_data(body, 8).encode('utf-8'));
# exit(1);
conn = http.client.HTTPConnection(host)
url = "/"
conn.putrequest('POST', url)
conn.putheader('Transfer-Encoding', 'chunked')
conn.endheaders()
conn.send(chunk_data(body, 300).encode('utf-8'))

resp = conn.getresponse()
print(resp.status, resp.reason)
conn.close()
