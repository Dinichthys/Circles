#ifndef DRAW_HPP
#define DRAW_HPP

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "graph.hpp"
#include "circle.hpp"
#include "vector.hpp"

#include "my_assert.h"

const char* const kWindowName = "Graphics";
const Coordinates kIBase(3, 19, 19, 19);
const float kEyeHeight = 1000;
const float kPowCosB = 32;
const float kMaxColor = 255;
const float kWindowDistance = 688;
const float kStepEye = 10;
const float kCosRotate = 0.9961947;
const float kSinRotate = 0.08715574;
const float kEpsilon = 0.1;

enum RendererError {
    kDoneRenderer = 0,
};

enum MoveCamera {
    kRightMove   = 86,  // Keyboard Right arrow
    kLeftMove    = 87,  // Keyboard Left arrow
    kBackMove    = 88,  // Keyboard Down arrow
    kForwardMove = 89,  // Keyboard Up arrow

    kRightRotate = 3,  // Keyboard D and d key
    kLeftRotate  = 0,  // Keyboard A and a key
    kDownRotate  = 18,  // Keyboard S and s key
    kUpRotate    = 22,  // Keyboard W and w key
};

class Eye {
    private:
        Coordinates pos;
        Coordinates lt_corner;
        Coordinates lb_corner;
        Coordinates rt_corner;
        Coordinates rb_corner;

    public:
        explicit Eye(Coordinates pos_val,
                     Coordinates lt_val, Coordinates lb_val, Coordinates rt_val, Coordinates rb_val)
            :pos(pos_val), lt_corner(lt_val), lb_corner(lb_val), rt_corner(rt_val), rb_corner(rb_val) {};

        Coordinates GetEyePos() const {return Coordinates(pos);};
        Coordinates GetEyeLTCorner() const {return Coordinates(lt_corner);};
        Coordinates GetEyeLBCorner() const {return Coordinates(lb_corner);};
        Coordinates GetEyeRTCorner() const {return Coordinates(rt_corner);};
        Coordinates GetEyeRBCorner() const {return Coordinates(rb_corner);};

        void SetEyePos(Coordinates pos_val) {pos = pos_val;};
        void SetEyePosX(float x) {pos.SetCoordinate(0, x);};
        void SetEyePosY(float y) {pos.SetCoordinate(1, y);};
        void SetEyePosZ(float z) {pos.SetCoordinate(2, z);};

        void SetEyeLTCorner(Coordinates lt_corner_val) {lt_corner = lt_corner_val;};
        void SetEyeLBCorner(Coordinates lb_corner_val) {lb_corner = lb_corner_val;};
        void SetEyeRTCorner(Coordinates rt_corner_val) {rt_corner = rt_corner_val;};
        void SetEyeRBCorner(Coordinates rb_corner_val) {rb_corner = rb_corner_val;};
};

class SceneManager {
    private:
        Circle* circles;
        size_t circles_num;

        Circle*  lights;
        size_t lights_num;

        Graph* graphs;
        size_t graphs_num;

        MyVector* vectors;
        size_t vectors_num;

        Eye eye;

