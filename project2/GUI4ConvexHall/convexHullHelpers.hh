#include <cmath>

// structures and methods used to implement algorithms

// represent values such as points and vectors that are represented in (x, y) form
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
   return XY(p2.x - p1.x, p2.y - p1.y);
}

// tangent formed by edge between between two points relative to vertical line extending up from p1
// result corresponds to value of angle between edge and horizontal line extending right from p1
// used for graham scan
double get_tangent(XY &p1, XY &p2) {
   double result = static_cast<double>(p1.x - p2.x) / (p2.y - p1.y);
   return result;
}

// cross product of two (x, y) vectors, used for Graham Scan
int get_cross_product(XY &v1, XY &v2) {
   int result = (v1.x * v2.y) - (v1.y * v2.x);
   return result;
}

// dot product of two (x, y) vectors, used for Jarvis March
int get_dot_product(XY &v1, XY &v2) {
   int result = (v1.x * v2.x) + (v1.y * v2.y);
   return result;
}

// magnitude of vector, used for Jarvis March
double get_magnitude(XY &v) {
   return sqrt(static_cast<double>(v.x * v.x) + (v.y * v.y));
}

// cosine of angle between two vectors, result corresponds inversely to value of angle
// used for Jarvis March
double get_cosine(XY &v1, XY &v2) {
   return get_dot_product(v1, v2) / (get_magnitude(v1) * get_magnitude(v2));
}

// distance from a point to a line formed by two other points, used for Quickhull
double get_distance_to_line(XY &p, XY &leftPoint, XY &rightPoint) {
   // given the line between leftPoint and rightPoint represented as ax + by + c - 0
   // the distance to p can be calculated as 
   double a, b, c;
   double result;
   return result;
}

// given a set of points on one side of a line formed by two other points
// given that those two points are known to be part of the hull
// recusively eliminate points that can't be on the hull until there is no more uncertainty
// used for Quickhull
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