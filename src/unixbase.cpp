#include "socket.h"

#include <string>

using namespace std;

namespace libsocket {
	unix_socket::unix_socket() : _path("") {}

	string unix_socket::getpath() {
		return _path;
	}
}
