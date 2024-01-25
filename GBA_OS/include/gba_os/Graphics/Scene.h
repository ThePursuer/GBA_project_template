#ifndef GBA_SCENE_H
#define GBA_SCENE_H

#include <memory>
#include <optional>
#include <vector>
#include <algorithm>
#include <libfixmath/fixmath.h>

#include "gba_os/Graphics/Types.h"

namespace gba_os::graphics {

class Scene {
public:
    Scene() {
    }

    ~Scene() {
    }

    void update() {
        for (const auto& entity : entities_) {
            renderEntity(entity);
        }
    }

    void register_entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, std::shared_ptr<Vector3> pos, int id) {
        entities_.push_back({mesh, mat, pos, id});
    }

    void deregister_entity(int id) {
        entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [id](const Entity& entity) {
            return entity.id == id;
        }), entities_.end());
    }

    void register_camera(std::shared_ptr<Camera> cam) {
        cam_ = cam;
    }

    const void* get_buffer_ptr() const { return static_cast<const void*>(&buffer_); }

private:
    struct Entity {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> mat;
        std::shared_ptr<Vector3> pos;
        int id;
    };

    void renderEntity(const Entity& entity) {
        // Transform vertices according to entity's position
        std::vector<Vector3> transformedVertices;
        transformedVertices.reserve(entity.mesh->vertices.size());
        for (const auto& v : entity.mesh->vertices) {
            // Simple translation transformation for demonstration
            transformedVertices.push_back(translateVertex(v, *entity.pos));
        }

        // Project vertices onto 2D screen space
        std::vector<Vector2> projectedVertices;
        projectedVertices.reserve(transformedVertices.size());
        for (const auto& v : transformedVertices) {
            auto projected = projectVertex(v, *cam_);
            if (projected) {
                projectedVertices.push_back(*projected);
            }
        }

        // Rasterize faces onto the buffer
        for (const auto& face : entity.mesh->faces) {
            drawFace(face, projectedVertices, transformedVertices, *entity.mat);
        }
    }

    Vector3 translateVertex(const Vector3& vertex, const Vector3& translation) {
        // Simple translation
        return Vector3{fix16_add(vertex.x, translation.x), fix16_add(vertex.y, translation.y), fix16_add(vertex.z, translation.z)};
    }

    std::optional<Vector2> projectVertex(const Vector3& vertex, const Camera& cam) {
        // Perspective projection (simplified)
        // This should be replaced with a proper projection based on the camera's FOV and other parameters
        const fix16_t zNear = fix16_from_float(0.1f);
        if (vertex.z > zNear) {
            fix16_t x = fix16_div(vertex.x, vertex.z);
            fix16_t y = fix16_div(vertex.y, vertex.z);
            return Vector2{fix16_add(fix16_mul(x, fix16_from_int(120)), 120), fix16_add(fix16_mul(y, fix16_from_int(80)), 80)}; // Adjusting for screen dimensions
        }
        return std::nullopt;
    }

    void drawFace(const Face& face, const std::vector<Vector2>& projectedVertices, const std::vector<Vector3>& originalVertices, const Material& mat) {
        // Draw the face on the buffer with depth testing
        for (int i = 0; i < 3; ++i) {
            auto v = projectedVertices[face.v[i]];
            if (v.x >= 0 && v.x < 240 && v.y >= 0 && v.y < 160) {
                fix16_t depth = calculateDepth(face, originalVertices); // Calculate depth for the current pixel
                if (depth < depthBuffer_[static_cast<int>(v.y)][static_cast<int>(v.x)]) { // Perform depth testing
                    buffer_[static_cast<int>(v.y)][static_cast<int>(v.x)] = mat.colorCode; // Update buffer
                    depthBuffer_[static_cast<int>(v.y)][static_cast<int>(v.x)] = depth; // Update depth buffer
                }
            }
        }
    }

    fix16_t calculateDepth(const Face& face, const std::vector<Vector3>& vertices) {
        // Calculate depth as the average z-coordinate of the face's vertices
        fix16_t depth = fix16_from_int(0);
        for (int i = 0; i < 3; ++i) {
            auto v = vertices[face.v[i]];
            depth = fix16_add(depth, v.z);
        }
        return fix16_div(depth, fix16_from_int(3));
    }

    std::shared_ptr<Camera> cam_;
    std::vector<Entity> entities_;
    uint8_t buffer_[160][240];
    fix16_t depthBuffer_[160][240];
};

} // namespace gba_os::graphics

#endif // GBA_SCENE_H