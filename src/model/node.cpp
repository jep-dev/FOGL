#include "model/node.hpp"

namespace Model {
	int node::ids = 0;
	bool node::connected(const node *n) const {
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
	void node::insert(node *src, node *dest, node *middle) {
		disconnect(src, dest);
		connect(src, middle);
		connect(dest, middle);
		*middle = {(src->x+dest->x)/2, (src->y+dest->y)/2,
			(src->z+dest->z)/2};
	}
	bool node::operator==(const node &n) const {
		return id == n.id;
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
	std::vector<int> graph::faces(void) {
		// TODO prevent duplicates including rotations?
		std::vector<int> output;
		//int i = 0;
		for(const auto n : nodes) {
			for(const auto a : n.adj) {
				/*int j = std::find(std::begin(nodes), std::end(nodes), *a)
					- std::begin(nodes);*/
				for(const auto b : a -> adj) {
					/*int k = std::find(std::begin(nodes), std::end(nodes), *b)
						- std::begin(nodes);*/
					if(a -> connected(b) && b -> id != n.id) {
						output.emplace_back(n.id);
						output.emplace_back(a->id);
						output.emplace_back(b->id);
					}
				}
			}
			//i++;
		}
		return output;
	}
	graph::graph(int n): nodes(n) {}
	graph::~graph(void) {}
}
