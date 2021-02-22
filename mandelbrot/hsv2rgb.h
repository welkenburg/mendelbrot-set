namespace c {
	float k(float n, float h) {
		return (int)(n + h * 360 / (255 * 60)) % 6;
	}
	float max(float x, float y) {
		if (y > x) {
			return y;
		}
		else {
			return x;
		}
	}

	float min(float x, float y, float z) {
		if (y < x && y < z) {
			return y;
		}
		else if (z < x && z < y) {
			return z;
		}
		else {
			return x;
		}
	}

	float f(float n, float v, float s, float h) {
		return v / 255 - (v / 255) * (s / 255) * max(min(k(n, h), 4 - k(n, h), 1), 0);
	}
	
	void hsv2rgb(float h, float s, float v, float *r , float *g, float *b) {
		*r = 255 * f(5, v, s, h);
		*g = 255 * f(3, v, s, h);
		*b = 255 * f(1, v, s, h);
	}
}