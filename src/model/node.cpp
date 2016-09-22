#include "model/node.hpp"

namespace Model {
	bool node::connected(const node *n) {
		return std::find(std::begin(adj), std::end(adj), n) != std::end(adj);
	}
	void node::connect(node *a, node *b) {
		*a += b;
		*b += a;
	}
	void node::disconnect(node *a, node *b) {
		*a -= b;
		*b -= a;
	}
	node& node::operator=(const float (&p) [3]) {
		x = p[0];
		y = p[1];
		z = p[2];
		return *this;
	}
	node& node::operator+=(const node *n) {
		adj.insert(n);
		return *this;
	}
	node& node::operator-=(const node *n) {
		adj.erase(n);
		return *this;
	}
	node::~node(void) {}
}
