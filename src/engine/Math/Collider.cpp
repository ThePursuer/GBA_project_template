#include "engine/Math/Collider.h"

// Fixed-point precision factor (e.g., 1000 means 3 decimal places)
static const s32 PRECISION = 1000;

// Circle

Circle::Circle(s32 x, s32 y, s32 radius, const Actor& actor) : x_(x), y_(y), radius_(radius), actor_(actor) {}

bool Circle::collidesWith(const Collider& other) const {
    const Circle* circle = dynamic_cast<const Circle*>(&other);
    if (circle) {
        return collidesWithCircle(*circle);
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other);
    if (rectangle) {
        return collidesWithRectangle(*rectangle);
    }

    const ConvexPolygon* polygon = dynamic_cast<const ConvexPolygon*>(&other);
    if (polygon) {
        return collidesWithPolygon(*polygon);
    }

    return false;
}

bool Circle::collidesWithCircle(const Circle& circle) const {
    s32 dx = x() - circle.x();
    s32 dy = y() - circle.y();
    s32 distanceSquared = dx * dx + dy * dy;
    s32 radiusSum = radius_ + circle.radius();
    return distanceSquared <= radiusSum * radiusSum;
}

bool Circle::collidesWithRectangle(const Rectangle& rectangle) const {
    s32 closestX = std::clamp(x(), rectangle.x(), rectangle.x() + rectangle.width());
    s32 closestY = std::clamp(y(), rectangle.y(), rectangle.y() + rectangle.height());

    s32 dx = x() - closestX;
    s32 dy = y() - closestY;
    s32 distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= radius_ * radius_;
}

bool Circle::collidesWithPolygon(const ConvexPolygon& polygon) const {
    return polygon.collidesWith(*this);
}

// Rectangle

Rectangle::Rectangle(s32 x, s32 y, s32 width, s32 height, const Actor &actor) : x_(x), y_(y), width_(width), height_(height), actor_(actor) {}

bool Rectangle::collidesWith(const Collider& other) const {
    const Circle* circle = dynamic_cast<const Circle*>(&other);
    if (circle) {
        return collidesWithCircle(*circle);
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other);
    if (rectangle) {
        return collidesWithRectangle(*rectangle);
    }

    const ConvexPolygon* polygon = dynamic_cast<const ConvexPolygon*>(&other);
    if (polygon) {
        return collidesWithPolygon(*polygon);
    }

    return false;
}

std::vector<Vector2> Rectangle::getVertices() const {
    return {
        Vector2(x_, y_),
        Vector2(x_ + width_, y_),
        Vector2(x_ + width_, y_ + height_),
        Vector2(x_, y_ + height_) 
    };
}

bool Rectangle::collidesWithCircle(const Circle& circle) const {
    return circle.collidesWith(*this);
}

bool Rectangle::collidesWithRectangle(const Rectangle& rectangle) const {
    return !(x() > rectangle.x() + rectangle.width() || x() + width() < rectangle.x() ||
             y() > rectangle.y() + rectangle.height() || y() + height() < rectangle.y());
}

bool Rectangle::collidesWithPolygon(const ConvexPolygon& polygon) const {
    return polygon.collidesWith(*this);
}

// Convex polygon

ConvexPolygon::ConvexPolygon(const std::vector<Vector2>& vertices, const Actor& actor) : vertices(vertices), actor_(actor) {}

bool ConvexPolygon::collidesWith(const Collider& other) const {
    const Circle* circle = dynamic_cast<const Circle*>(&other);
    if (circle) {
        return collidesWithCircle(*circle);
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&other);
    if (rectangle) {
        return collidesWithRectangle(*rectangle);
    }

    const ConvexPolygon* polygon = dynamic_cast<const ConvexPolygon*>(&other);
    if (polygon) {
        return collidesWithPolygon(*polygon);
    }

    return false;
}

