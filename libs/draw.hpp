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
const int kTimeForSleeping = 100000;
const float kIBaseRGB[3] = {19, 19, 19};
const Coordinates kIBase(3, kIBaseRGB);
const float kEyeHeight = 500;
const float kPowCosB = 32;
const float kMaxColor = 255;

enum RendererError {
    kDoneRenderer = 0,
};

class Light {
    private:
        Coordinates brightness;
        Coordinates position;

    public:
        Light(Coordinates brightness_val, Coordinates position_val)
            :brightness(brightness_val), position(position_val) {};

        Coordinates GetBrightness() const {
            return brightness;
        };
        Coordinates GetPosition() const {
            return position;
        };
};

class SceneManager {
    private:
        Circle* circles;
        size_t circles_num;

        Light*  lights;
        size_t lights_num;

        Graph* graphs;
        size_t graphs_num;

        MyVector* vectors;
        size_t vectors_num;

    public:
        SceneManager(const Circle* circles_val, size_t circles_num, const Light* lights_val, size_t lights_num,
                     const Graph* graphs_val, size_t graphs_num, const MyVector* vectors_val, size_t vectors_num) {
            ASSERT(circles != NULL, "");
            ASSERT(lights != NULL, "");

            circles = (Circle*) calloc(circles_num, sizeof(Circle));
            SceneManager::circles_num = circles_num;
            if (circles == NULL) {
                lights = NULL;
                throw "Cant create SceneManager";
            }
            lights = (Light*) calloc(lights_num, sizeof(Light));
            SceneManager::lights_num = lights_num;
            if (lights == NULL) {
                free(circles);
                circles = NULL;
                throw "Cant create SceneManager";
            }
            graphs = (Graph*) calloc(graphs_num, sizeof(Graph));
            SceneManager::graphs_num = graphs_num;
            if (lights == NULL) {
                free(circles);
                circles = NULL;
                free(lights);
                lights = NULL;
                throw "Cant create SceneManager";
            }
            vectors = (MyVector*) calloc(vectors_num, sizeof(MyVector));
            SceneManager::vectors_num = vectors_num;
            if (lights == NULL) {
                free(circles);
                circles = NULL;
                free(lights);
                lights = NULL;
                free(graphs);
                graphs = NULL;
                throw "Cant create SceneManager";
            }


            for (size_t i = 0; i < circles_num; i++) {
                circles[i] = circles_val[i];
            }
            for (size_t i = 0; i < lights_num; i++) {
                lights[i] = lights_val[i];
            }
            for (size_t i = 0; i < graphs_num; i++) {
                graphs[i] = graphs_val[i];
            }
            for (size_t i = 0; i < vectors_num; i++) {
                vectors[i] = vectors_val[i];
            }
        };

        ~SceneManager() {
            free(circles);
            free(lights);

            circles = NULL;
            lights = NULL;
        };

        Circle* GetCircleArray() const {return circles;};
        size_t  GetCircleArrayLen() const {return circles_num;};
        Light* GetLightArray() const {return lights;};
        size_t  GetLightArrayLen() const {return lights_num;};
        Graph* GetGraphArray() const {return graphs;};
        size_t  GetGraphArrayLen() const {return graphs_num;};
        MyVector* GetVectorArray() const {return vectors;};
        size_t  GetVectorArrayLen() const {return vectors_num;};
};

class Renderer {
    private:
        sf::RenderWindow window;
        unsigned int screen_width;
        unsigned int screen_height;

        SceneManager scene_manager;

    public:
        Renderer(unsigned int width, unsigned int height,
                 const Circle* circles, size_t circles_num, const Light* lights, size_t lights_num,
                 const Graph* graphs, size_t graphs_num, const MyVector* vectors, size_t vectors_num)
            : window(sf::VideoMode ({width, height}), kWindowName),
              scene_manager(circles, circles_num, lights, lights_num, graphs, graphs_num, vectors, vectors_num) {
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
        RendererError DrawCoordinatesSystem(Graph graph);
        RendererError DrawAxis(Graph graph);
        RendererError DrawGraph(Graph graph, float (*func)(float));
        RendererError DrawCircle(Circle circle);
        RendererError DrawVector(MyVector vector);
};

#endif // DRAW_HPP
