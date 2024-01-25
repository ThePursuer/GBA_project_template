#ifndef GBA_SCENE_H
#define GBA_SCENE_H

#include <memory>
#include <optional>

#include "gba_os/Graphics/Types.h"

namespace gba_os::graphics {

class Scene {
public:
    Scene();
    ~Scene();

    void update(); // Render the meshes into buffer_

    void register_entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat, std::shared_ptr<Vector3> pos, int id);
    void deregister_entity(int id);
    void register_camera(std::shared_ptr<Camera> cam);

    const void* get_buffer_ptr() const { return static_cast<const void*>(buffer_.get()); }

private:
    struct Entity {
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> mat;
        std::shared_ptr<Vector3> pos;
        int id;
    };

    void renderEntity(Entity& entity) {
        // Transform vertices according to entity's position
        std::vector<Vector3> transformedVertices;
        for (const auto& v : entity.mesh->vertices) {
            // Simple translation transformation for demonstration
            transformedVertices.push_back(translateVertex(v, *entity.pos));
        }

        // Project vertices onto 2D screen space
        std::vector<Vector2> projectedVertices;
        for (const auto& v : transformedVertices) {
            auto projected = projectVertex(v, *cam_);
            if (projected) {
                projectedVertices.push_back(*projected);
            }
        }

        // Rasterize faces onto the buffer
        for (const auto& face : entity.mesh->faces) {
            drawFace(face, projectedVertices);
        }
    }

    Vector3 translateVertex(const Vector3& vertex, const Vector3& translation) {
        // Simple translation
        return Vector3{vertex.x + translation.x, vertex.y + translation.y, vertex.z + translation.z};
    }

    std::optional<Vector2> projectVertex(const Vector3& vertex, const Camera& cam) {
        // Perspective projection (simplified)
        // This should be replaced with a proper projection based on the camera's FOV and other parameters
        const float zNear = 0.1f;
        if (vertex.z > zNear) {
            float x = vertex.x / vertex.z;
            float y = vertex.y / vertex.z;
            return Vector2{x * 120 + 120, y * 80 + 80}; // Adjusting for screen dimensions
        }
        return std::nullopt;
    }

    void drawFace(const Face& face, const std::vector<Vector2>& vertices) {
        // Draw the face on the buffer
        // This is a placeholder function; in a real implementation, you would draw the triangle formed by the face's vertices
        // For simplicity, we're just marking the vertices
        for (int i = 0; i < 3; ++i) {
            auto v = vertices[face.v[i]];
            if (v.x >= 0 && v.x < 240 && v.y >= 0 && v.y < 160) {
                (*buffer_)[static_cast<int>(v.y)][static_cast<int>(v.x)] = 255; // White color for demonstration
            }
        }
    }

    std::shared_ptr<Camera> cam_;
    std::vector<Entity> entities_;
    std::vector<int> active_entities_; // List of indices of entities that are active after culling and clipping
    std::unique_ptr<uint8_t[160][240]> buffer_;
};

}

#endif // GBA_SCENE_H