#include <iostream>
#include <complex>
#include <chrono>
#include <string>
#include <fstream>

const char* path = { "mendel.ppm" };

/*
360p = 480 x 360
480p = 858 x 480 - SD
720p = 1280 x 720 - HDready
1080p = 1920 x 1080 - FullHD.
2160p = 3860 x 2160 - Ultra-HD / 4K
*/
int width = 2000;
int height = width;

long double x = -1.45;
long double y = 0;

int N = 2000;
int zoom = 8;

long double wradius = 2 / pow(2, zoom);
long double hradius = wradius * height / width;
long double two_wradius = wradius * 2;
long double two_hradius = hradius * 2;

float K = log(2);
int P = 1000000;

bool remove_file(const char* path) {
	if (std::remove(path) != 0)
		return true;
	else return false;
}

int option1() {
	auto tp1 = std::chrono::system_clock::now();
	remove_file(path);
	std::ofstream file;
	file.open(path, std::fstream::app);
	file << "P3" << " " << width << " " << height << " " << "255" << "\n";

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c((x - wradius) + two_wradius * i / width, (y - hradius) + two_hradius * j / height);
			std::complex<long double> z = 0;
			int n = 0;
			while (std::real(z) * std::real(z) + std::imag(z) * std::imag(z) < 4 && n < N) {
				z = z * z + c;
				n++;
			}
			float color = n / N * 255;
			file << " " << color << " " << color << " " << color << " ";
		}
		file << "\n";
		/*if (int(j % 10) == 0)
			std::cout << j << " / " << height << std::endl;*/
	}
	auto tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	float fElapsedTime = elapsedTime.count();
	std::cout << "option 1\t" << fElapsedTime /*<< "s"*/ << std::endl;
	return 0;
}

int option2() {
	auto tp1 = std::chrono::system_clock::now();
	remove_file(path);
	std::string file = "";
	file.append("P3 " + std::to_string(width) + " " + std::to_string(height) + " 255\n");

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			std::complex<long double> c((x - wradius) + two_wradius * i / width, (y - hradius) + two_hradius * j / height);
			std::complex<long double> z = 0;
			int n = 0;
			while (std::real(z) * std::real(z) + std::imag(z) * std::imag(z) < 4 && n < N) {
				z = z * z + c;
				n++;
			}
			float color = n / N * 255;
			file.append(" " + std::to_string(color) + " " + std::to_string(color) + " " + std::to_string(color) + " ");
		}
		file.append("\n");
		/*if (int(j % 10) == 0)
			std::cout << j << " / " << height << std::endl;*/
	}
	std::ofstream outfile;
	outfile.open(path, std::fstream::app);
	outfile << file;
	auto tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	float fElapsedTime = elapsedTime.count();
	std::cout << "option 2\t" << fElapsedTime /*<< "s"*/ << std::endl;
	return 0;
}

int option3() {
	auto tp1 = std::chrono::system_clock::now();
	remove_file(path);
	std::ofstream file;
	file.open(path, std::fstream::app);
	file << "P3 " << std::to_string(width) << " " << std::to_string(height) << " 255\n";
	for (int j = 0; j < height; j++) {
		std::string line = "";
		for (int i = 0; i < width; i++) {
			std::complex<long double> c((x - wradius) + two_wradius * i / width, (y - hradius) + two_hradius * j / height);
			std::complex<long double> z = 0;
			int n = 0;
			while (std::real(z) * std::real(z) + std::imag(z) * std::imag(z) < P && n < N) {
				z = z * z + c;
				n++;
			}
			float V = log(std::real(z) * std::real(z) + std::imag(z) * std::imag(z)) / pow(2,n);
			/*float r = (0.5 + std::sin(n) * std::cos(n)) * 255;*/
			float r = (cos(V) + 1) * (255 / 2);
			float g = (sin(V) + 1) * (255 / 2);
			float b = (cos(V) * sin(V) + 1) * (255 / 2);
			line.append(" " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ");
		}
		file << line + "\n";
		/*if (int(j % 10) == 0)
			std::cout << j << " / " << height << std::endl;*/
	}
	auto tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	float fElapsedTime = elapsedTime.count();
	std::cout << "option 3\t" << fElapsedTime << "s" << std::endl;
	return 0;
}

int option4() {
	auto tp1 = std::chrono::system_clock::now();
	remove_file(path);
	std::ofstream file;
	file.open(path, std::fstream::app);
	file << "P3 " << std::to_string(width) << " " << std::to_string(height) << " 255\n";
	for (int j = 0; j < height; j++) {
		std::string line = "";
		for (int i = 0; i < width; i++) {
			std::complex<long double> c((x - wradius) + two_wradius * i / width, (y - hradius) + two_hradius * j / height);
			std::complex<long double> z = 0;
			int n = 0;
			int power = 1;
			while (std::real(z) * std::real(z) + std::imag(z) * std::imag(z) < P && n < N) {
				z = z * z + c;
				n++;
				power *= 2;
			}
			long double abs = std::real(z) * std::real(z) + std::imag(z) * std::imag(z);
			if (/*abs < P*/false) {
				line.append(" " + std::to_string(220) + " " + std::to_string(220) + " " + std::to_string(220) + " ");
			}
			else {
				float V = log(abs) / power;
				float r = (cos(10 * log(V) / K) + 1) * 255 / 2;
				/*float g = (sin(V * K) + 1) * (255 / 2);
				float b = (cos(V * K) * sin(V * K) + 1) * (255 / 2);*/
				line.append(" " + std::to_string(r) + " " + std::to_string(r) + " " + std::to_string(r) + " ");
			}
		}
		file << line + "\n";
		/*if (int(j % 10) == 0)
			std::cout << j << " / " << height << std::endl;*/
	}
	auto tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	float fElapsedTime = elapsedTime.count();
	std::cout << "option 4\t" << fElapsedTime << "s" << std::endl;
	return 0;
}

int main() {
	option4();
	/*option3();
	option2();
	option1();*/
	return 0;
}

/*
bilan:

option 1 : la plus lente de toutes , durée expodentielle

option 2 : la plus courte de toutes, ne marche pas a partir d'une certaine valeure //stack overflow

option 3 : la meilleur : plus rapide que l'option 1 mais marche tout le temps //stack overflow a partir d'une certaine valeur aussi mais elle est le carré de la valeur d'option 2


PROCHAINE ETAPE:

-les fonctions couleurs, comment les opti

-comment utiliser les differents cores, threads et SIMB
*/