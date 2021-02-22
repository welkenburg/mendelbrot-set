#include <iostream>
#include <complex>
#include <chrono>
#include "bmpCreator.h"

/*
360p = 480 x 360
480p = 858 x 480 - SD
720p = 1280 x 720 - HDready
1080p = 1920 x 1080 - FullHD
1200p = 1920 x 1200 - FullHD
1440p = 2560 x 1440 - Quad HD
1600p = 2560 x 1600 - Quad HD
2160p = 3840 x 2160 - 4K
2880p = 5120 x 2880 - 5K
4320p = 7689 x 4320 - 8K
6480p = 11534 x 6480 - 12K
8640p = 15360 x 8640 - 16K
*/

const int width = 1280;
const int height = 720;
char imageBuffer[height * (width * 3 + width % 4)];


long double centerPoint[2] = {0,0};
float zoom = -0.5;

long double startX = centerPoint[0] - 2 / pow(2, zoom);
long double startY = centerPoint[1] - (2 / pow(2, zoom)) * height / width;
long double coeficient = 4 / (pow(2, zoom) * width);

int maxValue = 4; /* R * R */
int maxInteration = 1000;


long double epsilon = 0.001;
float thickness = 1;
float rgb[3] = { 0.5,0.3,0.8 };


std::complex<long double> one = 1;
std::complex<long double> two = 2;
std::complex<long double> boundingCoef = thickness* coeficient;

const char* path = { "mendelc=6.bmp" };

//char setColor[3] = { (char)255*rgb[0],(char)255 * rgb[1],(char)255 * rgb[2] };
char setColor[3] = { 220,220,220 };
char boundaryColor[3] = { 0,0,0 };
//char boundaryColor[3] = { (char)255 * rgb[2],(char)255 * rgb[1],(char)255 * rgb[0] };
char outsideColor[3] = { 255,255,255 };

long double complexAbsolute(std::complex<long double> z) {
	return std::real(z) * std::real(z) + std::imag(z) * std::imag(z);
}

char * basic(long double n,bool in = false) {
	if(n < maxInteration && in == false)
		return outsideColor;
	else
		return setColor;
}

char * escapedTime(long double n, bool in = false) {
	if (n == maxInteration || in)
		return setColor;
	else
		return new char[3]{ (char)((cos(n * rgb[0]) + 1) * 255 / 2),
							(char)((cos(n * rgb[1]) + 1) * 255 / 2),
							(char)((cos(n * rgb[2]) + 1) * 255 / 2) };
}

char *  potential(long double n, long double abs, bool in = false) {
	if (n == maxInteration || in)
		return setColor;
	else {
		float K = 1; //K est une variable
		long double V = log(abs) / pow(2, n);
		long double x = log(V) / K;
		return new char[3]{ (char)((cos(x * rgb[0]) + 1) * 255 / 2),
							(char)((cos(x * rgb[1]) + 1) * 255 / 2),
							(char)((cos(x * rgb[2]) + 1) * 255 / 2) };
	}
}

void mendelbrot() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c(startX + coeficient * i, startY + coeficient * j);
			std::complex<long double> z = c;
			int n = 0;
			while (complexAbsolute(z) < maxValue && n < maxInteration) {
				z = z * z + c;
				n++;
			}
			char* colors = potential(n, complexAbsolute(z));
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 0] = colors[0];
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 1] = colors[1];
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 2] = colors[2];
		}
	}
}

void hyperbolicComponents() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c(startX + coeficient * i, startY + coeficient * j);
			std::complex<long double> z = c;
			std::complex<long double> derivative = 1;
			bool in = false;
			int finalN = maxInteration;
			for (int n = 0; n < maxInteration; n++) {
				if (complexAbsolute(derivative) < epsilon * epsilon) {
					in = true;
					finalN = n;
					break;
				}
				if (complexAbsolute(z) > maxValue) {
					finalN = n;
					break;
				}
				derivative = derivative * two * z;
				z = z * z + c; //z = z * z * z * z * z * z * z * z * z + c;
			}
			char* colors = basic(finalN, in);
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 0] = colors[0];
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 1] = colors[1];
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 2] = colors[2];
		}
	}
}

void bondaryDetection() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c(startX + coeficient * i, startY + coeficient * j);
			std::complex<long double> z = c;
			std::complex<long double> derivative = 1;
			bool in = true;
			int finalN = maxInteration;
			for (int n = 0; n < maxInteration; n++) {
				if (complexAbsolute(z) > maxValue) {
					finalN = n;
					in = false;
					break;
				}
				derivative = derivative * two * z + one;
				z = z * z + c;
			}
			if (in) {
				imageBuffer[j * (width * 3 + width % 4) + i * 3 + 0] = setColor[0];
				imageBuffer[j * (width * 3 + width % 4) + i * 3 + 1] = setColor[1];
				imageBuffer[j * (width * 3 + width % 4) + i * 3 + 2] = setColor[2];
			}
			else {
				long double sqrModulusZ = complexAbsolute(z);
				long double sqrModulusD = complexAbsolute(boundingCoef * derivative);
				if (sqrModulusZ * log(sqrModulusZ) * log(sqrModulusZ) < sqrModulusD) {
					imageBuffer[j * (width * 3 + width % 4) + i * 3 + 0] = boundaryColor[0];
					imageBuffer[j * (width * 3 + width % 4) + i * 3 + 1] = boundaryColor[1];
					imageBuffer[j * (width * 3 + width % 4) + i * 3 + 2] = boundaryColor[2];
				}
				else {
					char* colors = potential(finalN, complexAbsolute(z));
					imageBuffer[j * (width * 3 + width % 4) + i * 3 + 0] = colors[0];
					imageBuffer[j * (width * 3 + width % 4) + i * 3 + 1] = colors[1];
					imageBuffer[j * (width * 3 + width % 4) + i * 3 + 2] = colors[2];
				}
			}
		}
	}
}

int orbits_detection() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c(startX + coeficient * i, startY + coeficient * j);
			std::complex<long double> z = c;
			for (int n = 0; n < maxInteration; n++) {
				long double abs = std::real(z) * std::real(z) + std::imag(z) * std::imag(z);
				if (abs > maxValue) {
					break;
				}
				z = z * z + c;
			}
		}
	}
	return 0;
}

int main() {
	auto ti = std::chrono::system_clock::now();

	hyperbolicComponents();
	bmpImage set(width, height, imageBuffer);
	set.save(path);

	auto tf = std::chrono::system_clock::now();

	std::chrono::duration<float> deltaTime = tf - ti;
	float fDeltaTime = deltaTime.count();
	std::cout << "program finished in " << fDeltaTime << "s" << std::endl;

	/*
	
	orbits_detection a faire ou pas ?--> voir desktop/"iterations graph.py"

	*/
	return 0;
}