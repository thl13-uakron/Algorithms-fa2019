// You need to complete this program for your second project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <vector>
#include <map>
#include <fstream>

// helper functions and structures
#include "convexHullAlgs.hh"

// main program
int main(int argc, char *argv[])
{   
   if (argc < 3) 
      std::cout << "wrong format! should be \"a.exe algType dataFile\"";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];

      std::string outputFile = "";

      //read your data points from dataFile (see class example for the format)
      std::ifstream dataFilestream(dataFilename);

      // handle file errors
      try {
         if (!dataFilestream) throw(std::runtime_error("Invalid or empty input file."));

         std::vector<XY> points; // set of all points
         while (!dataFilestream.eof()) {
            points.push_back(*(new XY));
            dataFilestream >> points[points.size() - 1].x;
            dataFilestream >> points[points.size() - 1].y;
         }

         dataFilestream.close();

         std::cout << "Points: \n";
         for (XY p : points) std::cout << p.x << " " << p.y << "\n";

         std::vector<XY> hull; // set of verticies

         std::cout << "\nCalculating hull..." << std::endl;
         
         if (std::toupper(algType[0])=='G') {
            //call your Graham Scan algorithm to solve the problem
            hull = graham_scan(points);
            outputFile = "hull_G.txt";
         } 
         else if (std::toupper(algType[1])=='J') {
            //call your Jarvis March algorithm to solve the problem
            hull = jarvis_march(points);
            outputFile = "hull_J.txt";
         }
         else { //default 
            //call your Quickhull algorithm to solve the problem
            hull = quickhull(points);
            outputFile = "hull_Q.txt";
         }
         
         //write your convex hull to the outputFile (see class example for the format)
         //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.

         std::ofstream outputFilestream(outputFile);
         if (!outputFilestream) throw(std::runtime_error("Failed to open output file."));

         std::cout << "\nHull: \n";
         for (XY p : hull) {
            outputFilestream << p.x << " " << p.y << "\n";
            std::cout << p.x << " " << p.y << "\n";
         }

         std::cout << "\nVertices written to file " << outputFile << "\n";

         outputFilestream.close();
      }
      catch(std::runtime_error err) {
         std::cout << err.what() << std::endl;
      }

	}
	return 0;
}