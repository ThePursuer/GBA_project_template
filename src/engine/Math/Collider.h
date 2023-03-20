#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>
#include <cmath>
#include <algorithm>

#include <gba_types.h>

#include "engine/Math/Vector.h"
#include "engine/Math/ColliderI.h"
#include "engine/Actor/Actor.h"

class Rectangle;
class ConvexPolygon;

class Circle : public Collider {
public:
    Circle(s32 x, s32 y, s32 radius, const Actor &actor);

    bool collidesWith(const Collider& other) const override;
    const s32 x() const {return actor_.getX() + x_;}
    const s32 y() const {return actor_.getY() + y_;}
    const s32 radius() const {return radius_;}
    const Actor& actor() const {return actor_;}
private:
    s32 x_, y_;
    s32 radius_;
    const Actor &actor_;

    bool collidesWithCircle(const Circle& circle) const;
    bool collidesWithRectangle(const Rectangle& rectangle) const;
    bool collidesWithPolygon(const ConvexPolygon& polygon) const;
};

class Rectangle : public Collider {
public:
    Rectangle(s32 x, s32 y, s32 width, s32 height, const Actor &actor);

    bool collidesWith(const Collider& other) const override;
    const s32 x() const {return actor_.getX() + x_;}
    const s32 y() const {return actor_.getY() + y_;}
    const s32 width() const {return width_;}
    const s32 height() const {return height_;}
    const Actor& actor() const {return actor_;}
    std::vector<Vector2> getVertices() const;
private:
    s32 x_, y_;
    s32 width_, height_;
    const Actor &actor_;

    bool collidesWithCircle(const Circle& circle) const;
    bool collidesWithRectangle(const Rectangle& rectangle) const;
    bool collidesWithPolygon(const ConvexPolygon& polygon) const;
};

class ConvexPolygon : public Collider {
public:
    ConvexPolygon(const std::vector<Vector2>& vertices, const Actor& actor);

    bool collidesWith(const Collider& other) const override;
    const Actor& actor() const {return actor_;}
private:
    std::vector<Vector2> vertices;
    const Actor& actor_;

    bool collidesWithCircle(const Circle& circle) const;
    bool collidesWithRectangle(const Rectangle& rectangle) const;
    bool collidesWithPolygon(const ConvexPolygon& polygon) const;

    // Helper function for the Separating Axis Theorem (SAT)
    bool isSeparatingAxis(const Vector2& axis, const ConvexPolygon& polygonA, const ConvexPolygon& polygonB) const;
    void projectOntoAxis(const Vector2& axis, s32& min, s32& max) const;

    std::vector<Vector2> getEdges() const;
    bool circleIntersectsLineSegment(const Circle& circle, const Vector2& p1, const Vector2& p2) const;
    bool pointInPolygon(const Vector2& point) const;;
};

#endif // COLLIDER_H
