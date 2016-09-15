#ifndef MATRIX_HPP
#define MATRIX_HPP

namespace Math {
	template<int W, int H, typename R>
	static void transpose(R (&src)[W*H], R (&dest)[H*W]) {
		for(int row = 0; row < H; row++) {
			for(int col = 0; col < W; col++) {
				dest[row*W+col] = src[col*H+row];
			}
		}
	}
}

#endif
