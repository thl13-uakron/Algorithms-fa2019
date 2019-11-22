#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// representation of pixel color values
typedef int pixel_t;

// representation for image matrices
// given the first index as the x-coordinate
// and the second index as the y-coordinate
typedef std::vector< std::vector<pixel_t> > pixelMatrix;

// algorithm methods

// helper function to parse through comments in file stream
// set s to next space-separated value that isn't commented out
void ignore_comments(std::ifstream &infileStream, std::string &s) {
	while (!infileStream.eof()) {
		infileStream >> s;
		// ignore comments
		if (s[0] == '#') {
			infileStream.ignore(INT_MAX, '\n');
		}
		else {
			// read until non-comment detected
			break;
		}
	}
}

// get pixel matrix from .pgm file
pixelMatrix read_pixel_matrix(std::string infileName) {
	// open filestream
	std::ifstream infileStream(infileName);

	std::string s; // hold last string read from file

	// read past header line
	ignore_comments(infileStream, s);

	// read dimensions
	ignore_comments(infileStream, s);
	int x_size = stoi(s);
	ignore_comments(infileStream, s);
	int y_size = stoi(s); 

	// create matrix
	pixelMatrix matrix;
	matrix.resize(x_size);

	// read pixels
	for (int i = 0; i < x_size; ++i) {
		matrix[i].resize(y_size);
		for (int j = 0; j < y_size; ++j) {
			ignore_comments(infileStream, s);
			matrix[i][j] = stoi(s);
		}
	}

	// close filestream and return matrix
	infileStream.close();
	return matrix;
}

// calculate energy value corresponding to each pixel in matrix
void set_pixel_energy(pixelMatrix &energy_matrix, pixelMatrix &pixel_matrix, int i, int j) {
	energy_matrix[i][j] = 0;
	if (i > 0) {
		energy_matrix[i][j] += abs(pixel_matrix[i][j] - pixel_matrix[i - 1][j]);
	}
	if (i < energy_matrix.size() - 1) {
		energy_matrix[i][j] += abs(pixel_matrix[i][j] - pixel_matrix[i + 1][j]);
	}
	if (j > 0) {
		energy_matrix[i][j] += abs(pixel_matrix[i][j] - pixel_matrix[i][j - 1]);
	}
	if (j < energy_matrix[i].size() - 1) {
		energy_matrix[i][j] += abs(pixel_matrix[i][j] - pixel_matrix[i][j + 1]);
	}
}
pixelMatrix get_energy_matrix(pixelMatrix &pixel_matrix) {
	// create matrix
	pixelMatrix energy_matrix;

	// get dimensions
	int x_size = pixel_matrix.size();
	int y_size = pixel_matrix[0].size();
	energy_matrix.resize(x_size);

	// fill values
	for (int i = 0; i < x_size; ++i) {
		energy_matrix[i].resize(y_size);
		for (int j = 0; j < y_size; ++j) {
			set_pixel_energy(energy_matrix, pixel_matrix, i, j);
			std::cout << energy_matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return energy_matrix;
}

// find and remove vertical seams from the pixel matrix

// find and remove horizontal seams from the pixel matrix

// write pixel matrix to .pgm file
void write_pixel_matrix(std::string outfileName, pixelMatrix &pixel_matrix) {
	// open filestream
	std::ofstream outfileStream(outfileName);

	// write header line
	outfileStream << "P2" << '\n';

	// write dimensions
	int x_size = pixel_matrix.size();
	int y_size = pixel_matrix[0].size();
	outfileStream << x_size << " " << y_size << '\n';

	// write pixel values
	for (int i = 0; i < x_size; ++i) {
		for (int j = 0; j < y_size; ++j) {
			outfileStream << pixel_matrix[i][j] << ' ';
		}
	}

	// cloxe filestream
	outfileStream.close();
}

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "Too few command line arguments detected. Check readme.txt for execution instructions." << std::endl;
	}
	else {
		std::string infileName = argv[1]; // name of input file

		std::string outfileName = infileName; // name of output file
		outfileName.insert(outfileName.find(".pgm"), "_modified");

		int verticalSeams = atoi(argv[2]); // number of vertical seams to remove
		int horizontalSeams = atoi(argv[3]); // number of horizontal seams to remove

		pixelMatrix pixel_matrix = read_pixel_matrix(infileName); // get pixel color values
		pixelMatrix energy_matrix = get_energy_matrix(pixel_matrix); // get pixel energy values

		write_pixel_matrix(outfileName, pixel_matrix); // write result to output file

		return 0; 
	}
}