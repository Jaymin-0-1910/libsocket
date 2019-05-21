#include "socket.h"

#include <cstring>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

namespace libsocket {
	stream_client_socket::stream_client_socket() : shut_rd(false), shut_wr(false) {}

	ssize_t stream_client_socket::send(const void* buf, size_t len, int flags) {
		if (buf == NULL || len == 0) {
			throw socket_exception("Either buffer or length is NULL");
		}
		if (fd == -1) {
			throw socket_exception("Socket is not connected");
		}
		if (shut_wr) {
			throw socket_exception("Socket has already been shut down");
		}
		ssize_t sent = ::send(fd, buf, len, flags);
		if (sent == -1) {
			throw socket_exception("Could not write to socket");
		}
		return sent;
	}

	ssize_t stream_client_socket::recv(void* buf, size_t len, int flags) {
		if (buf == NULL || len == 0) {
			throw socket_exception("Either buffer or length is NULL");
		}
		if (fd == -1) {
			throw socket_exception("Socket is not connected");
		}
		if (shut_rd) {
			throw socket_exception("Socket has already been shut down");
		}

		memset(buf, 0, len);

		ssize_t recvd = ::recv(fd, buf, len, flags);
		if (recvd == -1) {
			throw socket_exception("Could not read from socket");
		}
		return recvd;
	}

	void stream_client_socket::shutdown(int method) {
		if (method & (LIBSOCKET_READ | LIBSOCKET_WRITE) && shut_rd && shut_wr) {
			return;
		}
		if (method & LIBSOCKET_READ && shut_rd) {
			return;
		}
		if (method & LIBSOCKET_WRITE && shut_wr) {
			return;
		}

		int _method = 0;

		if (method == (LIBSOCKET_READ | LIBSOCKET_WRITE)) {
			_method = SHUT_RDWR;
		} else if (method == LIBSOCKET_READ) {
			_method = SHUT_RD;
		} else if (method == LIBSOCKET_WRITE) {
			_method = SHUT_WR;
		} else {
			return;
		}

		if (::shutdown(fd, _method) < 0) {
			throw socket_exception("Could not shutdown socket");
		}
		if (method & LIBSOCKET_READ) {
			shut_rd = true;
		}
		if (method & LIBSOCKET_WRITE) {
			shut_wr = true;
		}
	}

	stream_client_socket& operator<<(stream_client_socket& sock, const char* str) {
		if (sock.fd == -1) {
			throw socket_exception("Socket is not connected");
		}
		if (sock.shut_wr) {
			throw socket_exception("Socket has already been shut down");
		}
		if (str == NULL) {
			throw socket_exception("Given string is NULL");
		}
		if (write(sock.fd, str, strlen(str)) == -1) {
			throw socket_exception("Could not write to socket");
		}
		return sock;
	}

	stream_client_socket& operator<<(stream_client_socket& sock, const string& str) {
		if (sock.fd == -1) {
			throw socket_exception("Socket is not connected");
		}
		if (sock.shut_wr) {
			throw socket_exception("Socket has already been shut down");
		}
		if (str.empty()) {
			throw socket_exception("Givent string is null");
		}
		if (write(sock.fd, str.c_str(), str.size()) == -1) {
			throw socket_exception("Could not write to socket");
		}
		return sock;
	}

	stream_client_socket& operator>>(stream_client_socket& sock, string& dest) {
		if (sock.fd == -1) {
			throw socket_exception("Socket is not connected");
		}
		if (sock.shut_rd) {
			throw socket_exception("Socket has already been shut down");
		}

		unique_ptr<char[]> buffer(new char[dest.size()]);
		memset(buffer.get(), 0, dest.size());

		ssize_t readd = read(sock.fd, buffer.get(), dest.size());
		if (readd == -1) {
			throw socket_exception("Could not read from socket");
		}
		dest = buffer.get();
		return sock;
	}
}
