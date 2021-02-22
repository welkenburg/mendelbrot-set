#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include<tuple>
#include <cmath>
#include <math.h>
#include "hsv2rgb.h"

std::string path = "mendelbrot set.ppm";

/*
360p = 480 x 360
480p = 858 x 480 - SD
720p = 1280 x 720 - HDready
1080p = 1920 x 1080 - FullHD.
2160p = 3860 x 2160 - Ultra-HD / 4K
*/
const int width = 1280;
const int height = 720;
const int imax = 10;

//const long double x = -0.7746806106269039;
//const long double y = -0.1374168856037867;

//const long double x = -1.7499576837060935036022145060706997072711057972625207793024283782028600808297280488721867278443170083110054450765565953137974754199999999;
//const long double y = 0.00000000000000000278793706563379402178294753790944364927085054500163081379043930650189386849765202169477470552201325772332454726999999995;

const long double x = 0;
const long double y = 0;

const int zoom = 0;
long double radius = 2 / pow(2, zoom);
const int color_type = 0;

//const float k = 3;
const float border = 2;
const float k = std::log(2);

int remove(std::string path) {
	const char* c = path.c_str();
	if( std::remove(c) != 0){
		std::cout << "[REMOVING] operation failed" << std::endl;
	}
	else {
		std::cout << "[REMOVING] file has been removed" << std::endl;
	}
	return 0;
}

void color(float i, float* r, float* g, float* b) {
	if (color_type == 0) {
		*r = (int)(i * 2) % 255;
		*g = (int)(i * 2) % 255;
		*b = (int)(i * 2) % 255;
	}
	else if (color_type == 1 ) {
		std::tuple <float, float, float> rbg(1 * (1 / k), (1 / (3 * std::sqrtf(2))) * (1 / k), (1 / (7 * std::powf(3, 1 / 8))) * (1 / k));
		*r = 255 * 1 - std::cos(std::get<0>(rbg) * i);
		*g = 255 * 1 - std::cos(std::get<1>(rbg) * i);
		*b = 255 * 1 - std::cos(std::get<2>(rbg) * i);
	}
	else if (color_type == 2 ) {
		if ((int)i % 8 < 4) {
			*r = 255;
			*g = 255;
			*b = 255;
		}
		else {
			*r = 0;
			*g = 0;
			*b = 0;
		}
	}
	else if (color_type == 3 ) {
		*r = 255 - (std::cos(i) * 255 / 2 + 255 / 2) * (1 / (3 * std::sqrtf(2)));
		*g = 255 - std::cos(i) * 255 / 2 + 255 / 2;
		*b = 255-(std::cos(i) * 255 / 2 + 255 / 2) * (1 / k);
	}
}

void mendelbrot() {
	remove(path);
	std::string space = " ";
	std::string enter = "\n";
	std::ofstream myfile;
	myfile.open(path, std::fstream::app);
	myfile << "P3 ";
	myfile << std::to_string(width) + space;
	myfile << std::to_string(height) + space;
	myfile << std::to_string(255) + enter;
	for (int j = 0; j < height; j++)
	{
		if (j * height/100  % 20 == 0) {
			std::cout << "[DRAWING]" << (((float)j / (float)height) * 100) << "%" << std::endl;
		}

		for (int i = 0; i < width; i++) {
			long double c[2] = { x - radius * width / height + (i * radius * width / height * 2 / width),y - radius + (j * radius * 2 / height) };
			int shade = 0;
			long double z[2] = { 0,0 };
			for (int e = 0; e <= imax; e++) {
				long double a = z[0];
				z[0] = z[1] * z[1] * -1 + z[0] * z[0] + c[0];
				z[1] = 2 * a * z[1] + c[1];
				if (z[0] >= border || z[1] >= border) {
					break;
				}
				else {
					shade++;
				}
			}
			float h = (shade * 2) % 255;
			float r, g, b;
			color(h, &r, &g, &b);
			myfile << space + std::to_string(r) + space + std::to_string(g) + space + std::to_string(b) + space;

		}
		myfile << enter;
	}
}

int main() {
	std::cout << "[INFO] x point : "<< x << std::endl;
	std::cout << "[INFO] y point : " << y << std::endl;
	std::cout << "[INFO] radius : " << radius << std::endl;
	std::cout << "[INFO] iterations : " << imax << std::endl;
	mendelbrot();
	std::cout << "[FINISHED] set created with success" << std::endl;
	
	return 0;
}