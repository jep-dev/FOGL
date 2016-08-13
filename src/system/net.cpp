#include "util.hpp"

#include "system.hpp"
#include "system/net.hpp"
#include "system/printer.hpp"

#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/system/error_code.hpp>
#include <boost/optional.hpp>
#include <boost/operators.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <functional>

namespace System {
	namespace Net {

		static void sleep_for(deadline_timer &dt, short ms_wait = 500) {
			static const auto delay =
				boost::posix_time::milliseconds(ms_wait);
			dt.expires_from_now(delay);
			dt.wait();
		}

		struct channel:
		public std::enable_shared_from_this<channel> {
			TCP::socket sock;
			template<typename T>
			channel& read(T &t) {
				auto scoped = shared_from_this();
				sock.async_read_some(
					buffer(ibuf, page_size::value),
					[&, this, scoped] (error_code ec, std::size_t len) {
						if(ec) return;
						if(len > 0) t.emplace_back(ibuf, len);
						read(t);
					});
				return *this;
			}
			channel& write(short bytes = page_size::value) {
				auto scoped = shared_from_this();
				sock.async_write_some(buffer(obuf, strlen(obuf)),
					[&, this, scoped] (error_code ec, std::size_t len) {
						if(ec) return;
						write();
					});
				return *this;
			}
			channel(io_service &svc, TCP::socket socket):
				sock(std::move(socket)), svc(svc) {}
		private:
			io_service &svc;
			page ibuf, obuf;
		};

		template<class C>
		struct agent {
			agent(io_service &svc):
				sock(svc), svc(svc) {}
		protected:
			page ibuf, obuf;
			TCP::socket sock;
			io_service &svc;
			std::deque<std::string> lines;
		};
		
		struct server: public agent<server>,
		std::enable_shared_from_this<server> {
			server(io_service &svc, short port):
				agent<server>(svc),
				acc(svc, std::move(TCP::endpoint(TCP::v4(), port))) {}
			/// Async (accept and recurse) and return
			void accept(void) {
				auto scoped = shared_from_this();
				acc.async_accept(sock,
					[&, this, scoped] (error_code ec) {
						if(ec) return;
						std::make_shared<channel>(svc, std::move(sock))
							-> read(lines).write();
						accept();
					});
			}
			/// Async (flush and recurse) and return
			void flush(void) {
				auto scoped = shared_from_this();
				svc.post([&, this, scoped] {
					while(!lines.empty()) {
						std::cout << lines.front() << std::endl;
						lines.pop_front();
					}
					flush();
				});
			}
			/// Start async processes and return
			void start(void) {
				accept();
				flush();
			}
		protected:
			TCP::acceptor acc;
		};

		struct client: public agent<client>,
		std::enable_shared_from_this<client> {
			void read(void) {
				auto scoped = shared_from_this();
				async_read(sock, buffer(ibuf),
					[&, this, scoped] (error_code ec, std::size_t len) {
						if(ec) return;
						if(len > 0) {
							std::cout << "Client read \"" << len << "B: \""
								<< std::string(ibuf, len) << std::endl;
						}
						read();
					});
			}
			void write(void) {
				auto scoped = shared_from_this();
				const char *msg = "Same message";
				std::copy(msg, msg+strlen(msg), obuf);
				async_write(sock, buffer(obuf, strlen(obuf)),
					[&, this, scoped] (error_code ec, std::size_t len) {
						if(ec) return;
						write();
					});
			}
			void start(int max_attempts = 3, int attempts = 0) {
				send("Hello, world!");
				try {
					auto scoped = shared_from_this();
					sock.async_connect(
						TCP::endpoint(TCP::v4(), port),
						[&, this, scoped] (const error_code &ec) {
							if(ec) return;
							read();
							write();
						});
				} catch(std::exception &e) {
					std::cout << e.what() << std::endl;
					attempts++;
					if(attempts < max_attempts) {
						start(max_attempts, attempts);
					}
				}
			}
			client(io_service &svc, short port):
			agent<client>(svc), port(port), res(svc) {}
		private:
			void send(const char *msg) {
				std::copy(msg, msg+strlen(msg), obuf);
			}
			short port;
			TCP::resolver res;
		};
	}
}
