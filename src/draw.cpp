#include "draw.hpp"

#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "graph.hpp"
#include "circle.hpp"

#include "logging.h"
#include "my_assert.h"

RendererError Renderer::ShowWindow() {
    Circle* circles = scene_manager.GetCircleArray();
    size_t circles_num = scene_manager.GetCircleArrayLen();
    Graph* graphs = scene_manager.GetGraphArray();
    size_t graphs_num = scene_manager.GetGraphArrayLen();
    MyVector* vectors = scene_manager.GetVectorArray();
    size_t vectors_num = scene_manager.GetVectorArrayLen();

    sf::RectangleShape background(sf::Vector2f((float)screen_width, (float)screen_height));
    background.setPosition(0.f,0.f);
    background.setFillColor(sf::Color::Black);

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        LOG(kDebug, "Drawing window");

        window.draw(background);

        for (size_t index = 0; index < graphs_num; index++) {
            DrawCoordinatesSystem(graphs[index]);
        }

        for (size_t index = 0; index < circles_num; index++) {
            DrawCircle(circles[index]);
        }

        for (size_t index = 0; index < vectors_num; index++) {
            DrawVector(vectors[index]);

            vectors[index].Rotate(kRotationAngle);
        }

        usleep(kTimeForSleeping);

        window.display();
    }

    return kDoneRenderer;
}

RendererError Renderer::DrawCoordinatesSystem(Graph graph) {
    Coordinates lt_corner(graph.GetLTCoordinates());
    Coordinates rb_corner(graph.GetRBCoordinates());

    LOG(kDebug, "Starting drawing coordinates system");

    sf::RectangleShape rectangle(sf::Vector2f(
        rb_corner[0] - lt_corner[0],
        rb_corner[1] - lt_corner[1]
    ));
    rectangle.setPosition(lt_corner[0], lt_corner[1]);
    rectangle.setOutlineColor(sf::Color::Black);
    window.draw(rectangle);

    DrawAxis(graph);

    return kDoneRenderer;
}

RendererError Renderer::DrawAxis(Graph graph) {
    Coordinates lt_corner(graph.GetLTCoordinates());
    Coordinates rb_corner(graph.GetRBCoordinates());
    Coordinates start_point = graph.GetStartCoordinates();
    float scale = graph.GetScale();

    LOG(kDebug, "Starting drawing axis");

    float rb_corner_x = rb_corner[0];
    float rb_corner_y = rb_corner[1];
    float lt_corner_x = lt_corner[0];
    float lt_corner_y = lt_corner[1];
    float start_point_x = start_point[0];
    float start_point_y = start_point[1];

    if ((start_point_x > lt_corner_x) &&
        (start_point_x < rb_corner_x)) {
        sf::RectangleShape axis(sf::Vector2f(kAxisWidth, rb_corner_y - lt_corner_y));
        axis.setPosition(start_point_x, lt_corner_y);
        axis.setFillColor(sf::Color::Blue);
        window.draw(axis);
    }

    if ((start_point_y > lt_corner_y) &&
        (start_point_y < rb_corner_y)) {
        sf::RectangleShape axis(sf::Vector2f(rb_corner_x - lt_corner_x, kAxisWidth));
        axis.setPosition(lt_corner_x, start_point_y);
        axis.setFillColor(sf::Color::Blue);
        window.draw(axis);
    }

    sf::RectangleShape line({rb_corner_x - lt_corner_x, kGridWidth});
    sf::Color grid_line_color = sf::Color::Blue;
    grid_line_color.a = kGridBrightness;
    line.setFillColor(grid_line_color);

    float top_point = floor((lt_corner_y - start_point_y) / scale);
    top_point = (top_point * scale + start_point_y < lt_corner_y) ? top_point + 1 : top_point;
    for (; top_point * scale + start_point_y < rb_corner_y; top_point++) {
        line.setPosition({lt_corner_x, top_point * scale + start_point_y});
        window.draw(line);
        LOG(kDebug, "Horizontal line y = %f", top_point);
    }

    float left_point = floor((lt_corner_x - start_point_x) / scale);
    line.setSize({kGridWidth, rb_corner_y - lt_corner_y});
    left_point = (left_point * scale + start_point_x < lt_corner_x) ? left_point + 1 : left_point;
    for (; left_point * scale + start_point_x < rb_corner_x; left_point++) {
        line.setPosition({left_point * scale + start_point_x, lt_corner_y});
        window.draw(line);
        LOG(kDebug, "Vertical line x = %f", left_point);
    }

    return kDoneRenderer;
}

RendererError Renderer::DrawGraph(Graph graph, float (*func)(float)) {
    ASSERT(func != NULL, "Invalid func pointer");

    LOG(kDebug, "Starting drawing graph");

    Coordinates lt_corner = graph.GetLTCoordinates();
    Coordinates rb_corner = graph.GetRBCoordinates();
    Coordinates start_point = graph.GetStartCoordinates();
    float scale = graph.GetScale();

    float rb_corner_x = rb_corner[0];
    float rb_corner_y = rb_corner[1];
    float lt_corner_x = lt_corner[0];
    float lt_corner_y = lt_corner[1];
    float start_point_x = start_point[0];
    float start_point_y = start_point[1];

    float right_side = rb_corner_x - start_point_x;

    size_t vertex_size = (size_t) floor((double)((rb_corner_x - lt_corner_x) / kStep));
    size_t point_index = 0;

    sf::VertexArray vertices (sf::PrimitiveType::Points, vertex_size);

    for (float x = lt_corner_x - start_point_x;
        x < right_side; x += kStep) {
        float y = - func(x / scale) * scale;

        if ((y > lt_corner_y - start_point_y) &&
            (y < rb_corner_y - start_point_y)) {
            vertices [point_index].position = {x + start_point_x, y + start_point_y};

            vertices [point_index].color = sf::Color::Blue;

            point_index++;
        }
    }

    window.draw(vertices);

    return kDoneRenderer;
}

