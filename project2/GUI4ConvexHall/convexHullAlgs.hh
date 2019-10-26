#include <vector>
#include <map>
#include <cmath>

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
         // determine relative direction of vectors through cross product
         // positive cross product means left (push), negative means right (pop)
         XY v1 = get_distance_vector(hull[hull.size() - 2], hull[hull.size() - 1]);
         XY v2 = get_distance_vector(hull[hull.size() - 1], p);
         if (get_cross_product(v1, v2) < 0) {
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
   // the point whose vector bends the least compared to the previous vector is selected
   // as the next point on the hull, and removed from the set 
   // the hull is complete when the starting point, which isn't removed, is selected again

   XY v1(0, 1), v2; // vectors

   // point selected to be added to hull, represented as iterator so it can be erased easily
   auto nextVertex = points.begin(); 

   // angles formed by vectors are measured through cosine
   double cosine, maxCosine;

   while (true) {
      // pick a starting candidate
      nextVertex = points.begin();
      v2 = get_distance_vector(hull[hull.size() - 1], *nextVertex);
      maxCosine = get_cosine(v1, v2);

      // find the vertex by comparing the currently selected candidate to the remaining points
      for (auto p = points.begin(); p < points.end(); ++p) {
         v2 = get_distance_vector(hull[hull.size() - 1], *p);
         // the cosine increasess as the relative angle shrinks, reaching 1 for parallel vectors
         cosine = get_cosine(v1, v2);
         if (cosine > maxCosine) {
            maxCosine = cosine;
            nextVertex = p;
         }
      }

      if (*nextVertex != hull[0]) {
         hull.push_back(*nextVertex);
         points.erase(nextVertex);
         v1 = get_distance_vector(hull[hull.size() - 2], hull[hull.size() - 1]);
      }
      else {
         break;
      }
   }

   return hull;
}

std::vector<XY> quickhull(std::vector<XY> points) {
   std::vector<XY> hull;

   // find the two starting points as the leftmost and rightmost points
   XY leftPoint = points[0], rightPoint = points[0];
   for (int i = 1; i < points.size(); ++i) {
      if (points[i].x < leftPoint.x) leftPoint = points[i];
      else if (points[i].x > rightPoint.x) rightPoint = points[i];
   }

   // split the points into two halfs based on the partition line formed by starting points
   std::vector<XY> topPoints, bottomPoints;
   double partitionSlope = get_slope(leftPoint, rightPoint);
   for (XY p : points) {
      if (p != leftPoint && p != rightPoint) {
         // the side of the parition that the point is one is determined by 
         // the slope of the vector leading to it from the left endpoint
         // compared to the slope of the vector from the left endpoint to the right endpoint
         double slope = get_slope(leftPoint, p);
         if (slope > partitionSlope) topPoints.push_back(p);
         else if (slope < partitionSlope) bottomPoints.push_back(p);
      }
   }

   // get hulls from each half
   std::vector<XY> topHull = partial_quickhull(leftPoint, rightPoint, topPoints);
   std::vector<XY> bottomHull = partial_quickhull(leftPoint, rightPoint, bottomPoints);

   // put the hulls together in left=to-right order
   hull.push_back(leftPoint);
   for (int i = 0; i < topHull.size(); ++i) hull.push_back(topHull[i]);
   hull.push_back(rightPoint);
   for (int i = bottomHull.size() - 1; i >= 0; --i) hull.push_back(bottomHull[i]);

   return hull;
}

