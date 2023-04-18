#include "engine/Math/Shapes.h"

namespace gb_physics {

Vector2 circlePenetrationWithCircle(const Circle& a, const Circle& b) {
    // Calculate the vector between the two circle centers
    Vector2 centerToCenter = Vector2(b.x(), b.y()) - Vector2(a.x(), a.y());

    // Calculate the sum of the radii of both circles
    fix16_t radiiSum = fix16_add(a.radius(), b.radius());

    // Calculate the penetration depth
    fix16_t penetration = fix16_sub(radiiSum, centerToCenter.length());

    // Calculate the direction of the shortest distance to the edge of the overlapping region
    Vector2 direction = centerToCenter.normalize();

    // Multiply the direction by the penetration depth to get the shortest distance to the edge of the overlapping region
    return direction * penetration;
}

Vector2 circlePenetrationWithRectangle(const Circle& circle, const Rectangle& rectangle) {
    // Find the closest point on the AABB to the circle center
    Vector2 closestPoint(
        fix16_clamp(circle.x(), rectangle.x(), fix16_add(rectangle.x(), rectangle.width())),
        fix16_clamp(circle.y(), rectangle.y(), fix16_add(rectangle.y(), rectangle.height()))
    );

    // Calculate the vector from the circle center to the closest point
    Vector2 circleToClosest = Vector2(circle.x(), circle.y()) - closestPoint;

    // Calculate the penetration depth
    fix16_t penetration = fix16_sub(circle.radius(), circleToClosest.length());

    // Calculate the direction of the shortest distance to the edge of the overlapping region
    Vector2 direction = circleToClosest.normalize();

    // Multiply the direction by the penetration depth to get the shortest distance to the edge of the overlapping region
    return direction * penetration;
}

Vector2 rectanglePenetrationWithRectangle(const Rectangle& a, const Rectangle& b) {
    fix16_t penetrationDepthX = fix16_min(fix16_sub(fix16_add(a.x(), a.width()), b.x()),  fix16_sub(fix16_add(b.x(), b.width() ), a.x()));
    fix16_t penetrationDepthY = fix16_min(fix16_sub(fix16_add(a.y(), a.height()), b.y()), fix16_sub(fix16_add(b.y(), b.height()), a.y()));

    // Calculate the direction of the shortest distance to the edge of the overlapping region
    Vector2 direction;
    if (penetrationDepthX < penetrationDepthY)
        direction = Vector2(fix16_div(fix16_add(a.x(), a.width()), F16(2.0)) < fix16_div(fix16_add(b.x(), b.width()), F16(2.0)) ? F16(-1.0) : F16(1.0), F16(0.0));
    else
        direction = Vector2(F16(0.0), fix16_div(fix16_add(a.y(), a.height()), F16(2.0)) < fix16_div(fix16_add(b.y(), b.height()), F16(2.0)) ? F16(-1.0) : F16(1.0));

    // Multiply the direction by the penetration depth to get the shortest distance to the edge of the overlapping region
    return direction * fix16_min(penetrationDepthX, penetrationDepthY);
}

bool circleCollidesWithCircle(const Circle& a, const Circle& b) {
    auto dx = fix16_sub(a.x(), b.x());
    auto dy = fix16_sub(a.y(), b.y());
    auto distanceSquared = fix16_add(fix16_mul(dx, dx), fix16_mul(dy, dy));
    auto radiusSum = fix16_add(a.radius(), b.radius());
    return distanceSquared <= fix16_mul(radiusSum, radiusSum);
}

bool circleCollidesWithRectangle(const Circle& circle, const Rectangle& rectangle) {
    auto closestX = fix16_clamp(circle.x(), rectangle.x(), fix16_add(rectangle.x(), rectangle.width()));
    auto closestY = fix16_clamp(circle.y(), rectangle.y(), fix16_add(rectangle.y(), rectangle.height()));

    auto dx = fix16_sub(circle.x(), closestX);
    auto dy = fix16_sub(circle.y(), closestY);
    auto distanceSquared = fix16_add(fix16_mul(dx, dx), fix16_mul(dy, dy));

    return distanceSquared <= fix16_mul(circle.radius(), circle.radius());
}

bool rectangleCollidesWithRectangle(const Rectangle& a, const Rectangle& b) {
    return !(a.x() > fix16_add(b.x(), b.width() ) || fix16_add(a.x(), a.width() ) < b.x() ||
             a.y() > fix16_add(b.y(), b.height()) || fix16_add(a.y(), a.height()) < b.y());
}

bool Circle::collidesWith(Shape& shape){
    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (circle) {
        return circleCollidesWithCircle(*this, *circle);
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&shape);
    if (rectangle) {
        return circleCollidesWithRectangle(*this, *rectangle);
    }

    return false;
}

Vector2 Circle::getPenetration(Shape& shape){
    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (circle) {
        return circlePenetrationWithCircle(*this, *circle);
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&shape);
    if (rectangle) {
        return circlePenetrationWithRectangle(*this, *rectangle);
    }
    return Vector2(F16(0.0), F16(0.0));
}

Vector2 Rectangle::getPenetration(Shape& shape){
    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (circle) {
        auto ret = circlePenetrationWithRectangle(*circle, *this);
        return Vector2(fix16_sub(0, ret.x), fix16_sub(0, ret.y));
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&shape);
    if (rectangle) {
        return rectanglePenetrationWithRectangle(*this, *rectangle);
    }
    return Vector2(F16(0.0), F16(0.0));
}

bool Rectangle::collidesWith(Shape& shape){
    const Circle* circle = dynamic_cast<const Circle*>(&shape);
    if (circle) {
        return circleCollidesWithRectangle(*circle, *this);
    }

    const Rectangle* rectangle = dynamic_cast<const Rectangle*>(&shape);
    if (rectangle) {
        return rectangleCollidesWithRectangle(*this, *rectangle);
    }

    return false;    
}

} // namespace gb_physics