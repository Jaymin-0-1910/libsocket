#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define LIBSOCKET_BACKLOG 128

#define LIBSOCKET_READ  1
#define LIBSOCKET_WRITE 2

int create_socket(const char* path, int flags) {
	if (path == NULL) {
		return -1;
	}
	if (strlen(path) > sizeof(((struct sockaddr_un*) 0)->sun_path) - 1) {
		return -1;
	}

	int fd = socket(AF_LOCAL, SOCK_STREAM | flags, 0);
	if (fd < 0) {
		return -1;
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr.sun_family) + strlen(addr.sun_path))) {
		close(fd);
		return -1;
	}
	return fd;
}

int destroy_socket(int fd) {
	return close(fd) < 0 ? -1 : 0;
}

int shutdown_socket(int fd, int method) {
	if (fd < 0) {
		return -1;
	}
	switch (method) {
		case LIBSOCKET_READ | LIBSOCKET_WRITE:
		case LIBSOCKET_READ:
		case LIBSOCKET_WRITE:
			break;
		default: return -1;
	}

	if (method & LIBSOCKET_READ) {
		if (shutdown(fd, SHUT_RD) < 0) {
			return -1;
		}
	}
	if (method & LIBSOCKET_WRITE) {
		if (shutdown(fd, SHUT_WR) < 0) {
			return -1;
		}
	}
	return 0;
}

int create_server_socket(const char* path, int flags) {
	if (path == NULL) {
		return -1;
	}
	if (strlen(path) > sizeof(((struct sockaddr_un*) 0)->sun_path) - 1) {
		return -1;
	}

	int fd = socket(AF_LOCAL, SOCK_STREAM | flags, 0);
	if (fd < 0) {
		return -1;
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));

	addr.sun_family = AF_LOCAL;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);

	if (unlink(path) == -1) {
		if (errno != ENOENT) {
			return close(fd), -1;
		}
	}

	if (bind(fd, (struct sockaddr*) &addr, sizeof(addr.sun_family) + strlen(addr.sun_path)) < 0) {
		return close(fd), -1;
	}
	if (listen(fd, LIBSOCKET_BACKLOG) < 0) {
		return close(fd), -1;
	}
	return fd;
}

int accept_socket(int fd, int flags) {
	return fd < 0 ? -1 : (fd = accept4(fd, 0, 0, flags)) < 0 ? -1 : fd;
}
