#include <stdlib.h>
#include <stdio.h>

#include "draw.hpp"

#include "logging.h"

int main() {
    set_log_lvl(kError);

    Graph graphs[2] = {
        Graph(Coordinates(2, 100, 100),
              Coordinates(2, 620, 620),
              Coordinates(2, 200, 300), 10),
        Graph(Coordinates(2, 720, 320),
              Coordinates(2, 1020, 620),
              Coordinates(2, 900, 500), 5),
    };

    Light lights[4] = {
        Light(Coordinates(3, 200, 200, 200), Coordinates(3, 100, 400, 0)),
        Light(Coordinates(3, 150, 150, 150), Coordinates(3, 600, 600, 1000)),
        Light(Coordinates(3, 150, 150, 150), Coordinates(3, 1080, 720, -500)),
        Light(Coordinates(3, 150, 150, 150), Coordinates(3, 840, 760, 100)),
    };

    Circle circles[8] = {
        Circle(Coordinates(3, 100, 0, 0), 400),
        Circle(Coordinates(3, 1000, 1000, 0), 400),
        Circle(Coordinates(3, 1000, 0, 0), 400),
        Circle(Coordinates(3, 500, 300, 100), 200),

        Circle(Coordinates(3, 100, 400, 0),     50, kLight, Coordinates(3, 200, 200, 200)),
        Circle(Coordinates(3, 600, 600, 1000),  50, kLight, Coordinates(3, 150, 150, 150)),
        Circle(Coordinates(3, 1080, 720, -500), 50, kLight, Coordinates(3, 150, 150, 150)),
        Circle(Coordinates(3, 840, 760, 100),   50, kLight, Coordinates(3, 150, 150, 150)),
    };

    MyVector vectors[2] = {
        MyVector(Coordinates(2, 200, 300),
               Coordinates(2, 300, 200)),
        MyVector(Coordinates(2, 900, 500),
               Coordinates(2, 800, 400)),
    };

    Renderer renderer(1080, 720,
                      circles, sizeof(circles) / sizeof(circles[0]),
                    //   lights, sizeof(lights) / sizeof(lights[0]),
                      graphs, sizeof(graphs) / sizeof(graphs[0]),
                      vectors, sizeof(vectors) / sizeof(vectors[0]));

    renderer.ShowWindow();

    return EXIT_SUCCESS;
}
