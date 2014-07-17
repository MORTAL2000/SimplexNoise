#include "SimplexNoise.h"
#include <algorithm>
#include <random>
#include "Vector2.h"

/*const int SimplexNoise::RANDOM_SEED = 0;
const int SimplexNoise::NUMBER_OF_SWAPS = 400;*/
const Vector3i SimplexNoise::grad3[12] = {
	Vector3i(1,1,0), Vector3i(-1,1,0), Vector3i(1,-1,0), Vector3i(-1,-1,0),
	Vector3i(1,0,1), Vector3i(-1,0,1), Vector3i(1,0,-1), Vector3i(-1,0,-1),
	Vector3i(0,1,1), Vector3i(0,-1,1), Vector3i(0,1,-1), Vector3i(0,-1,-1)
};
const short SimplexNoise::p_supply[256] = {
	151,160,137,91,90,15, //this contains all the numbers between 0 and 255, these are put in a random order depending upon the seed
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

const double SimplexNoise::F2 = 0.5 * (sqrt(3.0) - 1.0);
const double SimplexNoise::G2 = (3.0 - sqrt(3.0)) / 6.0;

SimplexNoise::SimplexNoise(int largestFeature, double persistence, int low, int high, int seed) {
	std::copy(p_supply, p_supply + 256, p);
#if 0
	// TO DO : Add seed randomness
	if (seed == RANDOM_SEED) {
		std::random_device rd;
		seed = rd();
	}

	std::mt19937 eng(seed);
	std::uniform_int_distribution<> rand(0,255);
	
	for (int i = 0; i < NUMBER_OF_SWAPS; ++i) {
		int swapFrom = rand(eng);
		int swapTo = rand(eng);

		/*aint nobody got time for XOR swaps
		p[swapFrom] ^= p[swapTo];
		p[swapTo] = p[swapFrom] ^ p[swapTo];
		p[swapFrom] ^= p[swapTo];*/

		short temp = p[swapFrom];
		p[swapFrom] = p[swapTo];
		p[swapTo] = temp;
		std::cout << p[swapFrom] << std::endl;
		std::cout << p[swapTo] << std::endl;
	}
#endif

	for (int i = 0; i < 512; ++i) {
		perm[i] = p[i & 255];
		permMod12[i] = (short)(perm[i] % 12);
	}
}

double SimplexNoise::NoiseAt(int x, int y) {
	/*TO DO: 
		Pre compute freq/amp on class creation?
	*/
	double noise = 0;
	double freq = 1.0 / 80.0;
	double amp = 0.5;

	for (int i = 0; i < 8; ++i) {
		noise += Noise(x * freq, y * freq) * amp;
		freq *= 2.0;
		amp *= 0.5;
	}

	return noise;
}

/*I changed this to use a custom Vector class instead of the POD types
used in the original. It's a tad slower, but helps me read/understand 
it a little better. I'm still not 100% confident with this algorithm.
TO DO: 
	Worth declaring Vecs outside class to prevent con/des each iter?*/
double SimplexNoise::Noise(double xin, double yin) const {
	Vector2d xyin(xin, yin);
    // Skew the input space to determine which simplex cell we're in
	double s = xyin.ComponentSum() * F2; // Hairy factor for 2D
	Vector2d ij = (xyin + s).Floor();
	double t = ij.ComponentSum() * G2;
	Vector2d XY(ij - t); // Unskew the cell origin back to (x,y) space
    // The x,y distances from the cell origin
	Vector2d xy(xyin - XY);
    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    // Offset for second (middle) corner of simplex in (i,j) coords
	Vector2i ij1 = xy.x > xy.y ? Vector2i(1, 0) : Vector2i(0, 1); // lower triangle, XY order: (0,0)->(1,0)->(1,1) : upper triangle, YX order: (0,0)->(0,1)->(1,1)
    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    // Offsets for middle corner in (x,y) unskewed coords
	Vector2d xy1(xy - ij1 + G2);
	// Offsets for last corner in (x,y) unskewed coords
	Vector2d xy2(xy - 1.0 + 2.0 * G2);
    // Work out the hashed gradient indices of the three simplex corners
	Vector2i ij2((int)ij.x & 255, (int)ij.y & 255);
    int gi0 = permMod12[ij2.x+perm[ij2.y]];
    int gi1 = permMod12[ij2.x+ij1.x+perm[ij2.y+ij1.y]];
    int gi2 = permMod12[ij2.x+1+perm[ij2.y+1]];
    // Calculate the contribution from the three corners
	// Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
	return 70.0 * (CornerContribution(gi0, xy) + CornerContribution(gi1, xy1) + CornerContribution(gi2, xy2));;
}

/*Helper functions to cut down the main noise function size*/
double SimplexNoise::CornerContribution(int gradIndex, const Vector2d& xy) const {
	double t = 0.5 - xy.Dot(xy);
	if (t < 0) return 0;
	t *= t;
	return t * t * dot(grad3[gradIndex],xy);
}
/*Dot a 2d/3d vector*/
double SimplexNoise::dot(const Vector3i& a, const Vector2d& b) const {
	return a.x * b.x + a.y * b.y;
}