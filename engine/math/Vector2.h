#pragma once

struct Vector2 {
	float x, y;

    Vector2& operator+=(const Vector2& other) {
        this->x += other.x;
        this->y += other.y;
        return *this; 
    }
};