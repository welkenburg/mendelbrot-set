#include <iostream>
#include <complex>
#include <chrono>

/*
360p = 480 x 360
480p = 858 x 480 - SD
720p = 1280 x 720 - HDready
1080p = 1920 x 1080 - FullHD.
2160p = 3860 x 2160 - Ultra-HD / 4K
*/

const int width = 858;
const int height = 480;
char imageBuffer[height * (width * 3 + width % 4)];

long double x = -1.5;
long double y = 0;


int zoom = 5;
long double startX = x - 2 / pow(2, zoom);
long double startY = y - (2 / pow(2, zoom)) * height / width;
long double coeficient = 4 / (pow(2, zoom) * width);

int maxValue = 100000; /* R * R */
int maxInteration = 1000;

long double epsilon = 0.001;

float rgb[3] = { 1,0.5,0.3 };


std::complex<long double> one = 1;
std::complex<long double> two = 2;
std::complex<long double> thickness = 1;

const char* path = { "mendel.bmp" };
char setColor[3] = { 0,0,255 };

long double complexAbsolute(std::complex<long double> z) {
	return std::real(z) * std::real(z) + std::imag(z) * std::imag(z);
}

char * basic(long double n,bool in = false) {
	if(n < maxInteration && in == false)
		return new char[3]{0,0,0};
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
			char * colors = basic(n,complexAbsolute(z));
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
				z = z * z + c;
			}
			char* colors = escapedTime(finalN,in);
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 0] = colors[0];
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 1] = colors[1];
			imageBuffer[j * (width * 3 + width % 4) + i * 3 + 2] = colors[2];
		}
	}
}

void bondary_detection_method() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c(startX + coeficient * i, startY + coeficient * j);
			std::complex<long double> z = c;
			std::complex<long double> derivative = 1;
			std::string color = " 100 100 100 ";
			for (int n = 0; n < maxInteration; n++) {
				long double abs = std::real(z) * std::real(z) + std::imag(z) * std::imag(z);
				if (abs > maxValue) {
					std::complex<long double> dn = thickness * derivative;
					color = " 255 255 255 ";
					if (abs * log(abs) * log(abs) < std::real(dn) * std::real(dn) + std::imag(dn) * std::imag(dn)) {
						/*std::cout << abs * log(abs) * log(abs) << "\t < \t" << std::real(dn) * std::real(dn) + std::imag(dn) * std::imag(dn) << std::endl;*/
						color = " 0 0 0 ";
					}
				}
				derivative = derivative * two * z + one;
				z = z * z + c;
			}
			color;
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
	/*bmpImage set(width, height, imageBuffer);
	set.save(path);*/

	auto tf = std::chrono::system_clock::now();

	std::chrono::duration<float> deltaTime = tf - ti;
	float fDeltaTime = deltaTime.count();
	std::cout << "program finished in " << fDeltaTime << "s" << std::endl;

	/*
	
	bondary_detection_method ne marche pas du tout : essayer de coriger les potentiels beugs

	orbits_detection a faire --> voir desktop/"iterations graph.py"

	*/
	return 0;
}