#include <vector>
#include <map>

#include "convexHullHelpers.hh"

// algorithm implementation
// given the total set of points, return the set of vertices on the hull, in proper order

// Graham Scan Implementation
std::vector<XY> graham_scan(std::vector<XY> points) {

   std::vector<XY> hull;

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
   // compare the vector between the last two points in the hull with the vector 
   // between the currently selected point and the last point
   // if the new edge leans rightward compared to the previous edge, pop previous edge
   // the last point in the hull and try again
   // otherwise, push the new point to the hull
   while (pointIter != sortedPoints.end()) {
      XY p = pointIter->second;
      // push immediately if hull only contains one point
      if (hull.size() > 1) {
         // determine relative direction of vectors through dot product
         // positive dot product means left (push), negative means right (pop)
         XY v1 = get_distance_vector(hull[hull.size() - 1], hull[hull.size() - 2]);
         XY v2 = get_distance_vector(p, hull[hull.size() - 1]);
         if (get_dot_product(v1, v2) < 0) {
            // pop case
            hull.pop_back();
            continue;
         }
      }
      // push case
      hull.push_back(p);
      ++pointIter;
   }

   return hull;
}

// 
std::vector<XY> jarvis_march(std::vector<XY> points) {
   std::vector<XY> hull;

   // find the starting point as the point with lowest x-coordinate
   hull.push_back(points[0]);
   for (int i = 1; i < points.size(); ++i) {
      if (points[i].x < hull[0].x) hull[0] = points[i];
   }

   // for each remaining point on the hull:
   // compare the vector formed by the last two points (starting with a vertical line) 
   // with the vector formed by the last point and each of the other points 
   // select the point that forms the lowest angle to be the next point of the hull
   // the hull is complete when the starting point is selected

   return hull;
}

std::vector<XY> quickhull(std::vector<XY> points) {
   std::vector<XY> hull;

   // find two starting points as leftmost and rightmost points

   // split points into two halfs based on line formed by points

   // get hulls from each half

   // put hulls together in left=to-right order

   return hull;
}

