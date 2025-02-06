#pragma once
#include <cmath> 
struct Vector2f {
    float x;
    float y;

    // Constructeurs
    Vector2f() : x(0), y(0) {}
    Vector2f(float xVal, float yVal) : x(xVal), y(yVal) {}

    // Opérateurs mathématiques
    Vector2f operator+(const Vector2f& other) const {
        return Vector2f(x + other.x, y + other.y);
    }

    Vector2f operator-(const Vector2f& other) const {
        return Vector2f(x - other.x, y - other.y);
    }

    Vector2f operator*(float scalar) const {
        return Vector2f(x * scalar, y * scalar);
    }

    Vector2f operator/(float scalar) const {
        return Vector2f(x / scalar, y / scalar);
    }

    // Méthode de calcul de la norme
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // Méthode de normalisation
    Vector2f normalized() const {
        float len = length();
        if (len != 0) {
            return (*this) / len;
        }
        return Vector2f();
    }
};
