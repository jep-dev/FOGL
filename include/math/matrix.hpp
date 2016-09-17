#ifndef MATRIX_HPP
#define MATRIX_HPP

namespace Math {
	template<int W, int H, typename R>
	static auto transpose(R (&src)[W*H], R (&dest)[H*W]) -> decltype(dest) {
		for(int row = 0; row < H; row++) {
			for(int col = 0; col < W; col++) {
				dest[row*W+col] = src[col*H+row];
			}
		}
		return dest;
	}
}

#endif
