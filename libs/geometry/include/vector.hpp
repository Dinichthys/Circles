#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>
#include <malloc.h>
#include <stdint.h>

#include "my_assert.h"

const float kVectorWidth = 1.f;
const float kArrowLenPercentage  = 0.125f;
const float kArrowAngle = 30.f;
const float kRotationAngle = 5.f * M_PI / 180;

enum CoordinatesError {
    kDoneCoordinates    = 0,
    kCantSetCoordinates = 1,
};

class Coordinates {
    private:
        float* coordinates;
        size_t dimension;

    public:
        Coordinates(const Coordinates& need_copy) {
            ASSERT(need_copy.coordinates != NULL, "");
            dimension = need_copy.dimension;
            coordinates = (float*) calloc(dimension, sizeof(float));
            if (coordinates == NULL) {
                throw "Can't calloc coordinates";
            }
            for (size_t i = 0; i < dimension; i++) {
                coordinates[i] = need_copy.coordinates[i];
            }
        };
        Coordinates(size_t dimension, const float* values) {
            ASSERT(values != NULL, "");
            Coordinates::dimension = dimension;
            coordinates = (float*) calloc(dimension, sizeof(float));
            if (coordinates == NULL) {
                throw "Can't calloc coordinates";
            }
            for (size_t i = 0; i < dimension; i++) {
                coordinates[i] = values[i];
            }
        };
        ~Coordinates() {
            free(coordinates);
            coordinates = NULL;
        }

        CoordinatesError SetCoordinate(size_t index, float value) {
            if (index >= dimension) {
                return kCantSetCoordinates;
            }
            coordinates[index] = value;

            return kDoneCoordinates;
        };
        float GetCoordinate(size_t index) const {
            if (index >= dimension) {
                return NAN;
            }
            return coordinates[index];
        };
        float* GetArrayCoordinates() const {
            return coordinates;
        };
        size_t GetDimension() const {
            return dimension;
        };
        float GetModule() const {
            float mod = 0;
            for (size_t i = 0; i < dimension; i++)  {
                mod += coordinates[i] * coordinates[i];
            }
            return sqrt(mod);
        };

        float operator [] (size_t index) const {
            if (index >= dimension) {
                return NAN;
            }
            return coordinates[index];
        };
        Coordinates operator + (const Coordinates& a) const {
            float* result = (float*) calloc(dimension, sizeof(float));
            if (result == NULL) {
                return Coordinates(0, NULL);
            }
            for (size_t i = 0; i < dimension; i++) {
                result[i] = coordinates[i] + a[i];
            }
            Coordinates ret_val(dimension, result);
            free(result);
            return ret_val;
        };
        Coordinates operator + (float val) const {
            float* result = (float*) calloc(dimension, sizeof(float));
            if (result == NULL) {
                return Coordinates(0, NULL);
            }
            for (size_t i = 0; i < dimension; i++) {
                result[i] = coordinates[i] + val;
            }
            Coordinates ret_val(dimension, result);
            free(result);
            return ret_val;
        };
        Coordinates operator - (const Coordinates& a) const {
            float* result = (float*) calloc(dimension, sizeof(float));
            if (result == NULL) {
                return Coordinates(0, NULL);
            }
            for (size_t i = 0; i < dimension; i++) {
                result[i] = coordinates[i] - a[i];
            }
            Coordinates ret_val(dimension, result);
            free(result);
            return ret_val;
        };
        Coordinates operator - (float val) const {
            float* result = (float*) calloc(dimension, sizeof(float));
            if (result == NULL) {
                return Coordinates(0, NULL);
            }
            for (size_t i = 0; i < dimension; i++) {
                result[i] = coordinates[i] - val;
            }
            Coordinates ret_val(dimension, result);
            free(result);
            return ret_val;
        };
        Coordinates operator * (float val) const {
            float* result = (float*) calloc(dimension, sizeof(float));
            if (result == NULL) {
                return Coordinates(0, NULL);
            }
            for (size_t i = 0; i < dimension; i++) {
                result[i] = coordinates[i] * val;
            }
            Coordinates ret_val(dimension, result);
            free(result);
            return ret_val;
        };
        Coordinates operator / (float val) const {
            float* result = (float*) calloc(dimension, sizeof(float));
            if (result == NULL) {
                return Coordinates(0, NULL);
            }
            for (size_t i = 0; i < dimension; i++) {
                result[i] = coordinates[i] / val;
            }
            Coordinates ret_val(dimension, result);
            free(result);
            return ret_val;
        };
        Coordinates operator ! () {
            float len = GetModule();
            return *this / len;
        };
        float operator ^ (const Coordinates& a) const {
            float result = 0;
            for (size_t i = 0; i < dimension; i++) {
                result += coordinates[i] * a[i];
            }
            return result;
        };
        void operator = (const Coordinates& need_copy) {
            ASSERT(need_copy.coordinates != NULL, "");
            if (coordinates != NULL) {
                free(coordinates);
                coordinates = NULL;
            }
            dimension = need_copy.dimension;
            coordinates = (float*) calloc(dimension, sizeof(float));
            if (coordinates == NULL) {
                throw "Can't calloc coordinates";
            }
            for (size_t i = 0; i < dimension; i++) {
                coordinates[i] = need_copy.coordinates[i];
            }
        };
};

class MyVector {
    private:
        Coordinates start;
        Coordinates end;

    public:
        MyVector(Coordinates start_val, Coordinates end_val)
            :start(start_val), end(end_val) {
            if (start.GetDimension() != end.GetDimension()) {
                throw "Different dimensions of the ends of vector";
            }
        };

        Coordinates GetStartCoordinates() {return start;};
        Coordinates GetEndCoordinates() {return end;};

        float GetAngle();
        float Length() {
            size_t size = start.GetDimension();
            float len_sq = 0;
            for (size_t i = 0; i < size; i++) {
                float x = start[i] - end[i];
                len_sq += x * x;
            }
            return sqrt(len_sq);
        }
        void Rotate(float angle);
};

#endif // VECTOR_HPP
