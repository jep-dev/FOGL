#ifndef MODEL_HPP
#define MODEL_HPP

#include <map>
#include <vector>
#include <functional>
#include <iostream>
#include <utility>
#include <cmath>

#include "../inc/quat.hpp"
#include "../inc/dual.hpp"

namespace Model {

	template<typename R = float>
	struct model {
		static unsigned int nextID;
		const unsigned int uid;
		bool operator<(model<R> const& rhs) const {
			return uid < rhs.uid;
		}
		std::map<model<R>,dual<R>> subs;
		model(void): uid(nextID++), subs{} {}
	};
	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, model<R> const& rhs) {
		for(auto &entry : rhs.subs) {
			lhs << entry.second << ": " << entry.first << "\r\n";
		}
		return lhs;
	}
	
	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, 
			quat<R> const& rhs) {
		/*quat<R> singlet = rhs.w;
		if(rhs == singlet) {return lhs << rhs.w;}
		return lhs << "[" << rhs.w << ", " << rhs.x 
			<< ", " << rhs.y << ", " << rhs.z << "]";*/
		return lhs << "[" << rhs.w << ", " << rhs.x
			<< ", " << rhs.y << ", " << rhs.z << "]";
	}

	template<typename R = float>
	std::ostream& operator<<(std::ostream& lhs, dual<R> const& rhs) {
		return lhs << "{" << rhs.u << ", " << rhs.v << "}";
	}
}

#endif
