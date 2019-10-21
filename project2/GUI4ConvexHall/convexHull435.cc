// You need to complete this program for your second project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <vector>
#include <map>
#include <fstream>

// helper functions and structures

// values like points and vectors that are represented in (x, y) form
struct XY {
   int x;
   int y;
   XY(int x, int y) : x(x), y(y) {}
   XY() {}
   bool operator!=(XY p2) {return (this->x != p2.x) || (this->y != p2.y);}
   bool operator==(XY p2) {return (this->x == p2.x) && (this->y == p2.y);}
};

// distance between two points in (x, y) form
XY get_distance_vector(XY &p1, XY &p2) {
   return XY(p2.x - p1.x, p2.y - p1.x);
}

// tangent formed by edge between between two points relative to vertical line extending up from p1
// result ifs proportional to value of angle between edge and horizontal line extending right from p1
// used for graham scan
double get_tangent(XY &p1, XY &p2) {
   double result = static_cast<double>(p1.x - p2.x) / (p2.y - p1.y);
   return result;
}

// cross product of two (x, y) vectors, used for Graham Scan
int get_cross_product(XY &v1, XY &v2) {
   int result;
   return result;
}

// dot product of two (x, y) vectors, used for Jarvis March
int get_dot_product(XY &v1, XY &v2) {
   int result;
   return result;
}

// given a set of points on one side of a line formed by two other points
// given that those two points are known to be part of the hull
// recusively eliminate points that can't be on the hull until there is no more uncertainty
// used for quickhull
std::vector<XY> partial_quickhull(XY &rightPoint, XY &leftPoint, std::vector<XY> &points) {
   std::vector<XY> result;

   // base cases given 1 or 0 points in set
   if (points.size() == 1) result.push_back(points[0]);
   if (points.size() <= 1) return result;

   // find point furthest away from line
   XY farPoint;

   // selected points are guaranteed to be on hull
   // points inside the triangle created by the selected points cannot be on the hull

   // points outside of triangle may or may not be on hull
   // split into right half and left half
   // repeat process on each half if not empty

   // return known verticies listed in left-to-right order
   return result;
}

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
         
         if (std::toupper(algType[0])=='G') {
            //call your Graham Scan algorithm to solve the problem

            // find the starting point as the point with the lowest y-coordinate
            hull.push_back(points[0]);
            for (int i = 1; i < points.size(); ++i) {
               if (points[i].y < hull[0].y) hull[0] = points[i];
            }

            // get the tangent of each remaining point relative to the starting point
            // sort the points by tangent 
            std::map<double, XY> sortedPoints;
            for (XY p : points) {
               if (p != hull[0]) {
                  double tangent = get_tangent(hull[0], p);
                  // if two points have the same tangent, the one with the lower y-coordinate can be ignored
                  // since it would either be inside the hull or intersected by an edge
                  if (!sortedPoints.count(tangent) || p.y > sortedPoints[tangent].y) 
                     sortedPoints[tangent] = p;
               }
            }

            // add the lowest-tangent point to the hull
            auto pointIter = sortedPoints.begin();
            hull.push_back(pointIter->second);
            ++pointIter;

            // for each remaining point:
            // compare the vector between the last two points in the hull
            // with the vector between the currently selected point and the last point
            // if the new edge leans rightward compared to the previous edge, delete
            // the last point in the hull and try again
            // otherwise, add the new point to the hull
            while (pointIter != sortedPoints.end()) {
               XY p = pointIter->second;
               ++pointIter;
            }

            outputFile = "hull_G.txt";
         } 
         else if (std::toupper(algType[1])=='J') {
            //call your Jarvis March algorithm to solve the problem

            // find the starting point as the point with lowest x-coordinate

            // for each remaining point on the hull:
            // compare the vector formed by the last two points (starting with a vertical line) 
            // with the vector formed by the last point and each of the other points 
            // select the point that forms the lowest angle as the next point of the hull
            // the hull is complete when the starting point is selected

            outputFile = "hull_J.txt";
         }
         else { //default 
            //call your Quickhull algorithm to solve the problem
            outputFile = "hull_Q.txt";
         }
         
         //write your convex hull to the outputFile (see class example for the format)
         //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.

         std::ofstream outputFilestream(outputFile);
         if (!outputFilestream) throw(std::runtime_error("Failed to open output file."));

         std::cout << "\nHull: \n";
         for (XY p : hull) std::cout << p.x << " " << p.y << "\n";

         outputFilestream.close();
      }
      catch(std::runtime_error err) {
         std::cout << err.what() << std::endl;
      }

	}
	return 0;
}