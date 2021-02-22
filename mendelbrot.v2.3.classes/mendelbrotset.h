#pragma once

#include <iostream>
#include <complex>
#include <fstream>
#include <chrono>
#include <string>

class set {
private:
	//name of the file the image will be saved in
	const char* file_path;

	//file object
	std::ofstream file;

	//size of the picture
	int window_width;
	int window_height;

	int iterations_max;

	// radius = 2 / (2 ** zoom)
	int zoom;
	long double width_radius;
	long double height_radius;

	// variables to make the program go faster
	long double two_width_radius;
	long double two_height_radius;

	//coordinates of the center of the image
	long double x;
	long double y;

	//above this number the point "goes to infinity" -- default is 2
	int P;

	//variable that change the frequency of the color cycle in potentialColoring()
	float K;

	//change the estimated area said "inside the set" when using the inside detection method
	long double epsilon;

	//color of the set
	float in_r=0, in_g=0, in_b=0;
	std::string set_color = "";

	//change the colors of the outside
	float out_r=255, out_g=255, out_b=255;
	std::string out_color = "";

	//constant values
	const std::complex<long double> one = 1;
	const std::complex<long double> two = 2;

	void change_colors() {
		set_color = " " + std::to_string(in_r) + " " + std::to_string(in_g) + " " + std::to_string(in_b) + " ";
		out_color = " " + std::to_string(out_r) + " " + std::to_string(out_g) + " " + std::to_string(out_b) + " ";
	}

	void change_radius(int zoom) {
		width_radius = 2 / pow(2, zoom);
		two_width_radius = 2 * width_radius;

		height_radius = width_radius * window_height / window_width;
	}

	bool remove_file(const char* path) {
		if (std::remove(path) != 0)
			return true;
		return false;
	}

	std::string basicColoring(long double n, long double abs, bool in = false) {
		if (n < iterations_max && in == false)
			return out_color;
		else
			return set_color;
	}

	std::string escapeTimeBasedColoring(long double n, long double abs, bool in = false) {
		if (n == iterations_max || in == true)
			return set_color;
		else
			return " " + std::to_string((cos(n * out_r) + 1) * 255 / 2) + " " + std::to_string((cos(n * out_g) + 1) * 255 / 2) + " " + std::to_string((cos(n * out_b) + 1) * 255 / 2) + " ";
	}

	std::string potentialColoring(long double n, long double abs, bool in = false) {
		if (n == iterations_max || in == true)
			return set_color;
		else {
			long double V = log(abs) / pow(2, n);
			long double x = log(V) / K;
			return " " + std::to_string((cos(x * out_r) + 1) * 255 / 2) + " " + std::to_string((cos(x * out_g) + 1) * 255 / 2) + " " + std::to_string((cos(x * out_b) + 1) * 255 / 2) + " ";
		}
	}

public:
	void init(int windows_width, int windows_height, const char* path);
	void color(/*type, vect3d inside_color, vect3d outside_color*/);
	void setparameters(long double x, long double y, int zoom, int P, int iterations_max, long double epsilon);
	void draw();
};

void set::init(int width = 100, int height = 100, const char* path = { "mendelbrotset.ppm" }) {
	//check if the path contain .ppm if not, add it
	window_width = width;
	window_height = height;
	file_path = path;
	remove_file(path);

	//ppm header : "P3 width height color_type"
	file.open(path, std::fstream::app);
	file << "P3 " << std::to_string(window_width) << " " << std::to_string(window_height) << " 255\n";
}

void set::color(/*type, vect3d inside_color, vect3d outside_color*/ ) {
	change_colors();
}

void set::setparameters(long double cx = 0, long double cy = 0, int czoom = 0, int cP = 2, int citerations_max = 1000, long double cepsilon = 0.1) {
	x = cx;
	y = cy;
	change_radius(zoom = czoom);
	std::cout << width_radius << std::endl;
	P = cP;
	iterations_max = citerations_max;
	epsilon = cepsilon;
}

void set::draw() {
	for (int j = 0; j < window_height; j++) {
		std::string line = "";
		for (int i = 0; i < window_width; i++) {
			std::complex<long double> c((x - width_radius) + two_width_radius * i / window_width, (y - height_radius) + two_height_radius * j / window_height);
			std::complex<long double> z = c;
			std::complex<long double> derivative = 1;
			bool in = false;
			int iterations = iterations_max;
			for (int n = 0; n < iterations_max; n++) {
				if (std::real(derivative) * std::real(derivative) + std::imag(derivative) * std::imag(derivative) < epsilon * epsilon) {
					in = true;
					iterations = n;
					break;
				}
				if (std::real(z) * std::real(z) + std::imag(z) * std::imag(z) > P) {
					iterations = n;
					break;
				}
				derivative = derivative * two * z;
				z = z * z + c;
			}
			//what type of color did the user choose
			//maybe do that in an other function and return n and z here -- call this function a different name if so
			line.append(basicColoring(iterations, std::real(z) * std::real(z) + std::imag(z) * std::imag(z), in));
		}
		file << line + "\n";
	}
}