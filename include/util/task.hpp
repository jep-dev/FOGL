#ifndef UTIL_TASK_HPP
#define UTIL_TASK_HPP

#include <atomic>
#include <deque>

namespace Util {
	/** Interface specifying init, poll, and run */
	struct task {
		
		/** Initializes any resources deferred from constructor
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual bool init(std::atomic_bool& alive) =0;
		/** Polls for changes since init or last poll
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual bool poll(std::atomic_bool &alive) =0;
		/** Runs a given task
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual bool run(std::atomic_bool& alive) =0;
		virtual ~task(void) {}
	
		/** Initializes any resources deferred from constructor
 		 * @param alive Shared status; false signals shutdown
 		 * @param t The task to initialize
 		 */
		static bool init(std::atomic_bool &alive, task *t) {
			return t -> init(alive);
		}
		/** Polls for changes since init or last poll
 		 * @param alive Shared status; false signals shutdown
 		 * @param t The task to poll
 		 */
		static bool poll(std::atomic_bool &alive, task *t) {
			return t -> poll(alive);
		}
		/** Runs a given task
 		 * @param alive Shared status; false signals shutdown
 		 * @param t The task to run
 		 */
		static bool run(std::atomic_bool &alive, task *t) {
			return t -> run(alive);
		}
	};

	/** Container of tasks applying init, poll, and run to children */
	struct supertask : public virtual task {
		std::deque<task*> tasks;
		bool init(std::atomic_bool& alive) {
			for(auto t : tasks) {
				if(!(t -> init(alive))) {
					return false;
				}
			}
			return true;
		}
		bool poll(std::atomic_bool& alive) {
			for(auto t : tasks) {
				if(!(t -> poll(alive))) {
					return false;
				}
			}
			return true;
		}
		bool run(std::atomic_bool& alive) {
			for(auto t : tasks) {
				if(!(t -> run(alive))) {
					return false;
				}
			}
			return true;
		}
		virtual ~supertask(void) {}
	};

	/** Task constructed with callbacks for init, poll, and run */
	struct worker : public virtual task {
		std::function<bool (std::atomic_bool &)>
			m_init, m_poll, m_run;
		bool init(std::atomic_bool &alive) {
			return m_init(alive);
		}
		bool poll(std::atomic_bool &alive) {
			return m_poll(alive);
		}
		bool run(std::atomic_bool &alive) {
			return m_run(alive);
		}
		worker(std::function<bool (std::atomic_bool &)> m_init,
				std::function<bool (std::atomic_bool &)> m_poll,
				std::function<bool (std::atomic_bool &)> m_run):
			m_init(m_init), m_poll(m_poll), m_run(m_run) {}
		virtual ~worker(void) {}
	};
}

#endif
