#pragma once

#include <vector>
#include <memory>

#include <gba_types.h>

#include "engine/Math/Vector.h"
#include "engine/Math/Shape.h"
#include "engine/ECS/Interface.h"


namespace gb_physics {

class Circle : public Shape {
public:
    Circle(fix16_t x, fix16_t y, fix16_t radius, std::shared_ptr<Vector2>pos): x_(x), y_(y), radius_(radius), pos_(pos) {}

    const fix16_t x() const override { return fix16_add(pos_->x, x_); }
    const fix16_t y() const override { return fix16_add(pos_->y, y_); }
    const fix16_t radius() const { return radius_; }

    bool __attribute__((section(".iwram"), long_call)) collidesWith(Shape& shape) override;
    Vector2 __attribute__((section(".iwram"), long_call)) getPenetration(Shape& shape) override;
private:
    std::shared_ptr<Vector2> pos_;
    fix16_t x_, y_; // Relative Coordinates
    fix16_t radius_;
};

// class ConvexPolygon : public Shape {
// public:
//     ConvexPolygon(const std::vector<Vector2>& vertices, std::shared_ptr<Vector2>pos): vertices_(vertices), pos_(pos) {}
//     std::vector<Vector2> vertices_;

//     const fix16_t x() const override { return fix16_add(pos_->x, x_); }
//     const fix16_t y() const override { return fix16_add(pos_->y, y_); }
//     bool collidesWith(Shape& shape);
// private:
//     fix16_t x_, y_; // Relative Coordinates
//     std::shared_ptr<Vector2> pos_;
// };

class Rectangle : public Shape {
public:
    Rectangle(fix16_t x, fix16_t y, fix16_t width, fix16_t height, std::shared_ptr<Vector2>pos): x_(x),  y_(y), height_(height), width_(width), pos_(pos) {}
    // ConvexPolygon getSelfAsPolygon() const;

    const fix16_t x() const override { return fix16_add(pos_->x, x_); }
    const fix16_t y() const override { return fix16_add(pos_->y, y_); }
    const fix16_t height() const { return height_; } 
    const fix16_t width () const { return width_; } 

    bool __attribute__((section(".iwram"), long_call)) collidesWith(Shape& shape) override;
    Vector2 __attribute__((section(".iwram"), long_call)) getPenetration(Shape& shape) override;
private:
    std::shared_ptr<Vector2> pos_;
    fix16_t x_, y_; // Relative Coordinates
    fix16_t width_, height_;
};

Vector2 circlePenetrationWithCircle(const Circle& a, const Circle& b);
Vector2 circlePenetrationWithRectangle(const Circle& circle, const Rectangle& rectangle);
Vector2 rectanglePenetrationWithRectangle(const Rectangle& a, const Rectangle& b);

bool circleCollidesWithCircle(const Circle& a, const Circle& b);
bool circleCollidesWithRectangle(const Circle& circle, const Rectangle& rectangle);
bool rectangleCollidesWithRectangle(const Rectangle& a, const Rectangle& b);

} // namespace gb_physics