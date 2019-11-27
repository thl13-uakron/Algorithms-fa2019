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
void ignore_comments(std::ifstream &infileStream, std::string &s);
// get pixel matrix from .pgm file and record color range
pixelMatrix read_pixel_matrix(std::string infileName, int &num_colors);

// print pixel values
void print_pixel_matrix(pixelMatrix &pixel_matrix);

// calculate energy value corresponding to each pixel in matrix
pixelMatrix get_energy_matrix(pixelMatrix &pixel_matrix);

// find and remove vertical seams from the pixel matrix
void remove_vertical_seams(pixelMatrix &pixel_matrix, int seams);

// find and remove horizontal seams from the pixel matrix
void remove_horizontal_seams(pixelMatrix &pixel_matrix, int seams);

// write pixel matrix to .pgm file
void write_pixel_matrix(std::string outfileName, pixelMatrix &pixel_matrix, int &num_colors);

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cout << "Too few command line arguments detected. Check readme.txt for execution instructions." << std::endl;
	}
	else {
		std::string infileName = argv[1]; // name of input file

		std::string outfileName = infileName; // name of output file
		outfileName.insert(outfileName.find("."), "_modified");

		int verticalSeams = atoi(argv[2]); // number of vertical seams to remove
		int horizontalSeams = atoi(argv[3]); // number of horizontal seams to remove

		int num_colors; // record number of colors in image
		pixelMatrix pixel_matrix = read_pixel_matrix(infileName, num_colors); // get pixel color values
		// pixelMatrix energy_matrix = get_energy_matrix(pixel_matrix); // get pixel energy values

		remove_vertical_seams(pixel_matrix, verticalSeams); // remove seams
		remove_horizontal_seams(pixel_matrix, horizontalSeams);

		write_pixel_matrix(outfileName, pixel_matrix, num_colors); // write result to output file

		return 0; 
	}
}

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
pixelMatrix read_pixel_matrix(std::string infileName, int &num_colors) {
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

	// read color count
	ignore_comments(infileStream, s);
	num_colors = stoi(s);

	// create matrix
	pixelMatrix matrix;
	matrix.resize(x_size);

	// read pixels
	for (int j = 0; j < y_size; ++j) {
		for (int i = 0; i < x_size; ++i) {
			ignore_comments(infileStream, s);
			matrix[i].push_back(stoi(s));
		}
	}

	// close filestream and return matrix
	infileStream.close();
	return matrix;
}

