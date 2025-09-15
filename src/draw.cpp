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
    Graph* graphs = scene_manager.GetGraphArray();
    size_t graphs_num = scene_manager.GetGraphArrayLen();
    MyVector* vectors = scene_manager.GetVectorArray();
    size_t vectors_num = scene_manager.GetVectorArrayLen();

    sf::RectangleShape background(sf::Vector2f((float)screen_width, (float)screen_height));
    background.setPosition(0.f,0.f);
    background.setFillColor(sf::Color::Cyan);

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            AnalyseKey(event);
        }

        LOG(kDebug, "Drawing window");

        window.draw(background);

        // for (size_t index = 0; index < graphs_num; index++) {
        //     DrawCoordinatesSystem(graphs[index]);
        // }

        DrawCircles();

//         for (size_t index = 0; index < vectors_num; index++) {
//             DrawVector(vectors[index]);
//
//             vectors[index].Rotate(kRotationAngle);
//         }

        usleep(kTimeForSleeping);

        window.display();
    }

    return kDoneRenderer;
}

RendererError Renderer::AnalyseKey(const sf::Event event) {
    enum MoveCamera scancode = (MoveCamera)event.key.scancode;
    if (scancode == kForwardMove) {
        Eye* eye = scene_manager.GetEye();
        (*eye).SetEyePos((*eye).GetEyePos() + !((*eye).GetEyeLTCorner() + (*eye).GetEyeRBCorner()) * kStepEye);
        return kDoneRenderer;
    }
    if (scancode == kBackMove) {
        Eye* eye = scene_manager.GetEye();
        (*eye).SetEyePos((*eye).GetEyePos() - !((*eye).GetEyeLTCorner() + (*eye).GetEyeRBCorner()) * kStepEye);
        return kDoneRenderer;
    }
    if (scancode == kLeftMove) {
        Eye* eye = scene_manager.GetEye();
        (*eye).SetEyePos((*eye).GetEyePos() + !((*eye).GetEyeLTCorner() - (*eye).GetEyeRTCorner()) * kStepEye);
        return kDoneRenderer;
    }
    if (scancode == kRightMove) {
        Eye* eye = scene_manager.GetEye();
        (*eye).SetEyePos((*eye).GetEyePos() - !((*eye).GetEyeLTCorner() - (*eye).GetEyeRTCorner()) * kStepEye);
        return kDoneRenderer;
    }
    if (scancode == kUpRotate) {
        return RotateCameraUp();
    }
    if (scancode == kDownRotate) {
        return RotateCameraDown();
    }
    if (scancode == kLeftRotate) {
        return RotateCameraLeft();
    }
    if (scancode == kRightRotate) {
        return RotateCameraRight();
    }

    return kDoneRenderer;
}

