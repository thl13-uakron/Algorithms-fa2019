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
   // double result = 
   return static_cast<double>(p1.x - p2.x) / (p2.y - p1.y);
   // return result;
}

// cross product of two (x, y) vectors, used for Graham Scan
int get_cross_product(XY &v1, XY &v2) {
   // int result = 
   return (v1.x * v2.y) - (v1.y * v2.x);
   // return result;
}

// dot product of two (x, y) vectors, used for Jarvis March
int get_dot_product(XY &v1, XY &v2) {
   // int result = 
   return (v1.x * v2.x) + (v1.y * v2.y);
   // return result;
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
   // calculated as |(y2 - y1)x0 - (x2 - x1)y0 + x1y2 + x2y1| / sqrt((y2 - y1)^2 + (x2 - x1)^2)
   // given p = (x0, y0), leftPoint = (x1, x1), rightPoint = (x2, y2)
   double result = ((rightPoint.y - leftPoint.y) * p.x) - ((rightPoint.x - leftPoint.x) * p.y);
   result += (leftPoint.x * rightPoint.y) + (rightPoint.x * leftPoint.y);
   result = abs(result) / sqrt(pow(rightPoint.y - leftPoint.y, 2) + pow(rightPoint.x - leftPoint.x, 2));
   return result;
}

// slope of the distance vector leading from p1 to p2, used for Quickhull
double get_slope(XY &p1, XY &p2) {
   return static_cast<double>(p2.y - p1.y) / (p2.x - p1.x);
}

// given a set of points on one side of a line formed by two other points
// given that those two points are known to be part of the hull
// recusively eliminate points that can't be on the hull until there is no more uncertainty
// used for Quickhull
std::vector<XY> partial_quickhull(XY &leftPoint, XY &rightPoint, std::vector<XY> &points) {
   std::vector<XY> result;

   // base cases given 1 or 0 points in set
   if (points.size() == 1) result.push_back(points[0]);
   if (points.size() <= 1) return result;

   // recursive case:

   // find point furthest away from line
   XY farPoint = points[0];
   double maxDistance = get_distance_to_line(farPoint, leftPoint, rightPoint);
   double distance;
   for (int i = 1; i < points.size(); ++i) {
      distance = get_distance_to_line(points[i], leftPoint, rightPoint);
      if (distance > maxDistance) {
         farPoint = points[i];
         maxDistance = distance;
      }
   }

   // the left point, right point, and farthest point are guaranteed to be on hull
   // points inside the triangle created by the selected points cannot be on the hull
   // points outside of triangle may or may not be on hull

   // split the outside points based on whether they're to the right or to the left of the triangle
   // as determined by the angle it forms around the enpoints of the baseline
   // compared to the angle that the farthest point forms around the same endpoints
   // compute the needed edge vectors beforehand since they'll need to be reused
   XY leftTriangleBase = get_distance_vector(leftPoint, rightPoint);
   XY triangleEdge = get_distance_vector(leftPoint, farPoint);
   double leftTriangleCosine = get_cosine(leftTriangleBase, triangleEdge);

   XY rightTriangleBase = get_distance_vector(rightPoint, leftPoint);
   triangleEdge = get_distance_vector(rightPoint, farPoint);
   double rightTriangleCosine = get_cosine(rightTriangleBase, triangleEdge);

   double currentCosine;
   XY currentEdge;   

   std::vector<XY> leftHalf, rightHalf;
   for (XY p : points) {
      currentEdge = get_distance_vector(leftPoint, p);
      currentCosine = get_cosine(leftTriangleBase, currentEdge);
      if (currentCosine < leftTriangleCosine) {
         leftHalf.push_back(p);
         continue;
      }
      currentEdge = get_distance_vector(rightPoint, p);
      currentCosine = get_cosine(rightTriangleBase, currentEdge);
      if (currentCosine < rightTriangleCosine) {
         rightHalf.push_back(p);
      }
   }

   // repeat the process of each subset
   std::vector<XY> leftHull = partial_quickhull(leftPoint, farPoint, leftHalf);
   std::vector<XY> rightHull = partial_quickhull(farPoint, rightPoint, rightHalf);

   // return known verticies listed in left-to-right order
   for (XY p : leftHull) result.push_back(p);
   result.push_back(farPoint);
   for (XY p : rightHull) result.push_back(p);
   return result;
}