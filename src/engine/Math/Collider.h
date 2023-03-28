#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>
#include <cmath>
#include <algorithm>

#include <gba_types.h>

#include "engine/Math/Vector.h"
#include "engine/Math/ColliderI.h"
#include "engine/ECS/Interface.h"

class Rectangle;
class ConvexPolygon;

class Circle : public Collider {
public:
    __attribute__((section(".iwram"), long_call)) Circle(s32 x, s32 y, s32 radius, const Position &pos);

    bool __attribute__((section(".iwram"), long_call)) collidesWith(const Collider& other) const override;
    const __attribute__((section(".iwram"), long_call)) s32 x() const {return pos_.getX() + x_;}
    const __attribute__((section(".iwram"), long_call)) s32 y() const {return pos_.getY() + y_;}
    const __attribute__((section(".iwram"), long_call)) s32 radius() const {return radius_;}
private:
    s32 x_, y_;
    s32 radius_;
    const Position& pos_;

    bool __attribute__((section(".iwram"), long_call)) collidesWithCircle(const Circle& circle) const;
    bool __attribute__((section(".iwram"), long_call)) collidesWithRectangle(const Rectangle& rectangle) const;
    bool __attribute__((section(".iwram"), long_call)) collidesWithPolygon(const ConvexPolygon& polygon) const;
};

class Rectangle : public Collider {
public:
    __attribute__((section(".iwram"), long_call)) Rectangle(s32 x, s32 y, s32 width, s32 height, const Position &pos);

    bool __attribute__((section(".iwram"), long_call)) collidesWith(const Collider& other) const override;
    const __attribute__((section(".iwram"), long_call)) s32 x() const {return pos_.getX() + x_;}
    const __attribute__((section(".iwram"), long_call)) s32 y() const {return pos_.getY() + y_;}
    const __attribute__((section(".iwram"), long_call)) s32 width() const {return width_;}
    const __attribute__((section(".iwram"), long_call)) s32 height() const {return height_;}
    std::vector<Vector2> __attribute__((section(".iwram"), long_call)) getVertices() const;
private:
    s32 x_, y_;
    s32 width_, height_;
    const Position& pos_;

    bool __attribute__((section(".iwram"), long_call)) collidesWithCircle(const Circle& circle) const;
    bool __attribute__((section(".iwram"), long_call)) collidesWithRectangle(const Rectangle& rectangle) const;
    bool __attribute__((section(".iwram"), long_call)) collidesWithPolygon(const ConvexPolygon& polygon) const;

    friend class ConvexPolygon;
};

class ConvexPolygon : public Collider {
public:
    __attribute__((section(".iwram"), long_call)) ConvexPolygon(const std::vector<Vector2>& vertices, const Position &pos);

    bool __attribute__((section(".iwram"), long_call)) collidesWith(const Collider& other) const override;
private:
    std::vector<Vector2> vertices;
    const Position& pos_;

    bool __attribute__((section(".iwram"), long_call)) collidesWithCircle(const Circle& circle) const;
    bool __attribute__((section(".iwram"), long_call)) collidesWithRectangle(const Rectangle& rectangle) const;
    bool __attribute__((section(".iwram"), long_call)) collidesWithPolygon(const ConvexPolygon& polygon) const;

    // Helper function for the Separating Axis Theorem (SAT)
    bool __attribute__((section(".iwram"), long_call)) isSeparatingAxis(const Vector2& axis, const ConvexPolygon& polygonA, const ConvexPolygon& polygonB) const;
    void __attribute__((section(".iwram"), long_call)) projectOntoAxis(const Vector2& axis, s32& min, s32& max) const;

    std::vector<Vector2> __attribute__((section(".iwram"), long_call)) getEdges() const;
    bool __attribute__((section(".iwram"), long_call)) circleIntersectsLineSegment(const Circle& circle, const Vector2& p1, const Vector2& p2) const;
    bool __attribute__((section(".iwram"), long_call)) pointInPolygon(const Vector2& point) const;;
};

#endif // COLLIDER_H
