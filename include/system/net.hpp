#ifndef SYSTEM_NET_HPP
#define SYSTEM_NET_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

namespace System {
	namespace Net {
		typedef std::integral_constant<short, 512> page_size;
		typedef char page[page_size::value];

		using namespace boost::asio;
		typedef ip::tcp TCP;
		typedef ip::udp UDP;
		typedef boost::system::error_code error_code;

		/// A type for asynchronous buffered socket read/write operations
		struct channel;

		/**
 		 * An abstract reader and writer with a FIFO message container
		 * @tparam T An implementation type
		 */
		template<class T = Util::undef_t> struct agent;

		/// An asynchronous acceptor and notifier
		struct server;

 		/// An asynchronous connector, reader and writer
		struct client;
	}
}

#endif
