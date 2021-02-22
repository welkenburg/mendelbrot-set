#pragma once
#include<iostream>
#include<fstream>
#include<chrono>

class bmpImage {
private:
	char fileHeader[14] = { (char)66,(char)77,(char)255,(char)255,(char)255,(char)255,(char)0,(char)0,(char)0,(char)0,(char)15,(char)0,(char)0,(char)0 };
	char imageHeader[40] = { (char)40,(char)0,(char)0,(char)0,(char)255,(char)255,(char)255,(char)255,(char)255,(char)255,(char)255,(char)255,(char)1,(char)0,(char)24,(char)0,(char)0,(char)0,(char)0,(char)0,(char)255,(char)255,(char)255,(char)255,(char)35,(char)46,(char)0,(char)0,(char)35,(char)46,(char)0,(char)0,(char)0,(char)0,(char)0,(char)0,(char)0,(char)0,(char)0,(char)0 };

	void replaceHeader(char* buffer, int x, int position) {
		for (int i = 3; i >= 0; i--) {
			buffer[position + i] = (x >> i * 8) & 0xFF;
		}
	}

public:
	int width;
	int height;
	char * pixelBuffer;

	bmpImage(int fwidth, int fheight, char* fpixelBuffer) {
		width = fwidth;
		height = fheight;
		pixelBuffer = fpixelBuffer;
	}

	void save(const char* path) {
		replaceHeader(fileHeader, 54 + width * height * 3, 2);
		replaceHeader(imageHeader, width, 4);
		replaceHeader(imageHeader, height, 8);
		replaceHeader(imageHeader, width * height * 3, 20);

		std::ofstream out(path, std::ios::out | std::ios::binary);
		if (!out) std::cout << "Error : cannot write to the specified path" << std::endl;
		else {
			out.write(fileHeader, sizeof(fileHeader));
			out.write(imageHeader, sizeof(imageHeader));
			out.write(pixelBuffer, height * (width * 3 + width % 4));
			out.close();
		}
	}
};

class time {
public:
	std::chrono::system_clock::time_point initalTime;
	time() {
		initalTime = std::chrono::system_clock::now();
	}

	void stop() {
		auto finalTime = std::chrono::system_clock::now();
		std::chrono::duration<float> deltaTime = finalTime - initalTime;
		float fDeltaTime = deltaTime.count();
		std::cout << "finished in " << fDeltaTime << "s" << std::endl;
	}
};