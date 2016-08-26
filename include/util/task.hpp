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
		/** Polls for changes
 		 * @param alive Shared status; false signals shutdown
 		 */
		virtual void poll(std::atomic_bool &alive) =0;
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
		static void poll(std::atomic_bool &alive, task *t) {
			t -> poll(alive);
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
		void poll(std::atomic_bool& alive) {
			for(auto t : tasks) {
				t -> poll(alive);
			}
		}
		void run(std::atomic_bool& alive) {
			for(auto t : tasks) {
				t -> run(alive);
			}
		}
		virtual ~supertask(void) {}
	};

	struct worker : public virtual task {
		std::function<void (std::atomic_bool &)>
			m_init, m_poll, m_run;
		//void (*m_run)(std::atomic_bool &);
		void init(std::atomic_bool &alive) {
			m_init(alive);
		}
		void poll(std::atomic_bool &alive) {
			m_poll(alive);
		}
		void run(std::atomic_bool &alive) {
			m_run(alive);
		}
		worker(std::function<void (std::atomic_bool &)> m_init,
				std::function<void (std::atomic_bool &)> m_poll,
				std::function<void (std::atomic_bool &)> m_run):
			m_init(m_init), m_poll(m_poll), m_run(m_run) {}
		virtual ~worker(void) {}
	};
}

#endif
