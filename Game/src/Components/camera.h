#pragma once

struct Camera {
    Camera(float leftf = -480.0f, float rightf = 480.0f, float bottomf=-270.0f, float topf=270.0f, float znear=-1.0f, float zfar=1.0f, float x=0, float y=0, float z=0):
    leftf(leftf), rightf(rightf), bottomf(bottomf), topf(topf), znear(znear), zfar(zfar), x(x), y(y), z(z) {}

    float leftf, rightf, bottomf, topf;
    float znear, zfar;
    float x, y, z;
};
