#include "lodepng.h"
#include "SimplexNoise.h"
#include <vector>

/*Quick test class to demonstrate the use of SimplexNoise.cpp.
Simply generates a 500x500 vector of doubles using the 2D simplex
noise algorithm, then uses Lodepng (a lightweight, header only PNG
library) to write the values to value (after normalisation to range 0-255)*/

int main() {
	/*Generate noise*/
	double min = 9999;
	double max = -9999;
	int width, height = 0;
	SimplexNoise sn = SimplexNoise(100, 0.5, 1, 1, 5);
	std::vector<std::vector<double>> result(500, std::vector<double>(500));
	for (int i = 0; i < 500; ++i) {
		for (int j = 0; j < 500; ++j) {
			double res = sn.NoiseAt(i,j);
			if (res > max) { max = res;}
			if (res < min) { min = res;}
			result[i][j] = res;
		}
	}

	std::cout << "Min: " << min << std::endl;
	std::cout << "Max: " << max << std::endl;
	std::cout << "Range: " << max-min << std::endl;
	
	/*Write to png file*/
	std::cout << "Generating data for image.png" << std::endl;
	std::vector<unsigned char> image;
	image.resize(500 * 500 * 4);
	for (int i = 0; i < 500; ++i) {
		for (int j = 0; j < 500; ++j) {
			unsigned char pix = ((result[i][j] - min) / (max-min)) * 255;

			image[4 * 500 * i + 4 * j + 0] = pix;
			image[4 * 500 * i + 4 * j + 1] = pix;
			image[4 * 500 * i + 4 * j + 2] = pix;
			image[4 * 500 * i + 4 * j + 3] = 255;
		}
	}
	std::cout << "Writing image.png" << std::endl;
	unsigned error = lodepng::encode("test.png", image, 500, 500);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	std::cout << "Program finished" << std::endl;
	system("pause");
	return 0;
}