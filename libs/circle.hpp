#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "vector.hpp"

class Circle {
    private:
        Coordinates center;
        float radius;

    public:
        Circle(Coordinates center_val, float radius_val)
            :center(center_val) {
            radius = radius_val;
        };

        Coordinates GetCenterCoordinates() const {return center;};
        float GetRadius() const {return radius;};
};

#endif // CIRCLE_HPP
