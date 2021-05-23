#pragma once

struct Transform {
   Transform(
        float xpos = 0.0f, float ypos = 0.0f, float zpos = 0.0f,
        float angle = 0, float xrot = 0, float yrot = 0, float zrot = 0,
        float xscl = 1.0f, float yscl = 1.0f, float zscl = 1.0f) : 
        xpos(xpos), ypos(ypos), zpos(zpos), 
        angle(angle), xrot(xrot), yrot(yrot), zrot(zrot), 
        xscl(xscl), yscl(yscl), zscl(zscl) {}

    float xpos, ypos, zpos;        // Position
    float angle, xrot, yrot, zrot; // Rotation
    float xscl, yscl, zscl;        // Scale
};