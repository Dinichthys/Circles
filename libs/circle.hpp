#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "vector.hpp"

enum ObjectType {
    kObject,
    kLight
};

class Circle {
    private:
        Coordinates center;
        float radius;

        enum ObjectType type;

        Coordinates brightness;

    public:
        explicit Circle(Coordinates center_val = Coordinates(0, 0, 0, 0), float radius_val = 0,
                        enum ObjectType type_val = kObject, Coordinates brightness_val = Coordinates(0, 0, 0, 0))
            :center(center_val), brightness(brightness_val) {
            radius = radius_val;
            type = type_val;
        };

        Coordinates GetCenterCoordinates() const {return Coordinates(center);};
        float GetRadius() const {return radius;};
        enum ObjectType GetType() const {return type;};
        Coordinates GetBrightness() const {return Coordinates(brightness);};

        // void operator = (const Circle& circle) {
        //     center = circle.center;
        //     radius = circle.radius;
        // };
};

#endif // CIRCLE_HPP
