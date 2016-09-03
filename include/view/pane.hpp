#ifndef PANE_HPP
#define PANE_HPP

#include <string>
#include <vector>

namespace View {
	struct pane : public virtual Util::task {
		typedef enum : uint8_t {
			e_dx=0, e_dy, e_x0, e_y0,
			e_fbo, e_zbo, e_tex,
			e_q_va, e_q_vb, e_q_prog, e_q_tex,
			e_total
		} e_ids;
		GLuint ids[e_ids::e_total] = {0};
		std::vector<std::string> errors;

		void resize(int dx, int dy, int x0 = 0, int y0 = 0);
		
		bool init(std::atomic_bool &alive) override;
		bool poll(std::atomic_bool &alive) override;
		bool run(std::atomic_bool &alive) override;

		pane(int dx, int dy, int x0 = 0, int y0 = 0);
	};
}

#endif