RendererError Renderer::DrawCircle(Circle circle) {
    Coordinates center = circle.GetCenterCoordinates();
    Light* lights = scene_manager.GetLightArray();
    size_t lights_num = scene_manager.GetLightArrayLen();

    float eye_coordinates[3] = {(float) (screen_width / 2), (float) (screen_height / 2), kEyeHeight};
    Coordinates eye(3, eye_coordinates);

    float center_x = center[0];
    float center_y = center[1];
    float radius = circle.GetRadius();
    float lt_corner_x = center_x - radius;
    float lt_corner_y = center_y - radius;;
    float width = radius * 2;
    float height = radius * 2;

    LOG(kDebug, "Draw Circles\n"
                "\t Height = %f\n"
                "\t Width  = %f\n"
                "\t Radius = %f\n",
                height, width, radius);

    sf::VertexArray vertices (sf::PrimitiveType::Points, (size_t) (height * width));
    size_t vertices_num = 0;
    for (float i = 0; i < height; i++) {
        float y = i + lt_corner_y - center_y;
        for (float j = 0; j < width; j++) {
            float x = j + lt_corner_x - center_x;

            if (y * y + x * x > radius * radius) {
                continue;
            }

            float z = sqrt(radius * radius - x * x - y * y);
            float coordinates[3] = {x, y, z};
            Coordinates pixel_pos(3, coordinates);
            Coordinates color(kIBase);
            for (size_t light_index = 0; light_index < lights_num; light_index++) {
                Light light(lights[light_index]);
                Coordinates light_coordinates(light.GetPosition());
                Coordinates brightness(light.GetBrightness());
                light_coordinates.SetCoordinate(0, light_coordinates[0] -  center_x);
                light_coordinates.SetCoordinate(1, light_coordinates[1] -  center_y);

                float cos_a = (!pixel_pos)^(!(light_coordinates - pixel_pos));

                if (cos_a > 0) {
                    color = color + brightness * cos_a;

                    float cos_b = ((!(pixel_pos - light_coordinates))
                                    + (!pixel_pos) * abs((!pixel_pos)^(!(pixel_pos - light_coordinates))) * 2)
                                    ^(!(eye - pixel_pos));

                    LOG (kDebug, "Drawing Circles\n"
                                "\t Analyzing point\n"
                                "\t\t X = %f\n"
                                "\t\t Y = %f\n"
                                "\t\t Cos a = %f\n"
                                "\t\t Cos b = %f\n",
                                x, y, cos_a, cos_b);

                    if (cos_b > 0) {
                        color = color + kMaxColor * powf32(cos_b, kPowCosB);
                    }
                }

                for (size_t i = 0; i < 3; i++)
                {
                    if (color[i] > kMaxColor) {
                        color.SetCoordinate(i, kMaxColor);
                    }
                }
            }

            vertices[vertices_num].position = {lt_corner_x + j, lt_corner_y + i};
            vertices[vertices_num].color = sf::Color((uint8_t)color[0], (uint8_t)color[1], (uint8_t)color[2]);
            vertices_num++;
        }
    }

    window.draw(vertices);

    return kDoneRenderer;
}

RendererError Renderer::DrawVector(MyVector vector) {
    Coordinates start_coordinates(vector.GetStartCoordinates());
    Coordinates end_coordinates(vector.GetEndCoordinates());

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(start_coordinates[0], start_coordinates[1])),
        sf::Vertex(sf::Vector2f(end_coordinates[0], end_coordinates[1]))
    };
    line->color = (sf::Color::Black);
    Coordinates vec(end_coordinates);
    vec.SetCoordinate(0, end_coordinates[0] - start_coordinates[0]);
    vec.SetCoordinate(1, end_coordinates[1] - start_coordinates[1]);

    Coordinates normal(vec);
    normal.SetCoordinate(0, -vec[1]);
    normal.SetCoordinate(1, vec[0]);

    window.draw(line, 2, sf::Lines);

    line[0] = sf::Vertex(sf::Vector2f(end_coordinates[0], end_coordinates[1]));
    float length = vector.Length();
    normal = (!normal) * length * kArrowLenPercentage;
    vec = (!vec) * length * kArrowLenPercentage;

    line[1] = sf::Vertex(sf::Vector2f(end_coordinates[0] + normal[0] - vec[0],
                                      end_coordinates[1] + normal[1] - vec[1]));

    window.draw(line, 2, sf::Lines);

    line[1] = sf::Vertex(sf::Vector2f(end_coordinates[0] - normal[0] - vec[0],
                                      end_coordinates[1] - normal[1] - vec[1]));

    window.draw(line, 2, sf::Lines);

    return kDoneRenderer;
}
