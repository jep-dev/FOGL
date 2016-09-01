#ifndef SYSTEM_PRINTER_HPP 
#define SYSTEM_PRINTER_HPP

#include "util.hpp"

#include <cstring>
#include <iomanip>
#include <algorithm>
#include <sstream>

namespace System {

	struct Printer_Base {
		typedef std::string string;
		typedef std::ostream OS;
		typedef std::ostringstream OSS;

		enum Alignment {LEFT, CENTER, RIGHT};
		static constexpr const char
			border_nw='.', border_n='-', border_ne='.',
			border_w='|', space=' ', border_e='|',
			border_sw='\'', border_s='-', border_se='\'',
			divider_n=border_ne, divider_c='|', divider_s=border_se,
			padding_nw=border_n, padding_w=space, padding_sw=border_s,
			padding_ne=border_n, padding_e=space, padding_se=border_s;

		static bool uni_special(char ch);
		static int uni_strlen(const char *word);
		static int uni_strlen(const std::string &word);
		static int strlen(const char *word);
		static int split(const std::string &line,
				std::vector<std::string> &words);
		
		static string repeat(int N, char C = ' ');

		template<typename R>
		static string align(const R &value, int width, 
				Alignment dir = LEFT, char filler = space,
				int precision = 3) {
			OSS oss;
			bool numerical = std::is_floating_point<R>::value
					|| std::is_integral<R>::value;
			if(numerical) {
				oss << std::setprecision(precision) << std::fixed << value;
			} else {
				oss << value;
			}
			auto word = oss.str();
			int diff = width - word.length(),
				lhalf = diff/2, rhalf = diff - lhalf;
			if(diff < 0) {
				return word.substr(0, width);
			}
			for(int i = 0; i < lhalf; i++) {
				switch(dir) {
					case RIGHT:
						word = filler + word;
						break;
					default:
						word += filler;
				}
			}
			for(int i = 0; i < rhalf; i++) {
				switch(dir) {
					case LEFT:
						word += filler;
						break;
					default:
						word = filler + word;
				}
			}
			return word;
		}
	};

	template<unsigned int H> struct NoCRTP;

	template<unsigned int H, typename C = NoCRTP<H>>
	struct Printer: public Printer_Base {
		typedef Printer<H,C> SUPER;
		typedef C SELF;

		OSS lines[H];

		friend OS& operator<<(OS& lhs, const Printer &p) {
			for(auto &line : p.lines) {
				lhs << line.str() << "\r\n";
			}
			return lhs;
		}
		SELF& clear(void) {
			for(auto &line : lines) {
				line.clear();
				line.str("");
			}
			return self();
		}
		std::pair<int,int> minMax(void) const {
			int hi = 0, low = 0;
			for(int i = 0; i < H; ++i) {
				int size = lines[i].str().size();
				if(size<low) low=size;
				if(!i || size>hi) hi=size;
			}
			return {low, hi};
		}

		SELF& level(void) {
			int maxlen = minMax().second;
			std::for_each(begin(lines), end(lines),
			[&maxlen](OSS &line) {
				line << repeat(maxlen-line.str().size());
			});
			return self();
		}

		SELF& insert(int row, const string &add) {
			if(row < H) {
				lines[row] << add;
			}
			return self();
		}
		
		SELF& push(char *start, char *stop) {
			for(auto &line : lines) {
				if(start >= stop) {
					break;
				}
				line << *start++;
			}
			return self();
		}

		SELF& push(string *start, string *stop) {
			int maxlen = 0, diff = stop - start;
			std::for_each(start, stop,
			[&maxlen] (const string &label) {
				int len = label.size();
				if(len > maxlen) {
					maxlen = len;
				}
			});
			for(int row = 0; row < H && row < diff; ++row) {
				lines[row] << align(row < H ?
					start[row] : "", maxlen, CENTER);
			}
			return self();
		}

		template<typename T, int ROWS, int COLS, int WIDTH=10*COLS+1>
		SELF& push(T (&data)[ROWS*COLS], string *start, string *stop) {
			if(ROWS > H) {
				return self();
			}

			int cellspace = WIDTH - 3*COLS - 1,
				cellspan = cellspace/COLS,
				cellrem = cellspace%COLS,
				diff = stop-start;

			auto outerW = border_nw + repeat(ROWS, border_w) + border_sw,
				 innerW = padding_nw + repeat(ROWS, padding_w) + padding_sw,
				 divider = divider_n + repeat(ROWS, divider_c) + divider_s,
				 innerE = padding_ne + repeat(ROWS, padding_e) + padding_se, 
				 outerE = border_ne + repeat(ROWS, border_e) + border_se;

			push(&outerW[0], &outerW[0] + ROWS + 2);
			for(int col = 0; col < COLS; ++col) {
				if(col) {
					push(&divider[0], &divider[0] + ROWS + 2);
				}
				push(&innerW[0], &innerW[0] + ROWS + 2);

				int span = cellspan + !(col < cellrem);
				std::string word = col<diff ? start[col] : "";
				insert(0, align(word, span, CENTER, border_n));
				for(int row = 0; row < ROWS; ++row) {
					insert(row+1, align(data[row*COLS+col], 
								span, CENTER, space));
				}
				insert(ROWS+1, align("", span, LEFT, border_s));
				push(&innerE[0], &innerE[0] + ROWS + 2);
				level();
			}
			push(&outerE[0], &outerE[0] + ROWS + 2);
			return level();
		}
		template<unsigned int H2, typename C2>
		SELF& insert(int offset, const Printer<H2, C2> &p) {
			for(int row = 0; row < H2; ++row) {
				insert(row+offset, p.lines[row].str());
			}
			return self();
		}
	private:
		SELF& self(void) {
			return *static_cast<SELF*>(this);
		}
	};

	template<unsigned int H> struct NoCRTP:
		public Printer<H, NoCRTP<H>> {
			typedef Printer<H,NoCRTP<H>> SELF;
	};

	int split(std::string const& line, std::vector<std::string> &words);
}

#endif
