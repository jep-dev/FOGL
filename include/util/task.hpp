#ifndef UTIL_TASK_HPP
#define UTIL_TASK_HPP

#include <atomic>

namespace Util {
	struct task {
		virtual void init(std::atomic_bool&) =0;
		virtual void run(std::atomic_bool&) =0;
		//virtual ~task(void);
		//task()=default;
		static void run(std::atomic_bool &alive, task *t) {
			t -> run(alive);
		}
	};
}

#endif
