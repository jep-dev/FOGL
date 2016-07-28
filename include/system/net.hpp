#ifndef SYSTEM_NET_HPP
#define SYSTEM_NET_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include "util.hpp"
#include "util/types.hpp"

namespace Net {
	typedef std::integral_constant<short, 512> page_size;
	typedef char page[page_size::value];

	using namespace boost::asio;
	typedef ip::tcp TCP;
	typedef ip::udp UDP;
	typedef boost::system::error_code error_code;

	struct cotask;
	struct channel;
	template<typename> struct agent;
	struct server;
	struct client;
}

#endif
