README

	This is a program that can rescale images with minimal distortion through 
	seam carving, a method in which pixels with little distinction from their 
	surroundings are selected to be removed.

	Seams are sequences of adjacent pixels that are either vertical, with one 
	for every row, or horizontal, with one for every column, that get selected 
	to be removed.

	Removing a vertical seam sizes the image down by one column of pixels, and
	removing a horizontal seam sizes it down by one row.


	To execute this program:

		1. If the executable "a.exe" is present, execute it with three command 
		line arguments, each separated by a space:

			a. The name of the image file to run the algorithm on (.pgm format)

			b. The number of vertical seams to remove

			c. The number of horizontal seams to remove

		2. If a.exe is not present, compile the source file "seamcarve.cpp" 
		with g++


	Upon execution, this program will:

		1. Read the input file in text form to determine the dimensions as well 
		as the color value of each pixel, stored in a two-dimensional array by
		row and column

		2. Assign each pixel an energy level based on the difference in value 
		between it and it's adjacent pixels (not including diagonals)

		3. For every vertical seam:

			a. For each pixel in a row, record the value of the lowest-energy 
			path leading to it, calculated by combining the pixel's individual
			energy value with lowest path energy of an adjacent pixel in the 
			previous row, as well as the index of that pixel.

			b. Start with every pixel in the first row having a path energy 
			equal to their individual energy and iterate over every subsequent 
			row

			c. Find the pixel with the lowest path energy in the last row, and 
			retrace the path leading to it to identify the seam

			d. Remove every pixel in the seam

		4. For every horizontal seam:

			a. Do the same as in step 3 but by iterating through every column 
			rather than through every row

		5. Output the modified image into a new file, with:

			a. One less row for every horizontal seam removed

			b. One less column for every vertical seam removed

			c. Every removed pixel excluded


CONTACT

	This implementation was written by Thomas Li (thl13@zips.uakron.edu)
