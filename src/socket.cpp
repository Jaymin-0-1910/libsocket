#include "socket.h"

#include <sys/socket.h>
#include <unistd.h>

namespace libsocket {
	socket::socket() : fd(-1) {}

	socket::socket(socket&& other) : fd(other.fd) {
		other.fd = -1;
	}

	socket::~socket() {
		destroy();
	}

	int socket::getfd() const {
		return fd;
	}

	int socket::destroy() {
		return fd <= 0 ? 0 : close(fd) < 0 ? -1 : 0;
	}

	int socket::setsockopt(int level, int opt, const char* val, socklen_t len) const {
		return ::setsockopt(fd, level, opt, val, len);
	}
}