RendererError Renderer::RotateCameraUp() {
    Eye* eye = scene_manager.GetEye();
    Coordinates lt_corner = eye->GetEyeLTCorner();
    Coordinates lb_corner = eye->GetEyeLBCorner();
    Coordinates rt_corner = eye->GetEyeRTCorner();
    Coordinates rb_corner = eye->GetEyeRBCorner();

    Coordinates hor_vec = (lt_corner - rt_corner) / 2;
    Coordinates vec = lt_corner - hor_vec;
    float hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    lt_corner.SetCoordinate(0, hor_vec[0] + vec[0] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    lt_corner.SetCoordinate(1, hor_vec[1] + vec[1] * kCosRotate - kSinRotate * hor_length);
    lt_corner.SetCoordinate(2, hor_vec[2] + vec[2] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeLTCorner(Coordinates(lt_corner));

    vec = lb_corner - hor_vec;
    hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    lb_corner.SetCoordinate(0, hor_vec[0] + vec[0] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    lb_corner.SetCoordinate(1, hor_vec[1] + vec[1] * kCosRotate - kSinRotate * hor_length);
    lb_corner.SetCoordinate(2, hor_vec[2] + vec[2] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeLBCorner(Coordinates(lb_corner));

    vec = rt_corner + hor_vec;
    hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    rt_corner.SetCoordinate(0, -hor_vec[0] + vec[0] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    rt_corner.SetCoordinate(1, -hor_vec[1] + vec[1] * kCosRotate - kSinRotate * hor_length);
    rt_corner.SetCoordinate(2, -hor_vec[2] + vec[2] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeRTCorner(Coordinates(rt_corner));

    vec = rb_corner + hor_vec;
    hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    rb_corner.SetCoordinate(0, -hor_vec[0] + vec[0] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    rb_corner.SetCoordinate(1, -hor_vec[1] + vec[1] * kCosRotate - kSinRotate * hor_length);
    rb_corner.SetCoordinate(2, -hor_vec[2] + vec[2] * (kCosRotate + vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeRBCorner(Coordinates(rb_corner));

    return kDoneRenderer;
}

RendererError Renderer::RotateCameraDown() {
    Eye* eye = scene_manager.GetEye();
    Coordinates lt_corner = eye->GetEyeLTCorner();
    Coordinates lb_corner = eye->GetEyeLBCorner();
    Coordinates rt_corner = eye->GetEyeRTCorner();
    Coordinates rb_corner = eye->GetEyeRBCorner();

    Coordinates hor_vec = (lt_corner - rt_corner) / 2;
    Coordinates vec = lt_corner - hor_vec;
    float hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    lt_corner.SetCoordinate(0, hor_vec[0] + vec[0] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    lt_corner.SetCoordinate(1, hor_vec[1] + vec[1] * kCosRotate + kSinRotate * hor_length);
    lt_corner.SetCoordinate(2, hor_vec[2] + vec[2] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeLTCorner(Coordinates(lt_corner));

    vec = lb_corner - hor_vec;
    hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    lb_corner.SetCoordinate(0, hor_vec[0] + vec[0] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    lb_corner.SetCoordinate(1, hor_vec[1] + vec[1] * kCosRotate + kSinRotate * hor_length);
    lb_corner.SetCoordinate(2, hor_vec[2] + vec[2] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeLBCorner(Coordinates(lb_corner));

    vec = rt_corner + hor_vec;
    hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    rt_corner.SetCoordinate(0, -hor_vec[0] + vec[0] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    rt_corner.SetCoordinate(1, -hor_vec[1] + vec[1] * kCosRotate + kSinRotate * hor_length);
    rt_corner.SetCoordinate(2, -hor_vec[2] + vec[2] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeRTCorner(Coordinates(rt_corner));

    vec = rb_corner + hor_vec;
    hor_length = sqrt(vec[0] * vec[0] + vec[2] * vec[2]);
    rb_corner.SetCoordinate(0, -hor_vec[0] + vec[0] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    rb_corner.SetCoordinate(1, -hor_vec[1] + vec[1] * kCosRotate + kSinRotate * hor_length);
    rb_corner.SetCoordinate(2, -hor_vec[2] + vec[2] * (kCosRotate - vec[1] * kSinRotate / hor_length));
    (*eye).SetEyeRBCorner(Coordinates(rb_corner));

    return kDoneRenderer;
}

RendererError Renderer::RotateCameraLeft() {
    Eye* eye = scene_manager.GetEye();
    Coordinates lt_corner = eye->GetEyeLTCorner();
    Coordinates lb_corner = eye->GetEyeLBCorner();
    Coordinates rt_corner = eye->GetEyeRTCorner();
    Coordinates rb_corner = eye->GetEyeRBCorner();

    Coordinates ver_vec = (lt_corner - lb_corner) / 2;
    Coordinates vec = lt_corner - ver_vec;
    lt_corner.SetCoordinate(0, lt_corner[0] * kCosRotate - lt_corner[2] * kSinRotate);
    lt_corner.SetCoordinate(2, lt_corner[2] * kCosRotate + lt_corner[0] * kSinRotate);
    (*eye).SetEyeLTCorner(Coordinates(lt_corner));

    vec = lb_corner - ver_vec;
    lb_corner.SetCoordinate(0, lb_corner[0] * kCosRotate - lb_corner[2] * kSinRotate);
    lb_corner.SetCoordinate(2, lb_corner[2] * kCosRotate + lb_corner[0] * kSinRotate);
    (*eye).SetEyeLBCorner(Coordinates(lb_corner));

    vec = rt_corner + ver_vec;
    rt_corner.SetCoordinate(0, rt_corner[0] * kCosRotate - rt_corner[2] * kSinRotate);
    rt_corner.SetCoordinate(2, rt_corner[2] * kCosRotate + rt_corner[0] * kSinRotate);
    (*eye).SetEyeRTCorner(Coordinates(rt_corner));

    vec = rb_corner + ver_vec;
    rb_corner.SetCoordinate(0, rb_corner[0] * kCosRotate - rb_corner[2] * kSinRotate);
    rb_corner.SetCoordinate(2, rb_corner[2] * kCosRotate + rb_corner[0] * kSinRotate);
    (*eye).SetEyeRBCorner(Coordinates(rb_corner));

    return kDoneRenderer;
}

RendererError Renderer::RotateCameraRight() {
    Eye* eye = scene_manager.GetEye();
    Coordinates lt_corner = eye->GetEyeLTCorner();
    Coordinates lb_corner = eye->GetEyeLBCorner();
    Coordinates rt_corner = eye->GetEyeRTCorner();
    Coordinates rb_corner = eye->GetEyeRBCorner();

    Coordinates ver_vec = (lt_corner - lb_corner) / 2;
    Coordinates vec = lt_corner - ver_vec;
    lt_corner.SetCoordinate(0, lt_corner[0] * kCosRotate + lt_corner[2] * kSinRotate);
    lt_corner.SetCoordinate(2, lt_corner[2] * kCosRotate - lt_corner[0] * kSinRotate);
    (*eye).SetEyeLTCorner(Coordinates(lt_corner));

    vec = lb_corner - ver_vec;
    lb_corner.SetCoordinate(0, lb_corner[0] * kCosRotate + lb_corner[2] * kSinRotate);
    lb_corner.SetCoordinate(2, lb_corner[2] * kCosRotate - lb_corner[0] * kSinRotate);
    (*eye).SetEyeLBCorner(Coordinates(lb_corner));

    vec = rt_corner + ver_vec;
    rt_corner.SetCoordinate(0, rt_corner[0] * kCosRotate + rt_corner[2] * kSinRotate);
    rt_corner.SetCoordinate(2, rt_corner[2] * kCosRotate - rt_corner[0] * kSinRotate);
    (*eye).SetEyeRTCorner(Coordinates(rt_corner));

    vec = rb_corner + ver_vec;
    rb_corner.SetCoordinate(0, rb_corner[0] * kCosRotate + rb_corner[2] * kSinRotate);
    rb_corner.SetCoordinate(2, rb_corner[2] * kCosRotate - rb_corner[0] * kSinRotate);
    (*eye).SetEyeRBCorner(Coordinates(rb_corner));

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

RendererError Renderer::DrawCircles() {
    Circle* circles = scene_manager.GetCircleArray();
    size_t circles_num = scene_manager.GetCircleArrayLen();
    Circle* lights = scene_manager.GetLightArray();
    size_t lights_num = scene_manager.GetLightArrayLen();

    Eye eye = *(scene_manager.GetEye());
    Coordinates eye_pos = eye.GetEyePos();
    Coordinates lt_corner = eye.GetEyeLTCorner();
    Coordinates lb_corner = eye.GetEyeLBCorner();
    Coordinates rt_corner = eye.GetEyeRTCorner();

    Coordinates hor_vec = !(rt_corner - lt_corner);
    Coordinates ver_vec = !(lb_corner - lt_corner);
    LOG(kDebug, "hor_vec: %f, %f, %f\n", hor_vec[0], hor_vec[1], hor_vec[2]);
    LOG(kDebug, "ver_vec: %f, %f, %f\n", ver_vec[0], ver_vec[1], ver_vec[2]);

    sf::VertexArray vertices (sf::PrimitiveType::Points, (size_t) (screen_width * screen_height));
    Coordinates pixel_pos(eye_pos + lt_corner);
    for (unsigned int i = 0; i < screen_height; i++) {
        pixel_pos = eye_pos + lt_corner + ver_vec * i;
        for (unsigned int j = 0; j < screen_width; j++) {
            pixel_pos = pixel_pos + hor_vec;
            LOG(kDebug, "Pixel pos: %f, %f, %f\n", pixel_pos[0], pixel_pos[1], pixel_pos[2]);

            float coeff = -1;
            Circle circle(Coordinates(0, 0, 0, 0));
            size_t circle_number = -1;
            for (size_t circle_index = 0; circle_index < circles_num; circle_index++) {
                Coordinates center = circles[circle_index].GetCenterCoordinates();
                float radius = circles[circle_index].GetRadius();
                float distance = ((center - pixel_pos) || (pixel_pos - eye_pos)).GetModule()
                                    / (pixel_pos - eye_pos).GetModule();
                if (distance > radius) {
                    continue;
                }
                float a = (pixel_pos - eye_pos).SqLength();
                float b = 2 * ((pixel_pos - eye_pos) && (pixel_pos - center));
                float c = (pixel_pos - center).SqLength() - radius * radius;
                float discrim = b * b - 4 * a * c;
                if (discrim < 0) {
                    continue;
                }
                discrim = sqrt(discrim);
                float res_minus = (-b - discrim) / (2 * a);
                float res_plus = (-b + discrim) / (2 * a);

                if (((res_plus < coeff) || (coeff < 0)) && (res_plus > 0)) {
                    coeff = res_plus;
                    circle = circles[circle_index];
                    circle_number = circle_index;
                }
                if (((res_minus < coeff) || (coeff < 0)) && (res_minus > 0)) {
                    coeff = res_minus;
                    circle = circles[circle_index];
                    circle_number = circle_index;
                }
            }
            if (coeff < 0) {
                vertices[i * screen_width + j].position = {(float)j, (float)i};
                vertices[i * screen_width + j].color = sf::Color::Black;
                continue;
            }
            if (circle.GetType() == kLight) {
                vertices[i * screen_width + j].position = {(float)j, (float)i};
                vertices[i * screen_width + j].color = sf::Color(kMaxColor, kMaxColor, kMaxColor);
                continue;
            }

            Coordinates center = circle.GetCenterCoordinates();

            Coordinates point = pixel_pos + (pixel_pos - eye_pos) * coeff;

            Coordinates color(kIBase);
            for (size_t light_index = 0; light_index < lights_num; light_index++) {
                Circle light(lights[light_index]);
                Coordinates light_coordinates(light.GetCenterCoordinates());
                Coordinates brightness(light.GetBrightness());
                light_coordinates = light_coordinates - center;

                bool drawable = true;
                for (size_t circle_index = 0; circle_index < circles_num; circle_index++) {
                    if (circle_index == circle_number){
                        continue;
                    }
                    Coordinates checking_center = circles[circle_index].GetCenterCoordinates();
                    float distance = ((checking_center - point) || (point - light_coordinates)).GetModule()
                                     / (point - light_coordinates).GetModule();
                    float radius = circles[circle_index].GetRadius();
                    if (distance > radius) {
                        continue;
                    }

                    float a = (point - light_coordinates).SqLength();
                    float b = 2 * ((point - light_coordinates) && (point - checking_center));
                    float c = (point - checking_center).SqLength() - radius * radius;
                    float discrim = b * b - 4 * a * c;
                    if (discrim < 0) {
                        continue;
                    }
                    discrim = sqrt(discrim);
                    float res_minus = (-b - discrim) / (2 * a);
                    float res_plus = (-b + discrim) / (2 * a);

                    if (res_plus < kEpsilon) {
                        drawable = false;
                        break;
                    }
                    if (res_minus < kEpsilon) {
                        drawable = false;
                        break;
                    }
                }
                if (!drawable) {
                    continue;
                }

                float cos_a = (!(point - center)) && (!(light_coordinates - point));

                if (cos_a > 0) {
                    color = color + brightness * cos_a;

                    float cos_b = ((!(point - light_coordinates))
                                    + (!(point - center)) * ((!(point - center)) && (!(light_coordinates - point))) * 2)
                                    && (!(eye_pos - point));

                    LOG (kDebug, "Drawing Circles\n"
                                "\t Analyzing point\n"
                                "\t\t Cos a = %f\n"
                                "\t\t Cos b = %f\n",
                                cos_a, cos_b);

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

            vertices[i * screen_width + j].position = {(float)j, (float)i};
            vertices[i * screen_width + j].color = sf::Color((uint8_t)color[0], (uint8_t)color[1], (uint8_t)color[2]);
        }
    }

    window.draw(vertices);

    return kDoneRenderer;
}

//     Coordinates save_eye_pos(eye_pos);
//     for (size_t circle_index = 0; circle_index < circles_num; circle_index++) {
//     Circle circle = circles[circle_index];
//     Coordinates center = circle.GetCenterCoordinates();
//     Coordinates eye_pos = save_eye_pos - center;
//     float center_x = center[0];
//     float center_y = center[1];
//     float radius = circle.GetRadius();
//     float lt_corner_x = center_x - radius;
//     float lt_corner_y = center_y - radius;;
//     float width = radius * 2;
//     float height = radius * 2;
//
//     LOG(kDebug, "Draw Circles\n"
//                 "\t Height = %f\n"
//                 "\t Width  = %f\n"
//                 "\t Radius = %f\n",
//                 height, width, radius);
//
//     sf::VertexArray vertices (sf::PrimitiveType::Points, (size_t) (height * width));
//     size_t vertices_num = 0;
//     for (float i = 0; i < height; i++) {
//         float y = i + lt_corner_y - center_y;
//         for (float j = 0; j < width; j++) {
//             float x = j + lt_corner_x - center_x;
//
//             if (y * y + x * x > radius * radius) {
//                 continue;
//             }
//
//             float z = sqrt(radius * radius - x * x - y * y) + center[2];
//             if (z > keye_posHeight) {
//                 continue;
//             }
//             Coordinates pixel_pos(3, x, y, z);
//
//             // bool drawable = true;
//             // for (size_t circle_index = 0; circle_index < circles_num; circle_index++) {
//             //     Coordinates checking_center = circles[circle_index].GetCenterCoordinates() - center;
//             //     float distance = ((checking_center - pixel_pos) || (pixel_pos - eye_pos)).GetModule()
//             //                         / (pixel_pos - eye_pos).GetModule();
//             //     if (distance > circles[circle_index].GetRadius()) {
//             //         continue;
//             //     }
//             //     float cos = (!(pixel_pos - checking_center)) && (!(pixel_pos - eye_pos));
//             //     if (cos > 0) {
//             //         LOG(kDebug, "cos = %f\n", cos);
//             //         drawable = false;
//             //         break;
//             //     }
//             // }
//             // if (!drawable) {
//             //     continue;
//             // }
//
//             Coordinates color(kIBase);
//             for (size_t light_index = 0; light_index < lights_num; light_index++) {
//                 Light light(lights[light_index]);
//                 Coordinates light_coordinates(light.GetPosition());
//                 Coordinates brightness(light.GetBrightness());
//                 light_coordinates = light_coordinates - center;
//
//                 bool drawable = true;
//                 for (size_t circle_index = 0; circle_index < circles_num; circle_index++) {
//                     Coordinates checking_center = circles[circle_index].GetCenterCoordinates() - center;
//                     float distance = ((checking_center - pixel_pos) || (pixel_pos - light_coordinates)).GetModule()
//                                      / (pixel_pos - light_coordinates).GetModule();
//                     if (distance > circles[circle_index].GetRadius()) {
//                         continue;
//                     }
//                     float cos = (!(pixel_pos - checking_center)) && (!(pixel_pos - light_coordinates));
//                     if (cos > 0) {
//                         drawable = false;
//                         break;
//                     }
//                 }
//                 if (!drawable) {
//                     continue;
//                 }
//
//                 float cos_a = (!pixel_pos) && (!(light_coordinates - pixel_pos));
//
//                 if (cos_a > 0) {
//                     color = color + brightness * cos_a;
//
//                     float cos_b = ((!(pixel_pos - light_coordinates))
//                                     + (!pixel_pos) * ((!pixel_pos) && (!(light_coordinates - pixel_pos))) * 2)
//                                     && (!(eye_pos - pixel_pos));
//
//                     LOG (kDebug, "Drawing Circles\n"
//                                 "\t Analyzing point\n"
//                                 "\t\t X = %f\n"
//                                 "\t\t Y = %f\n"
//                                 "\t\t Cos a = %f\n"
//                                 "\t\t Cos b = %f\n",
//                                 x, y, cos_a, cos_b);
//
//                     if (cos_b > 0) {
//                         color = color + kMaxColor * powf32(cos_b, kPowCosB);
//                     }
//                 }
//
//                 for (size_t i = 0; i < 3; i++)
//                 {
//                     if (color[i] > kMaxColor) {
//                         color.SetCoordinate(i, kMaxColor);
//                     }
//                 }
//             }
//
//             vertices[vertices_num].position = {lt_corner_x + j, lt_corner_y + i};
//             vertices[vertices_num].color = sf::Color((uint8_t)color[0], (uint8_t)color[1], (uint8_t)color[2]);
//             vertices_num++;
//         }
//     }
//
//     window.draw(vertices);
//     }
//     return kDoneRenderer;
// }

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