    public:
        SceneManager(const Circle* circles_val, size_t circles_num, const Graph* graphs_val, size_t graphs_num,
                     const MyVector* vectors_val, size_t vectors_num,
                     const Eye& eye_val)
            :eye(eye_val) {
            ASSERT(circles_val != NULL, "");
            ASSERT(lights_val != NULL, "");
            ASSERT(graphs_val != NULL, "");
            ASSERT(vectors_val != NULL, "");

            size_t lights_num = 0;
            circles = (Circle*) calloc(circles_num, sizeof(Circle));
            SceneManager::circles_num = circles_num;
            if (circles == NULL) {
                lights = NULL;
                throw "Cant create SceneManager";
            }
            for (size_t i = 0; i < circles_num; i++) {
                circles[i] = circles_val[i];
                if (circles_val[i].GetType() == kLight) {
                    lights_num++;
                }
            }

            lights = (Circle*) calloc(lights_num, sizeof(Circle));
            SceneManager::lights_num = lights_num;
            if (lights == NULL) {
                free(circles);
                circles = NULL;
                throw "Cant create SceneManager";
            }
            lights_num = 0;
            for (size_t i = 0; i < circles_num; i++) {
                if (circles_val[i].GetType() == kLight) {
                    lights[lights_num] = circles_val[i];
                    lights_num++;
                }
            }

            graphs = (Graph*) calloc(graphs_num, sizeof(Graph));
            SceneManager::graphs_num = graphs_num;
            if (graphs == NULL) {
                free(circles);
                circles = NULL;
                free(lights);
                lights = NULL;
                throw "Cant create SceneManager";
            }
            for (size_t i = 0; i < graphs_num; i++) {
                graphs[i] = graphs_val[i];
            }

            vectors = (MyVector*) calloc(vectors_num, sizeof(MyVector));
            SceneManager::vectors_num = vectors_num;
            if (vectors == NULL) {
                free(circles);
                circles = NULL;
                free(lights);
                lights = NULL;
                free(graphs);
                graphs = NULL;
                throw "Cant create SceneManager";
            }
            for (size_t i = 0; i < vectors_num; i++) {
                vectors[i] = vectors_val[i];
            }
        };

        ~SceneManager() {
            free(circles);
            free(lights);
            free(graphs);
            free(vectors);

            circles = NULL;
            lights = NULL;
            graphs = NULL;
            vectors = NULL;
        };

        Circle* GetCircleArray() const {return circles;};
        size_t  GetCircleArrayLen() const {return circles_num;};
        Circle* GetLightArray() const {return lights;};
        size_t  GetLightArrayLen() const {return lights_num;};
        Graph* GetGraphArray() const {return graphs;};
        size_t  GetGraphArrayLen() const {return graphs_num;};
        MyVector* GetVectorArray() const {return vectors;};
        size_t  GetVectorArrayLen() const {return vectors_num;};
        Eye* GetEye() {return &eye;};
};

class Renderer {
    private:
        sf::RenderWindow window;
        unsigned int screen_width;
        unsigned int screen_height;

        SceneManager scene_manager;

    public:
        explicit Renderer(unsigned int width, unsigned int height,
                 const Circle* circles, size_t circles_num,
                 const Graph* graphs, size_t graphs_num,
                 const MyVector* vectors, size_t vectors_num)
            : window(sf::VideoMode ({width, height}), kWindowName),
              scene_manager(circles, circles_num, graphs, graphs_num, vectors, vectors_num,
                            Eye(Coordinates(3, (float)(width / 2), (float)(height / 2), - kEyeHeight),
                                Coordinates(3,-(float)(width / 2),-(float)(height / 2), kWindowDistance),
                                Coordinates(3,-(float)(width / 2), (float)(height / 2), kWindowDistance),
                                Coordinates(3, (float)(width / 2),-(float)(height / 2), kWindowDistance),
                                Coordinates(3, (float)(width / 2), (float)(height / 2), kWindowDistance))) {
            screen_width = width;
            screen_height = height;
        };
        ~Renderer() {
            if (window.isOpen()) {
                window.close();
            }

            screen_width = 0;
            screen_height = 0;
        };

        RendererError ShowWindow();

    private:
        RendererError AnalyseKey(const sf::Event event);
        RendererError RotateCameraUp();
        RendererError RotateCameraDown();
        RendererError RotateCameraLeft();
        RendererError RotateCameraRight();
        RendererError DrawCoordinatesSystem(Graph graph);
        RendererError DrawAxis(Graph graph);
        RendererError DrawGraph(Graph graph, float (*func)(float));
        RendererError DrawCircles();
        RendererError DrawVector(MyVector vector);
};

#endif // DRAW_HPP
