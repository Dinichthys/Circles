#include <stdlib.h>
#include <stdio.h>

#include "draw.hpp"

#include "logging.h"

int main() {
    set_log_lvl(kError);

    float lt_corner_coordinates_1[2] = {100, 100};
    float rb_corner_coordinates_1[2] = {620, 620};
    float start_point_coordinates_1[2] = {200, 300};
    float lt_corner_coordinates_2[2] = {720, 320};
    float rb_corner_coordinates_2[2] = {1020, 620};
    float start_point_coordinates_2[2] = {900, 500};

    Graph graphs[2] = {
        Graph(Coordinates(2, lt_corner_coordinates_1),
              Coordinates(2, rb_corner_coordinates_1),
              Coordinates(2, start_point_coordinates_1), 10),
        Graph(Coordinates(2, lt_corner_coordinates_2),
              Coordinates(2, rb_corner_coordinates_2),
              Coordinates(2, start_point_coordinates_2), 5),
    };

    float light_coordinates_1[3] = {100, 400, 0};
    float brightness_1[3] = {200, 200, 200};
    float light_coordinates_2[3] = {600, 600, 200};
    float brightness_2[3] = {150, 150, 150};
    float light_coordinates_3[3] = {1080, 720, 500};
    float brightness_3[3] = {150, 150, 150};
    float light_coordinates_4[3] = {840, 760, 100};
    float brightness_4[3] = {150, 150, 150};
    Light lights[4] = {
        Light(Coordinates(3, brightness_1), Coordinates(3, light_coordinates_1)),
        Light(Coordinates(3, brightness_2), Coordinates(3, light_coordinates_2)),
        Light(Coordinates(3, brightness_3), Coordinates(3, light_coordinates_3)),
        Light(Coordinates(3, brightness_4), Coordinates(3, light_coordinates_4)),
    };

    float center_coordinates_1[3] = {100, 100, 0};
    float center_coordinates_2[3] = {1020, 620, 50};
    float center_coordinates_3[3] = {500, 300, 100};
    float center_coordinates_4[3] = {150, 300, 0};
    float center_coordinates_5[3] = {540, 360, 0};
    Circle circles[5] = {
        Circle(Coordinates(3, center_coordinates_1), 100),
        Circle(Coordinates(3, center_coordinates_2), 50),
        Circle(Coordinates(3, center_coordinates_3), 200),
        Circle(Coordinates(3, center_coordinates_4), 50),
        Circle(Coordinates(3, center_coordinates_5), 50),
    };

    float start_1[2] = {200, 300};
    float end_1[2] = {300, 200};
    float start_2[2] = {900, 500};
    float end_2[2] = {800, 400};
    MyVector vectors[2] = {
        MyVector(Coordinates(2, start_1),
               Coordinates(2, end_1)),
        MyVector(Coordinates(2, start_2),
               Coordinates(2, end_2)),
    };

    Renderer renderer(1080, 720,
                      circles, sizeof(circles) / sizeof(circles[0]),
                      lights, sizeof(lights) / sizeof(lights[0]),
                      graphs, sizeof(graphs) / sizeof(graphs[0]),
                      vectors, sizeof(vectors) / sizeof(vectors[0]));

    renderer.ShowWindow();

    return EXIT_SUCCESS;
}