bool ConvexPolygon::collidesWithCircle(const Circle& circle) const {
    // Check if the circle's center is inside the polygon
    if (pointInPolygon(Vector2(circle.x(), circle.y()))) {
        return true;
    }

    // Check if the circle is colliding with any of the polygon's edges
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 p1 = vertices[i] + actor_.getPosition();
        Vector2 p2 = vertices[(i + 1) % vertices.size()] + actor_.getPosition();

        if (circleIntersectsLineSegment(circle, p1, p2)) {
            return true;
        }
    }

    return false;
}

bool ConvexPolygon::collidesWithRectangle(const Rectangle& rectangle) const {
    // Convert the rectangle to a polygon
    ConvexPolygon rectPolygon(rectangle.getVertices(), rectangle.actor());

    // Check for collisions between the two polygons using the collidesWithPolygon function
    return collidesWithPolygon(rectPolygon);
}

bool ConvexPolygon::collidesWithPolygon(const ConvexPolygon& polygon) const {
    // Check for a separating axis between the two polygons
    for (const Vector2& edge : getEdges()) {
        Vector2 axis = edge.perpendicular();
        if (isSeparatingAxis(axis, *this, polygon)) {
            return false;
        }
    }

    for (const Vector2& edge : polygon.getEdges()) {
        Vector2 axis = edge.perpendicular();
        if (isSeparatingAxis(axis, *this, polygon)) {
            return false;
        }
    }

    return true;
}

bool ConvexPolygon::isSeparatingAxis(const Vector2& axis, const ConvexPolygon& polygonA, const ConvexPolygon& polygonB) const {
    s32 minA, maxA, minB, maxB;
    polygonA.projectOntoAxis(axis, minA, maxA);
    polygonB.projectOntoAxis(axis, minB, maxB);

    // Use an epsilon to account for rounding errors
    s32 epsilon = 0;  // You can experiment with different values to find the best one for your use case

    return (minB > maxA + epsilon) || (minA > maxB + epsilon);
}

void ConvexPolygon::projectOntoAxis(const Vector2& axis, s32& min, s32& max) const {
    min = std::numeric_limits<s32>::max();
    max = -std::numeric_limits<s32>::max();

    for (const Vector2& vertex : vertices) {
        Vector2 globalVertex = vertex + actor_.getPosition();
        s32 projection = globalVertex.dot(axis);
        min = std::min(min, projection);
        max = std::max(max, projection);
    }
}

std::vector<Vector2> ConvexPolygon::getEdges() const {
    std::vector<Vector2> edges;
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector2 p1 = vertices[i] + actor_.getPosition();
        Vector2 p2 = vertices[(i + 1) % vertices.size()] + actor_.getPosition();
        edges.push_back(p2 - p1);
    }
    return edges;
}

bool ConvexPolygon::circleIntersectsLineSegment(const Circle& circle, const Vector2& p1, const Vector2& p2) const {
    Vector2 line = p2 - p1;
    Vector2 circleToP1 = Vector2(circle.x(), circle.y()) - p1;
    s32 t = (circleToP1.dot(line) * PRECISION) / line.lengthSquared();
    t = std::clamp(t, (s32)0, PRECISION);

    Vector2 closestPoint = p1 + (line * t / PRECISION);
    Vector2 circleToClosest = closestPoint - Vector2(circle.x(), circle.y());

    return circleToClosest.lengthSquared() <= circle.radius() * circle.radius();
}

bool ConvexPolygon::pointInPolygon(const Vector2& point) const {
    bool inside = false;
    size_t vertexCount = vertices.size();
    for (size_t i = 0, j = vertexCount - 1; i < vertexCount; j = i++) {
        Vector2 vertexI = vertices[i] + actor_.getPosition();
        Vector2 vertexJ = vertices[j] + actor_.getPosition();

        bool condition1 = (vertexI.y > point.y) != (vertexJ.y > point.y);
        s32 slope = (vertexJ.x - vertexI.x) * PRECISION / (vertexJ.y - vertexI.y);
        s32 projectedX = vertexI.x + slope * (point.y - vertexI.y) / PRECISION;
        bool condition2 = point.x < projectedX;

        if (condition1 && condition2) {
            inside = !inside;
        }
    }
    return inside;
}