#define _USE_MATH_DEFINES
#include <complex>
#include <iostream>
#include <fstream>
#include <string>

std::string path = "mendel.ppm";

/*
360p = 480 x 360
480p = 858 x 480 - SD
720p = 1280 x 720 - HDready
1080p = 1920 x 1080 - FullHD.
2160p = 3860 x 2160 - Ultra-HD / 4K
*/
int width = 480;
int height = 360;
int N = 200;

int P = 100000000;

//long double x = -0.77468061062694;
//long double y = -0.1374168856037867;

//long double x = 0.5;
//long double y = 0;

long double x = 0;
long double y = 0;

long double zoom = -2;
long double radius = 2 / pow(2, zoom);

long double K = std::log(2);
//long double pi = 3.141592654;

int remove(std::string path) {
	const char* c = path.c_str();
	if (std::remove(c) != 0) {
		std::cout << "[REMOVING] operation failed" << std::endl;
	}
	else {
		std::cout << "[REMOVING] file has been removed" << std::endl;
	}
	return 0;
}

int color(long double V, long double *r, long double *g, long double *b) {
	long double x = std::log(V) / K;
	*r = 255 * ((1 + std::cos((1 / std::log(2)) * x)) / 2);
	*g = 255 * ((1 + std::cos(((1 / (3 * std::sqrt(2))) / std::log(2)) * x)) / 2);
	*b = 255 * ((1 + std::cos(((1 / std::powl(7.3,1/8)) / std::log(2)) * x)) / 2);
	return 0;
}

int main() {
	std::cout << M_PI << std::endl;
	remove(path);
	std::string space = " ";
	std::string enter = "\n";
	std::ofstream myfile;
	myfile.open(path, std::fstream::app);
	myfile << "P3 ";
	myfile << std::to_string(width) + space;
	myfile << std::to_string(height) + space;
	myfile << std::to_string(255) + enter;

	for (float i = 0; i < height; i++) {
		if ((int)i % 10 == 0) {
			std::cout << "[DRAWING]" << i/(float)height * 100 << "%" << std::endl;
		}
		for (float j = 0; j < width; j++) {
			long double cx = (x - radius) + 2 * radius * j / width;
			long double cy = (y - radius*height/width) + 2 * radius*height/width * i / height;
			std::complex<long double> c(cx, cy);
			std::complex<long double> z = 0.;
			long double pow = 1.;
			for (int n = 0; n < N; n++) {
				long double R2 = std::sqrt(std::real(z) * std::real(z) + std::imag(z) * std::imag(z));
				if (R2 > P) {
					long double V = std::log(R2) / pow;
					long double r = n / N * 255, g = n / N * 255, b = n / N * 255;
					//long double r, g, b;
					//color(V, &r, &g, &b);
					myfile << space + std::to_string(r) + space + std::to_string(g) + space + std::to_string(b) + space;
					goto end;
				}
				z = z + c;
				pow = pow * 2;
			}
			myfile << space + std::to_string(200) + space + std::to_string(200) + space + std::to_string(200) + space;
			end: {};
		}
		myfile << enter;
	}
	std::cout << "[FINISHED] set created with success" << std::endl;
	return 0;
}