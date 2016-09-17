#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include "util.hpp"
#include <vector>
#include <string>

namespace Adapter {
	struct adapter : public Util::task {
		std::vector<std::string> errors;
		bool init(std::atomic_bool &alive) override;
		bool poll(std::atomic_bool &alive) override;
		bool run(std::atomic_bool &alive) override;
		adapter(std::atomic_bool &alive);
	};
}

#endif
