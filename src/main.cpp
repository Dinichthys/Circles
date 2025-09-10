#include <stdlib.h>
#include <stdio.h>

#include "draw.hpp"

#include "logging.h"

int main() {
    set_log_lvl(kDebug);

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

    float light_coordinates[3] = {100, 400, 100};
    float brightness[3] = {200, 200, 200};
    Light lights[1] = {
        Light(Coordinates(3, brightness), Coordinates(3, light_coordinates)),
    };

    Circle circles[2] = {
        Circle(Coordinates(2, start_point_coordinates_1), 100),
        Circle(Coordinates(2, start_point_coordinates_2), 50),
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
