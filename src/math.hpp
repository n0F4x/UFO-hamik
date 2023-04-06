namespace hyp {
    vec2 projectToDisk(const vec3& point) {
        return vec2{ point.x / (point.z + 1), point.y / (point.z + 1) };
    }

    struct Circle {
        vec3 center;
        float radius;
        unsigned pixelCount{ 32 };
        vec4 color;
        std::vector<vec2> vertices;
        GLuint vbo;

        void init() {
            glGenBuffers(1, &vbo);
        }

        void updateGeometry() {
            vertices.resize(pixelCount);
            vertices[0] = projectToDisk(center);
            for (uint32_t i{ 0 }; i < pixelCount-2; i++) {
                auto phi = static_cast<float>(i) / static_cast<float>(pixelCount-2) * 2 * M_PI;
                vertices[i + 1] = projectToDisk(posBySpeedTime(
                    center,
                    rotateBy(center,
                             hnormalize(fitSpeed(center, vec3{ 0, -1, 0 })),
                             static_cast<float>(phi)),
                    radius));
            }
            vertices[pixelCount - 1] = vertices[1];
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER,
                         static_cast<GLsizeiptr>(vertices.size() * sizeof(vec2)),
                         vertices.data(),
                         GL_DYNAMIC_DRAW);
        }
    };
}
