#include "Vector3.h"
#include "Vector2.h"
#include <vector>
/* 
C++ implementation that creates noisy terrain images using fractal brownian
motion, in combination with the simplex noise algorithm for generation of 
underlying octaves.

Simplex Octave generation based on code by Stefan Gustavson, originally written in Java.
Code contains further optimisations that were made by Peter Eastman.

Stefan Gustavson: stegu@itn.liu.se
Peter Easeman: peastman@drizzle.stanford.edu
*/

class SimplexNoise {
public:
	SimplexNoise(double featureSize, double persistence = DEF_PERSISTENCE, int octaves = DEF_OCTAVES, int seed = 0);
	double NoiseAt(int x, int y);
	double Noise(double xin, double yin) const;

private:
	static const int ZERO_SEED;
	static const int NUMBER_OF_SWAPS;
	static const Vector3i grad3[12];

	static const double F2;
	static const double G2;

	static const short p_supply[256];
	short p[256];
	short perm[512];
	short permMod12[512];

	double dot(const Vector3i& a, const Vector2d& b) const;
	double CornerContribution(int gradIndex, const Vector2d& xy) const;

	static const double DEF_PERSISTENCE;
	static const double DEF_OCTAVES;
	static const double lacunarity; //leave fixed as 2.0
	std::vector<double> frequency;
	std::vector<double> amplitude;
};