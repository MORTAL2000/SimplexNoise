#include "lodepng.h"
#include "SimplexNoise.h"
#include <vector>

/*Quick test class to demonstrate the use of SimplexNoise.cpp.
Simply generates a 512*512 vector of doubles using the 2D simplex
noise algorithm, then uses Lodepng (a lightweight, header only PNG
library) to create a greyscale image (after normalisation to range 0-255)*/

int main() {
	double min = 9999;
	double max = -9999;

	int width = 512;
	int height = 512;
	std::string filename("test.png");

	/*	Feature Size: 150
		Persistence: 0.65
		#Octaves: 8
		Seed: 5000
		*/
	SimplexNoise sn = SimplexNoise(150, 0.65, 8, 5000);
	std::vector<std::vector<double>> noise(height, std::vector<double>(width));

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			double res = sn.NoiseAt(i,j);
			if (res > max) { max = res;}
			if (res < min) { min = res;}
			noise[i][j] = res;
		}
	}

	std::cout << "Min: " << min << std::endl;
	std::cout << "Max: " << max << std::endl;
	double range = max-min;
	std::cout << "Range: "<< range << std::endl;



	/*Write to png file*/
	std::cout << "Generating data for image.png" << std::endl;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			unsigned char pix = ((noise[i][j] - min) / range) * 255;

			image[4 * width * i + 4 * j + 0] = pix;
			image[4 * width * i + 4 * j + 1] = pix;
			image[4 * width * i + 4 * j + 2] = pix;
			image[4 * width * i + 4 * j + 3] = 255;
		}
	}
	std::cout << "Writing image.png" << std::endl;
	unsigned error = lodepng::encode(filename, image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	std::cout << "Program finished" << std::endl;
	system("pause");
	return 0;
}