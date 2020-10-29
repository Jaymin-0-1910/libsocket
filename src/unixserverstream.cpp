#include "socket.h"

#include <string>

using namespace std;

namespace libsocket {
	unix_stream_server::unix_stream_server() {}

	unix_stream_server::unix_stream_server(const char *path, int flags) {
		setup(path, flags);
	}

	unix_stream_server::unix_stream_server(const string &path, int flags) {
		setup(path, flags);
	}

	void unix_stream_server::setup(const char *path, int flags) {
		if (path == NULL) {
			throw socket_exception("libsocket: Null path given");
		}
		if (fd != -1) {
			throw socket_exception("libsocket: Socket already set up");
		}
		fd = create_server_socket(path, flags);
		if (fd < 0) {
			throw socket_exception("libsocket: Error creating server socket");
		}
		_path = path;
	}

	void unix_stream_server::setup(const string &path, int flags) {
		setup(path.c_str(), flags);
	}

	unix_stream_client *unix_stream_server::accept(int flags) {
		if (fd == -1) {
			throw socket_exception("libsocket: Socket not set up yet");
		}
		int _fd = accept_socket(fd, flags);
		if (_fd < 0) {
			throw socket_exception("libsocket: Error accepting connection");
		}
		unix_stream_client *client = new unix_stream_client;
		client->fd = _fd;
		return client;
	}
}
