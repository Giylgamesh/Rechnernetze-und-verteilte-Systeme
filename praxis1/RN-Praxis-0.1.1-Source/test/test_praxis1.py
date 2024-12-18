import contextlib
import re
import socket
import time
from http.client import HTTPConnection

from util import KillOnExit

# Python 3.8 comptability
import sys
if sys.version_info[:3] >= (3, 9):
    from random import randbytes
else:
    from random import randint

    def randbytes(n):
        return bytes(randint(0, 255) for _ in range(n))


executable = 'build/webserver'
#executable = '/Users/neleporto/Studium/Semester 3/RN-Projekt1/build/webserver'
port = 4711


def test_execute():
    """
    Test server is executable
    """

    with KillOnExit([executable, '127.0.0.1', f'{port}']):
        pass


def test_listen():
    """
    Test server is listening on port
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), socket.create_connection(
        ('localhost', port), timeout=2
    ):
        pass


def test_reply():
    """
    Test the server is sending a reply
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), socket.create_connection(
        ('localhost', port), timeout=2
    ) as conn:
        conn.send('Request\r\n\r\n'.encode())
        reply = conn.recv(1024)
        assert len(reply) > 0


def test_packets():
    """
    Test HTTP delimiter for packet end
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), socket.create_connection(
        ('localhost', port), timeout=2
    ) as conn:
        conn.settimeout(.5)
        conn.send('GET / HTTP/1.1\r\n\r\n'.encode())
        time.sleep(.5)
        conn.send('GET / HTTP/1.1\r\na: b\r\n'.encode())
        time.sleep(.5)
        conn.send('\r\n'.encode())
        time.sleep(.5)  # Attempt to gracefully handle all kinds of multi-packet replies...
        replies = conn.recv(1024).split(b'\r\n\r\n')
        assert replies[0] and replies[1] and not replies[2]


def test_httpreply():
    """
    Test reply is syntactically correct HTTP packet
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), socket.create_connection(
        ('localhost', port), timeout=2
    ) as conn:
        conn.settimeout(.5)
        conn.send('Request\r\n\r\n'.encode())
        time.sleep(.5)  # Attempt to gracefully handle all kinds of multi-packet replies...
        reply = conn.recv(1024)
        assert re.search(br'HTTP/1.[01] 400.*\r\n(.*\r\n)*\r\n', reply)


def test_httpreplies():
    """
    Test reply is semantically correct HTTP packet
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), contextlib.closing(
        HTTPConnection('localhost', port, timeout=2)
    ) as conn:
        conn.connect()

        # there is no HEAD command in HTTP :-)
        conn.request('HEAD', '/index.html')
        reply = conn.getresponse()
        reply.read()
        assert reply.status == 501, "HEAD did not reply with '501'"

        conn.request('GET', '/index.html')
        reply = conn.getresponse()
        assert reply.status == 404


def test_static_content():
    """
    Test static content can be accessed
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), contextlib.closing(
        HTTPConnection('localhost', port, timeout=2)
    ) as conn:
        conn.connect()

        for path, content in {
            '/static/foo': b'Foo',
            '/static/bar': b'Bar',
            '/static/baz': b'Baz',
        }.items():
            conn.request('GET', path)
            reply = conn.getresponse()
            payload = reply.read()
            assert reply.status == 200
            assert payload == content

        for path in [
            '/static/other',
            '/static/anything',
            '/static/else'
        ]:
            conn.request('GET', path)
            reply = conn.getresponse()
            reply.length = 0  # Convince `http.client` to handle no-content 404s properly
            reply.read()
            assert reply.status == 404


def test_dynamic_content():
    """
    Test dynamic storage of data (key,value) works
    """

    with KillOnExit(
        [executable, '127.0.0.1', f'{port}']
    ), contextlib.closing(
        HTTPConnection('localhost', port, timeout=2)
    ) as conn:
        conn.connect()

        path = f'/dynamic/{randbytes(8).hex()}'
        content = randbytes(32).hex().encode()

        conn.request('GET', path)
        response = conn.getresponse()
        payload = response.read()
        assert response.status == 404, f"'{path}' should be missing, but GET was not answered with '404'"

        conn.request('PUT', path, content)
        response = conn.getresponse()
        payload = response.read()
        assert response.status in {200, 201, 202, 204}, f"Creation of '{path}' did not yield '201'"

        conn.request('GET', path)
        response = conn.getresponse()
        payload = response.read()
        assert response.status == 200
        assert payload == content, f"Content of '{path}' does not match what was passed"

        conn.request('DELETE', path)
        response = conn.getresponse()
        payload = response.read()
        assert response.status in {200, 202, 204}, f"Deletion of '{path}' did not succeed"

        conn.request('GET', path)
        response = conn.getresponse()
        payload = response.read()
        assert response.status == 404, f"'{path}' should be missing"
