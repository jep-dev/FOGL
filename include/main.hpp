#ifndef MAIN_HPP
#define MAIN_HPP


typedef enum : unsigned char {
	e_tid_view=0, ///< The view thread ID, necessarily 0 (master)
	e_tid_model,  ///< The model thread ID
	e_tid_total   ///< The total number of enumerated threads
} e_tid; ///< The ID of a thread, where 0 is the master

/**
 * Entry point of the program
 * @param argc Number of args (1+)
 * @param argv Array of args (starting with self)
 * @return Zero if and only if exiting without failure
 */
int main(int argc, const char **argv);

#endif
