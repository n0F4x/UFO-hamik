struct Disk {
    vec4 color{ 0.f, 0.f, 0.f, 1.f };
    std::vector<vec2> vertices;
    GLuint vbo{};

    void init() {
        glGenBuffers(1, &vbo);
        vertices.resize(180);
    }

    void updateGeometry() {
        for (uint32_t i{ 0 }; i < 360; i += 2) {
            vertices[i / 2].x = static_cast<float>(
                std::cos(static_cast<float>(i) / 180.f * M_PI));
            vertices[i / 2].y = static_cast<float>(
                std::sin(static_cast<float>(i) / 180.f * M_PI));
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() * sizeof(vec2)),
                     vertices.data(),
                     GL_STATIC_DRAW);
    }
};

struct UFO {
    vec3 position;
    vec3 speed;
    float mouthOpenness{ 1.f };
    vec3 otherUfoPos;
    hyp::Circle body;
    hyp::Circle mouth;
    hyp::Circle eyeLid1;
    hyp::Circle eyePupil1;
    hyp::Circle eyeLid2;
    hyp::Circle eyePupil2;

    void init() {
        body.init();
        body.radius = 0.5f;

        mouth.init();
        mouth.pixelCount = 12;

        eyeLid1.init();
        eyeLid1.radius = 0.1f;
        eyeLid1.pixelCount = 12;
        eyePupil1.init();
        eyePupil1.radius = 0.05f;
        eyePupil1.pixelCount = 10;

        eyeLid2.init();
        eyeLid2.radius = 0.1f;
        eyeLid2.pixelCount = 12;
        eyePupil2.init();
        eyePupil2.radius = 0.05f;
        eyePupil2.pixelCount = 10;
    }

    void updateGeometry() {
        body.center = position;
        body.updateGeometry();

        mouth.center =
            hyp::posByDirDist(position, hnormalize(speed), body.radius);
        mouth.radius = 0.1f * mouthOpenness;
        mouth.updateGeometry();

        eyeLid1.center = hyp::posByDirDist(
            position,
            hyp::rotateBy(position, hnormalize(speed), M_PI / 6.f),
            body.radius);
        eyeLid1.updateGeometry();
        eyePupil1.center =
            hyp::posByDirDist(eyeLid1.center,
                              hyp::direction(eyeLid1.center, otherUfoPos),
                              eyeLid1.radius / 2.f);
        eyePupil1.updateGeometry();

        eyeLid2.center = hyp::posByDirDist(
            position,
            hyp::rotateBy(position, hnormalize(speed), -M_PI / 6.f),
            body.radius);
        eyeLid2.updateGeometry();
        eyePupil2.center =
            hyp::posByDirDist(eyeLid2.center,
                              hyp::direction(eyeLid2.center, otherUfoPos),
                              eyeLid2.radius / 2.f);
        eyePupil2.updateGeometry();
    }
};

struct Saliva {
    vec4 color{ 1, 1, 1, 1 };
    std::vector<vec2> vertices;
    GLuint vbo{};

    void init() {
        glGenBuffers(1, &vbo);
    }

    void updateGeometry() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() * sizeof(vec2)),
                     vertices.data(),
                     GL_DYNAMIC_DRAW);
    }
};

struct SalivaCircle {
    vec3 mid;
    float radius{ 0.5f };
    double done{ 0.f };
    vec3 ufoPos;
    vec4 color{ 1, 1, 1, 1 };
    std::vector<vec2> vertices;
    GLuint vbo{};

    void init() {
        glGenBuffers(1, &vbo);
    }

    void updateGeometry() {
        if (done >= 1.f) {
            return;
        }

        vertices.clear();
        using namespace hyp;
        auto tmp =
            hnormalize(fitSpeed(vec3{ 0, 0, 1 }, direction(mid, ufoPos)));
        auto maxPhi =
            2 * M_PI - static_cast<float>(std::atan2(tmp.x, -tmp.y) + M_PI);
        if (maxPhi / (2 * M_PI) < done) {
            done = 1.f;
            maxPhi = 2 * M_PI;
        } else {
            done = maxPhi / (2 * M_PI);
        }
        for (uint32_t i{ 0 }; i < 31; i++) {
            auto phi = static_cast<float>(i) / 30.f * 2 * M_PI;
            if (phi > maxPhi) {
                break;
            }
            vertices.push_back(projectToDisk(posBySpeedTime(
                mid,
                rotateBy(mid,
                         hnormalize(fitSpeed(mid, vec3{ 0, 1, 0 })),
                         static_cast<float>(phi)),
                radius)));
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() * sizeof(vec2)),
                     vertices.data(),
                     GL_STATIC_DRAW);
    }
};

struct Scene {
    void init() {
        // Poincare disk
        disk.init();
        disk.updateGeometry();

        // Red UFO
        redUFO.position = vec3{ 0, 0, 1 };
        redUFO.body.color = vec4{ 1, 0, 0, 1 };
        redUFO.speed = vec3{ 0, 1, 0 };
        redUFO.mouth.color = vec4{ 0.05f, 0.05f, 0.05f, 1 };
        redUFO.eyeLid1.color = vec4{ 1.f, 1.f, 0.7f, 1 };
        redUFO.eyePupil1.color = vec4{ 0.01f, 0.3f, 0.02f, 0.1f };
        redUFO.eyeLid2.color = vec4{ 0.7f, 0.7f, 1.f, 1 };
        redUFO.eyePupil2.color = vec4{ 0.01f, 0.3f, 0.02f, 0.1f };
        redUFO.init();

        // Red Saliva
        redSaliva.init();

        // Green UFO
        greenUFO.position =
            hyp::posBySpeedTime(vec3{ 0, 0, 1 }, vec3{ 0, -1, 0 }, 1);
        greenUFO.body.color = vec4{ 0, 1, 0, 1 };
        greenUFO.speed = hyp::fitSpeed(greenUFO.position, vec3{ 1, 0, 0 });
        greenUFO.mouth.color = vec4{ 0.05f, 0.05f, 0.0 };
        greenUFO.eyeLid1.color = vec4{ 1.f, 1.f, 0.7f, 1 };
        greenUFO.eyePupil1.color = vec4{ 0.01f, 0.3f, 0.02f, 0.1f };
        greenUFO.eyeLid2.color = vec4{ 0.7f, 0.7f, 1.f, 1 };
        greenUFO.eyePupil2.color = vec4{ 0.01f, 0.3f, 0.02f, 0.1f };
        greenUFO.init();

        // Green saliva
        greenSaliva.mid = hyp::posByDirDist(greenUFO.position,
                                            hyp::fitSpeed(greenUFO.position,
                                                          vec3{ 0, -1, 0 }),
                                            greenSaliva.radius);
        greenSaliva.init();

        redUFO.otherUfoPos = greenUFO.position;
        greenUFO.otherUfoPos = redUFO.position;
        redUFO.updateGeometry();
        greenUFO.updateGeometry();
    }

    Disk disk;
    UFO redUFO;
    UFO greenUFO;
    Saliva redSaliva;
    SalivaCircle greenSaliva;
};
