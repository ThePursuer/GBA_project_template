#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>
#include <cmath>
#include <algorithm>

#include <gba_types.h>

#include "engine/Math/Vector.h"
#include "engine/Math/ColliderI.h"

class Rectangle;
class ConvexPolygon;

class Position{
public:
    const u32 getX() const;
    const u32 getY() const;
};

class Circle : public Collider {
public:
    Circle(s32 x, s32 y, s32 radius, const Position &pos);

    bool collidesWith(const Collider& other) const override;
    const s32 x() const {return pos_.getX() + x_;}
    const s32 y() const {return pos_.getY() + y_;}
    const s32 radius() const {return radius_;}
private:
    s32 x_, y_;
    s32 radius_;
    const Position& pos_;

    bool collidesWithCircle(const Circle& circle) const;
    bool collidesWithRectangle(const Rectangle& rectangle) const;
    bool collidesWithPolygon(const ConvexPolygon& polygon) const;
};

class Rectangle : public Collider {
public:
    Rectangle(s32 x, s32 y, s32 width, s32 height, const Position &pos);

    bool collidesWith(const Collider& other) const override;
    const s32 x() const {return pos_.getX() + x_;}
    const s32 y() const {return pos_.getY() + y_;}
    const s32 width() const {return width_;}
    const s32 height() const {return height_;}
    std::vector<Vector2> getVertices() const;
    const Position &getPosition() const {return pos_;}
private:
    s32 x_, y_;
    s32 width_, height_;
    const Position& pos_;

    bool collidesWithCircle(const Circle& circle) const;
    bool collidesWithRectangle(const Rectangle& rectangle) const;
    bool collidesWithPolygon(const ConvexPolygon& polygon) const;
};

class ConvexPolygon : public Collider {
public:
    ConvexPolygon(const std::vector<Vector2>& vertices, const Position &pos);

    bool collidesWith(const Collider& other) const override;
private:
    std::vector<Vector2> vertices;
    const Position& pos_;

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
