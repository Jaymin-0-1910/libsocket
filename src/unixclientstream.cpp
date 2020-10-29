#include "socket.h"

#include <string>

using namespace std;

namespace libsocket {
	unix_stream_client::unix_stream_client() {}

	unix_stream_client::unix_stream_client(const char *path, int flags) {
		connect(path, flags);
	}

	unix_stream_client::unix_stream_client(const string &path, int flags) {
		connect(path, flags);
	}

	void unix_stream_client::connect(const char *path, int flags) {
		if (path == NULL) {
			throw socket_exception("libsocket: Null path given");
		}
		if (fd != -1) {
			throw socket_exception("libsocket: Socket already connected");
		}
		fd = create_socket(path, flags);
		if (fd < 0) {
			throw socket_exception("libsocket: Error creating and connecting socket");
		}
		_path = path;
		shut_rd = false;
		shut_wr = false;
	}

	void unix_stream_client::connect(const string &path, int flags) {
		connect(path.c_str(), flags);
	}
}
