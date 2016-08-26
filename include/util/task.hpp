#ifndef UTIL_TASK_HPP
#define UTIL_TASK_HPP

#include <atomic>
#include <deque>

namespace Util {
	struct task {
		
		/** Initializes any resources deferred from constructor
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual void init(std::atomic_bool& alive) =0;
		/** Runs a given task
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual void run(std::atomic_bool& alive) =0;
		virtual ~task(void) {}
	
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

	struct supertask : public virtual task {
		std::deque<task*> tasks;
		void init(std::atomic_bool& alive) {
			for(auto t : tasks) {
				t -> init(alive);
			}
		}
		void run(std::atomic_bool& alive) {
			for(auto t : tasks) {
				t -> run(alive);
			}
		}
		virtual ~supertask(void) {}
	};
}

#endif
