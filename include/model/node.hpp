#ifndef NODE_HPP
#define NODE_HPP

#include <set>
#include <algorithm>

namespace Model {
	struct node;
	struct node {
		static int ids;
		int id = ids++;
		float x, y, z;
		std::set<const node*> adj;
		bool connected(const node *n) const;
		static void connect(node *a, node *b);
		static void disconnect(node *a, node *b);
		static void insert(node *src, node *dest, node *middle);
		bool operator==(const node& node) const;
		node& operator=(const float (&) [3]);
		node& operator+=(const node *n);
		node& operator-=(const node *n);
		virtual ~node(void);
	};
	struct graph {
		std::vector<node> nodes;
		std::vector<int> faces(void);
		graph(int n);
		virtual ~graph(void);
	};
}

#endif