// helper function to print pixel matrix
void print_pixel_matrix(pixelMatrix &pixel_matrix) {
	int x_size = pixel_matrix.size();
	int y_size = pixel_matrix[0].size();
	std::cout << std::endl;
	for (int j = 0; j < y_size; ++j) {
		for (int i = 0; i < x_size; ++i) {
			std::cout << pixel_matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

// calculate energy value corresponding to each pixel in matrix
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
	}

	return energy_matrix;
}

// find and remove vertical seams from the pixel matrix
void remove_vertical_seams(pixelMatrix &pixel_matrix, int seams) {
	// get dimensions
	int x_size = pixel_matrix.size();
	int y_size = pixel_matrix[0].size();

	// create path matrices
	pixelMatrix path_energies;
	pixelMatrix path_parents;

	// create energy matrix
	pixelMatrix energy_matrix;

	for (int s = 0; s < seams; ++s) {
		energy_matrix = get_energy_matrix(pixel_matrix);

		path_energies.resize(x_size);
		path_parents.resize(x_size);

		for (int i = 0; i < x_size; ++i) {
			path_energies[i].resize(y_size);
			path_parents[i].resize(y_size);
		}

		// fill path matrices
		for (int j = 0; j < y_size; ++j) {
			for (int i = 0; i < x_size; ++i) {
				if (j == 0) {
					// initialize first row
					path_energies[i][j] = energy_matrix[i][j];
					path_parents[i][j] = -1;
				}
				else {
					// get minimum path energy from adjacent pixels in previous row
					path_energies[i][j] = path_energies[i][j - 1];
					path_parents[i][j] = i;

					if (i < x_size - 1 && path_energies[i + 1][j - 1] < path_energies[i][j]) {
						path_energies[i][j] = path_energies[i + 1][j - 1];
						path_parents[i][j] = i + 1;
					}

					if (i > 0 && path_energies[i - 1][j - 1] <= path_energies[i][j]) {
						path_energies[i][j] = path_energies[i - 1][j - 1];
						path_parents[i][j] = i - 1;
					}

					path_energies[i][j] += energy_matrix[i][j];
				}
			}
		}

		// identify and remove seams
		int x_remove = 0;
		int y_remove = y_size - 1;
		for (int i = 1; i < x_size; ++i) {
			if (path_energies[i][y_remove] < path_energies[x_remove][y_remove]) {
				x_remove = i;
			}
		}

		// trace and remove pixels in seam
		while (y_remove >= 0) {
			// remove pixel
			for (int i = x_remove; i < x_size - 1; ++i) {
				pixel_matrix[i][y_remove] = pixel_matrix[i + 1][y_remove];
			}

			// get next pixel to remove
			x_remove = path_parents[x_remove][y_remove];
			--y_remove;

			// print_pixel_matrix(pixel_matrix);
		}

		pixel_matrix.pop_back();
		energy_matrix.pop_back();
		--x_size;
	}
}

// find and remove horizontal seams from the pixel matrix
void remove_horizontal_seams(pixelMatrix &pixel_matrix, int seams) {
	// get dimensions
	int x_size = pixel_matrix.size();
	int y_size = pixel_matrix[0].size();

	// create path matrices
	pixelMatrix path_energies;
	pixelMatrix path_parents;

	pixelMatrix energy_matrix;

	for (int s = 0; s < seams; ++s) {
		energy_matrix = get_energy_matrix(pixel_matrix);

		path_energies.resize(x_size);
		path_parents.resize(x_size);

		// fill path matrices
		for (int i = 0; i < x_size; ++i) {
			path_energies[i].resize(y_size, 0);
			path_parents[i].resize(y_size, 0);
			for (int j = 0; j < y_size; ++j) {
				if (i == 0) {
					// initialize first column
					path_energies[i][j] = energy_matrix[i][j];
					path_parents[i][j] = -1;
				}
				else {
					// get minimum path energy from adjacent pixels in previous column
					path_energies[i][j] = path_energies[i - 1][j];
					path_parents[i][j] = j;

					if (j < y_size - 1 && path_energies[i - 1][j + 1] < path_energies[i][j]) {
						path_energies[i][j] = path_energies[i - 1][j + 1];
						path_parents[i][j] = j + 1;
					}

					if (j > 0 && path_energies[i - 1][j - 1] <= path_energies[i][j]) {
						path_energies[i][j] = path_energies[i - 1][j - 1];
						path_parents[i][j] = j - 1;
					}

					path_energies[i][j] += energy_matrix[i][j];
				}
			}
		}

		// identify endpoint of seam
		int x_remove = x_size - 1;
		int y_remove = 0;
		for (int j = 1; j < y_size; ++j) {
			if (path_energies[x_remove][j] < path_energies[x_remove][y_remove]) {
				y_remove = j;
			}
		}

		// trace and remove pixels in seam
		while (x_remove >= 0) {
			// remove pixel
			// std::cout << energy_matrix[x_remove][y_remove] << " " << path_energies[x_remove][y_remove] << std::endl;
			pixel_matrix[x_remove].erase(pixel_matrix[x_remove].begin() + y_remove);

			// get next pixel to remove
			y_remove = path_parents[x_remove][y_remove];
			--x_remove;

			// print_pixel_matrix(pixel_matrix);
		}

		--y_size;
	}
}

// write pixel matrix to .pgm file
void write_pixel_matrix(std::string outfileName, pixelMatrix &pixel_matrix, int &num_colors) {
	// open filestream
	std::ofstream outfileStream(outfileName);

	// write header line
	outfileStream << "P2" << '\n';

	// write dimensions
	int x_size = pixel_matrix.size();
	int y_size = pixel_matrix[0].size();
	outfileStream << x_size << " " << y_size << '\n';

	// write color count
	outfileStream << num_colors << '\n';

	// write pixel values
	for (int j = 0; j < y_size; ++j) {
		for (int i = 0; i < x_size; ++i) {
			outfileStream << pixel_matrix[i][j] << ' ';
		}
		outfileStream << '\n';
	}

	outfileStream << '\n';

	// cloxe filestream
	outfileStream.close();
}
