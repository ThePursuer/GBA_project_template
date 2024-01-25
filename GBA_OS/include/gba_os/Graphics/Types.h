#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

// Include the fixed point math library
#include <libfixmath/fix16.h>
#include <vector>
#include <sstream>
#include <string>
#include <vector>

namespace gba_os::graphics {

struct Vector2 {
    float x, y;
};

struct Vector3 {
    fix16_t x, y, z;
};

struct Face {
    unsigned int v[3];
    unsigned int vn[3];
};

struct Mesh {
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Face> faces;
};

struct RGB {
    uint8_t r, g, b;
};

struct Material {
    fix16_t Ns;             // Specular exponent
    RGB originalColor;      // Original Kd color from the file
    uint16_t colorCode;     // Matched color in RGB555 format (index in BG_PALETTE)
    fix16_t Ni;             // Optical density (index of refraction)
    fix16_t d;              // Dissolve (transparency)
    int illum;              // Illumination model
};

// All cameras assume the same FOV
struct Camera {
    Vector3 pos;
    Vector3 rotation;
};

} // gba_os::graphics

#endif // GRAPHICS_MESH_H