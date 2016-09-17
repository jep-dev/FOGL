#include "control/adapter.hpp"
#include "model.hpp"
#include "view.hpp"

namespace Adapter {
	bool adapter::init(std::atomic_bool &alive) {return alive;}
	bool adapter::poll(std::atomic_bool &alive) {return alive;}
	bool adapter::run(std::atomic_bool &alive) {return alive;}
	adapter::adapter(std::atomic_bool &alive) : Util::task() {}
}
