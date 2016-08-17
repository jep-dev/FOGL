#ifndef UTIL_TASK_HPP
#define UTIL_TASK_HPP

#include <atomic>

namespace Util {
	struct task {
		
		/** Initializes any resources deferred from constructor
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual void init(std::atomic_bool&) =0;
		/** Runs a given task
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual void run(std::atomic_bool&) =0;
	
		/** Initializes any resources deferred from constructor
 		 * @param alive Shared status; false signals shutdown
 		 * @param t The task to initialize */
		static void init(std::atomic_bool &alive, task *t) {
			t -> init(alive);
		}
		/** Runs a given task
 		 * @param alive Shared status; false signals shutdown
 		 * @param t The task to run
 		 */
		static void run(std::atomic_bool &alive, task *t) {
			t -> run(alive);
		}
	};
}

#endif
