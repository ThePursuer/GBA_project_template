
// // Fixed-point precision fpos (e.g., 1000 means 3 decimal places)
// static const s32 PRECISION = 1000;


// std::vector<Vector2> Rectangle::getVertices() const {
//     return {
//         Vector2(x_, y_),
//         Vector2(x_ + width_, y_),
//         Vector2(x_ + width_, y_ + height_),
//         Vector2(x_, y_ + height_) 
//     };
// }

// // Convex polygon
// bool ConvexPolygon::collidesWithCircle(const Circle& circle) const {
//     // Check if the circle's center is inside the polygon
//     if (pointInPolygon(Vector2(circle.x(), circle.y()))) {
//         return true;
//     }

//     // Check if the circle is colliding with any of the polygon's edges
//     for (size_t i = 0; i < vertices.size(); ++i) {
//         Vector2 p1 = vertices[i] + Vector2(pos_.getX(), pos_.getY());
//         Vector2 p2 = vertices[(i + 1) % vertices.size()] + Vector2(pos_.getX(), pos_.getY());

//         if (circleIntersectsLineSegment(circle, p1, p2)) {
//             return true;
//         }
//     }

//     return false;
// }

// bool ConvexPolygon::collidesWithRectangle(const Rectangle& rectangle) const {
//     // Convert the rectangle to a polygon
//     ConvexPolygon rectPolygon(rectangle.getVertices(), rectangle.pos_);

//     // Check for collisions between the two polygons using the collidesWithPolygon function
//     return collidesWithPolygon(rectPolygon);
// }

// bool ConvexPolygon::collidesWithPolygon(const ConvexPolygon& polygon) const {
//     // Check for a separating axis between the two polygons
//     for (const Vector2& edge : getEdges()) {
//         Vector2 axis = edge.perpendicular();
//         if (isSeparatingAxis(axis, *this, polygon)) {
//             return false;
//         }
//     }

//     for (const Vector2& edge : polygon.getEdges()) {
//         Vector2 axis = edge.perpendicular();
//         if (isSeparatingAxis(axis, *this, polygon)) {
//             return false;
//         }
//     }

//     return true;
// }

// bool ConvexPolygon::isSeparatingAxis(const Vector2& axis, const ConvexPolygon& polygonA, const ConvexPolygon& polygonB) const {
//     s32 minA, maxA, minB, maxB;
//     polygonA.projectOntoAxis(axis, minA, maxA);
//     polygonB.projectOntoAxis(axis, minB, maxB);

//     // Use an epsilon to account for rounding errors
//     s32 epsilon = 0;  // You can experiment with different values to find the best one for your use case

//     return (minB > maxA + epsilon) || (minA > maxB + epsilon);
// }

// void ConvexPolygon::projectOntoAxis(const Vector2& axis, s32& min, s32& max) const {
//     min = std::numeric_limits<s32>::max();
//     max = -std::numeric_limits<s32>::max();

//     for (const Vector2& vertex : vertices) {
//         Vector2 globalVertex = vertex + Vector2(pos_.getX(), pos_.getY());
//         s32 projection = globalVertex.dot(axis);
//         min = std::min(min, projection);
//         max = std::max(max, projection);
//     }
// }

// std::vector<Vector2> ConvexPolygon::getEdges() const {
//     std::vector<Vector2> edges;
//     for (size_t i = 0; i < vertices.size(); ++i) {
//         Vector2 p1 = vertices[i] + Vector2(pos_.getX(), pos_.getY());
//         Vector2 p2 = vertices[(i + 1) % vertices.size()] + Vector2(pos_.getX(), pos_.getY());
//         edges.push_back(p2 - p1);
//     }
//     return edges;
// }

// bool ConvexPolygon::circleIntersectsLineSegment(const Circle& circle, const Vector2& p1, const Vector2& p2) const {
//     Vector2 line = p2 - p1;
//     Vector2 circleToP1 = Vector2(circle.x(), circle.y()) - p1;
//     s32 t = (circleToP1.dot(line) * PRECISION) / line.lengthSquared();
//     t = std::clamp(t, (s32)0, PRECISION);

//     Vector2 closestPoint = p1 + (line * t / PRECISION);
//     Vector2 circleToClosest = closestPoint - Vector2(circle.x(), circle.y());

//     return circleToClosest.lengthSquared() <= circle.radius() * circle.radius();
// }

// bool ConvexPolygon::pointInPolygon(const Vector2& point) const {
//     bool inside = false;
//     size_t vertexCount = vertices.size();
//     for (size_t i = 0, j = vertexCount - 1; i < vertexCount; j = i++) {
//         Vector2 vertexI = vertices[i] + Vector2(pos_.getX(), pos_.getY());
//         Vector2 vertexJ = vertices[j] + Vector2(pos_.getX(), pos_.getY());

//         bool condition1 = (vertexI.y > point.y) != (vertexJ.y > point.y);
//         s32 slope = (vertexJ.x - vertexI.x) * PRECISION / (vertexJ.y - vertexI.y);
//         s32 projectedX = vertexI.x + slope * (point.y - vertexI.y) / PRECISION;
//         bool condition2 = point.x < projectedX;

//         if (condition1 && condition2) {
//             inside = !inside;
//         }
//     }
//     return inside;
// }