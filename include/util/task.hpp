#ifndef UTIL_TASK_HPP
#define UTIL_TASK_HPP

#include <atomic>
#include <deque>

namespace Util {
	/** Interface specifying init, poll, and run */
	struct task {
		bool alive = false;
		/** Initializes any resources deferred from constructor
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual bool init(void) =0;
		/** Polls for changes since init or last poll
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual bool poll(void) =0;
		/** Runs a given task
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual bool run(void)=0;
		virtual ~task(void) {}
	};
}

#endif
