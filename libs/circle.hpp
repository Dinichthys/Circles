#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "vector.hpp"

class Circle {
    private:
        Coordinates center;
        float radius;

    public:
        explicit Circle(Coordinates center_val = Coordinates(0, 0, 0, 0), float radius_val = 0)
            :center(center_val) {
            radius = radius_val;
        };

        Coordinates GetCenterCoordinates() const {return Coordinates(center);};
        float GetRadius() const {return radius;};

        void operator = (const Circle& circle) {
            center = circle.center;
            radius = circle.radius;
        };
};

#endif // CIRCLE_HPP
