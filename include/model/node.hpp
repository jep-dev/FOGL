#ifndef NODE_HPP
#define NODE_HPP

#include <set>
#include <algorithm>

namespace Model {
	struct node;
	struct node {
		float x, y, z;
		std::set<const node*> adj;
		bool connected(const node *n);
		static void connect(node *a, node *b);
		static void disconnect(node *a, node *b);
		node& operator=(const float (&) [3]);
		node& operator+=(const node *n);
		node& operator-=(const node *n);
		virtual ~node(void);
	};
}

#endif
