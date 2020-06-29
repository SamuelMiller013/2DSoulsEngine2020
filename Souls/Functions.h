#pragma once

class Functions {
public:
	static int sgn(float n) {
		return (n < 0) ? -1 : ((n > 0) ? 1 : 0);
	}

	static float ApproachValue(float val, float tar, float inc) {
		if (val > tar) {
			val -= inc;
			if (val <= tar) {
				val = tar;
			}
		}
		else if (val < tar) {
			val += inc;
			if (val >= tar) {
				val = tar;
			}
		}

		return val;
	}

	static float Clamp(float val, float min, float max) {
		if (val < min) {
			val = min;
		}
		else if (val > max) {
			val = max;
		}

		return val;
	}
};