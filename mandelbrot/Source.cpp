//#include <iostream>
//#include <string>
//#include <fstream>
////#include <iomanip>
//#include "hsv2rgb.h"
//
//std::string path = "mendel.ppm";
//
//float width = 1000;
//float height = width;
//float imax = 1000;
//
//long double x = -1.75;
//long double y = 0;
//
//int resolution = 1;
//int zoom = 5;
////long float radius = 8.094947017729282e-13;
//long double radius = 2 / pow(2,zoom);
//
//int write(std::string path, std::string data) {
//	std::ofstream myfile;
//	myfile.open(path);
//	myfile << data;
//	myfile.close();
//	return 0;
//}
//
//int main() {
//	std::string data = "P3 ";
//	std::string space = " ";
//	data.append( std::to_string( width ) + space );
//	data.append( std::to_string( height ) + space );
//	//data.append( std::to_string( imax ) + "\n" );
//	data.append( std::to_string( 255 ) + "\n" );
//
//	for (int j = 0; j < width; j++)
//	{
//		if (j % 20 == 0) {
//			std::cout << j / width * 100 << "%" << std::endl;
//		}
//
//		for (int i = 0; i < height; i++) {
//			long double c[2] = { x - radius + (i * radius * 2 / width),y - radius + (j * radius * 2 / height) };
//			int shade = 0;
//			long double z[2] = { 0,0 };
//			for (int e = 0; e < imax; e++) {
//				z[0] = pow(z[0], 2) - pow(z[1], 2) + c[0];
//				z[1] = 2 * z[0] * z[1] + c[1];
//				//std::cout << z[0] << "\t\t" << z[1] << std::endl;
//				if (z[0] >= 2 || z[1] >= 2) {
//					break;
//				}
//				else {
//					shade++;
//				}
//			}
//			/*std::cout << shade << std::endl;*/
//			//std::string color = std::to_string(shade) + space + std::to_string(shade) + space + std::to_string(shade);
//			float h = shade % 360;
//			float s = 100;
//			float v = 100;
//			float r, g, b;
//			c::hsv2rgb(h, s, v, &r, &g, &b);
//			std::string color = std::to_string(r) + space + std::to_string(g) + space + std::to_string(b);
//			data.append(color + '\t');
//		}
//		data.append("\n");
//	}
//	/*std::cout << data << std::endl;*/
//	std::cout << "set finished" << std::endl;
//	write(path, data);
//	return 0;
//}
//
////ecrire directement dans le fichier plutot que de storer la string et de tt ecrire a la fin