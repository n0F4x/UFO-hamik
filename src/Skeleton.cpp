//=============================================================================================
// Mintaprogram: Zöld háromszög. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII
// karaktereket tartalmazhat, BOM kihuzando. Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni
// es
// - felesleges programsorokat a beadott programban hagyni!!!!!!!
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest
// kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual
// Studio-hoz kepesti elteresekrol es a leggyakoribb hibakrol (pl. ideiglenes
// objektumot nem lehet referencia tipusnak ertekul adni) a hazibeado portal ad
// egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az
// oran a feladatkiadasig elhangzottak A keretben nem szereplo GLUT fuggvenyek
// tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Galgóczy Gábor
// Neptun : QS2WJR
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen
// segitseget igenybe vettem vagy mas szellemi termeket felhasznaltam, akkor a
// forrast es az atvett reszt kommentekben egyertelmuen jeloltem. A
// forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi,
// illetve a grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban,
// irasban, Interneten, stb.) erkezo minden egyeb informaciora (keplet, program,
// algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is
// ertem, azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok
// azzal, hogy az atvett reszek nem szamitanak a sajat kontribucioba, igy a
// feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik
// dontes. Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese
// eseten a hazifeladatra adhato pontokat negativ elojellel szamoljak el es
// ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include "framework.h"

float hdot(const vec3& p1, const vec3& p2) {
    return p1.x * p2.x + p1.y * p2.y - p1.z * p2.z;
}

float hlength(const vec3& v) {
    return sqrtf(hdot(v, v));
}

vec3 hnormalize(const vec3& v) {
    return v * (1 / hlength(v));
}

namespace hyp {

// Task 1
vec3 perpendicularRight(const vec3& from, const vec3& direction) {
    return hnormalize(
        cross(vec3{ from.x, from.y, -1 * from.z },
              vec3{ direction.x, direction.y, -1 * direction.z }));
}

// Task 2
vec3 posBySpeedTime(const vec3& from, const vec3& speed, float time) {
    auto dir = hnormalize(speed);
    auto unitSpeed = hlength(speed);
    return from * coshf(time * unitSpeed)
         + dir * sinhf(time * unitSpeed);
}

// Task 2
vec3 speedBySpeedTime(const vec3& from, const vec3& speed, float time) {
    auto dir = hnormalize(speed);
    auto unitSpeed = hlength(speed);
    return unitSpeed
         * (from * sinhf(time * unitSpeed)
            + dir * coshf(time * unitSpeed));
}

// Task 3
float distance(const vec3& point1, const vec3& point2) {
    return acoshf(-hdot(point1, point2));
}

// Task 3
vec3 direction(const vec3& from, const vec3& to) {
    return hnormalize(-1 * (from * coshf(distance(from, to)) - to)
                      / sinhf(distance(from, to)));
}

// Task 4
vec3 posByDirDist(const vec3& from, const vec3& direction, float distance) {
    return posBySpeedTime(from, hnormalize(direction), distance);
}

// Task 5
/**
 * @param angle IN RADIANS!!!
 */
vec3 rotateBy(const vec3& point, const vec3& speed, float angle) {
    return speed * cosf(angle)
         + perpendicularRight(point, speed) * sinf(angle);
}

// Task 6
vec3 fit(const vec3& p) {
    return vec3{ p.x, p.y, sqrtf(p.x * p.x + p.y * p.y + 1) };
}

// Task 6
vec3 fitSpeed(const vec3& point, const vec3& speed) {
    return speed + point * hdot(speed, point);
}

vec2 projectToDisk(const vec3& point) {
    return vec2{ point.x / (point.z + 1), point.y / (point.z + 1) };
}

struct Circle {
    vec3 center;
    float radius{};
    unsigned pixelCount{ 32 };
    vec4 color;
    std::vector<vec2> vertices;
    GLuint vbo{};

    void init() {
        glGenBuffers(1, &vbo);
    }

    void updateGeometry() {
        vertices.resize(pixelCount);
        vertices[0] = projectToDisk(center);
        for (uint32_t i{ 0 }; i < pixelCount - 2; i++) {
            auto phi = static_cast<float>(i)
                     / static_cast<float>(pixelCount - 2) * 2 * M_PI;
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

}   // namespace hyp

const char* const g_vertexSource = R"(
#version 330
precision highp float;

layout(location = 0) in vec2 pos;

void main() {
    gl_Position = vec4(pos, 0, 1);
}
)";

const char* const g_fragmentSource = R"(
#version 330
precision highp float;

uniform vec4 color;
out vec4 outColor;

void main() {
    outColor = color;
}
)";


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
                cosf(static_cast<float>(static_cast<float>(i) / 180.f * M_PI)));
            vertices[i / 2].y = static_cast<float>(
                sinf(static_cast<float>(static_cast<float>(i) / 180.f * M_PI)));
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
            2 * M_PI - static_cast<float>(atan2f(tmp.x, -tmp.y) + M_PI);
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

class PhysicsSystem {
public:
    void startMoving() {
        m_moving = true;
    }

    void stopMoving() {
        m_moving = false;
    }

    void startRightTurn() {
        m_turningRight = true;
    }

    void endRightTurn() {
        m_turningRight = false;
    }

    void startLeftTurn() {
        m_turningLeft = true;
    }

    void endLeftTurn() {
        m_turningLeft = false;
    }

    void advance(Scene& t_scene, u_long dt) {
        m_time += dt;

        if (m_turningRight) {
            t_scene.redUFO.speed = hyp::rotateBy(t_scene.redUFO.position,
                                                 t_scene.redUFO.speed,
                                                 M_PI / 2 / 1000 * dt);
        }
        if (m_turningLeft) {
            t_scene.redUFO.speed =
                hyp::rotateBy(t_scene.redUFO.position,
                              t_scene.redUFO.speed,
                              static_cast<float>(-M_PI / 2 / 1000 * dt));
        }
        if (m_moving) {
            auto position =
                hyp::posBySpeedTime(t_scene.redUFO.position,
                                    t_scene.redUFO.speed,
                                    static_cast<float>(dt) / 1000.f);
            auto speed = hyp::speedBySpeedTime(t_scene.redUFO.position,
                                               t_scene.redUFO.speed,
                                               static_cast<float>(dt) / 1000.f);
            t_scene.redUFO.position = hyp::fit(position);
            t_scene.redUFO.speed =
                hyp::fitSpeed(t_scene.redUFO.position, speed);

            t_scene.redSaliva.vertices.push_back(
                hyp::projectToDisk(t_scene.redUFO.position));
            t_scene.redSaliva.updateGeometry();
        }

        auto origin = hyp::rotateBy(t_scene.greenSaliva.mid,
                                    hyp::direction(t_scene.greenSaliva.mid,
                                                   t_scene.greenUFO.position),
                                    static_cast<float>(dt) / 1000.f);
        t_scene.greenUFO.position = hyp::posByDirDist(
            t_scene.greenSaliva.mid, origin, t_scene.greenSaliva.radius);

        t_scene.greenUFO.speed =
            hyp::perpendicularRight(t_scene.greenSaliva.mid, origin);

        t_scene.greenSaliva.ufoPos = t_scene.greenUFO.position;
        t_scene.greenSaliva.updateGeometry();

        if (hyp::distance(t_scene.redUFO.position, t_scene.greenUFO.position)
            < 1.5)
        {
            t_scene.redUFO.mouthOpenness -= 1.f / static_cast<float>(dt) / 500;
            t_scene.greenUFO.mouthOpenness -=
                1.f / static_cast<float>(dt) / 500;
            if (t_scene.redUFO.mouthOpenness < -1.0) {
                t_scene.redUFO.mouthOpenness *= -1;
            }
            if (t_scene.greenUFO.mouthOpenness < -1.0) {
                t_scene.greenUFO.mouthOpenness *= -1;
            }
        }

        t_scene.redUFO.otherUfoPos = t_scene.greenUFO.position;
        t_scene.greenUFO.otherUfoPos = t_scene.redUFO.position;
        t_scene.redUFO.updateGeometry();
        t_scene.greenUFO.updateGeometry();
    }

private:
    bool m_turningRight = false;
    bool m_turningLeft = false;
    bool m_moving = false;
    u_long m_time{};
};

class RenderSystem {
public:
    void render(const Scene& t_scene) {
        glClear(GL_COLOR_BUFFER_BIT);

        draw(t_scene.disk);
        draw(t_scene.greenSaliva);
        draw(t_scene.redSaliva);
        draw(t_scene.greenUFO);
        draw(t_scene.redUFO);
    }

    void init(int t_windowWidth, int t_windowHeight) {
        glViewport(0, 0, t_windowWidth, t_windowHeight);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        m_gpuProgram.create(g_vertexSource, g_fragmentSource, "outColor");

        glClearColor(27.f / 255.f, 27.f / 255.f, 27.f / 255.f, 1);
    }

private:
    void draw(const Disk& t_disk) {
        m_gpuProgram.setUniform(t_disk.color, "color");

        glBindBuffer(GL_ARRAY_BUFFER, t_disk.vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(
            GL_TRIANGLE_FAN, 0, static_cast<GLint>(t_disk.vertices.size()));
    }

    void draw(const UFO& t_ufo) {
        draw(t_ufo.body);

        draw(t_ufo.mouth);

        draw(t_ufo.eyeLid1);
        draw(t_ufo.eyePupil1);

        draw(t_ufo.eyeLid2);
        draw(t_ufo.eyePupil2);
    }

    void draw(const Saliva& t_saliva) {
        m_gpuProgram.setUniform(t_saliva.color, "color");

        glBindBuffer(GL_ARRAY_BUFFER, t_saliva.vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(
            GL_POINTS, 0, static_cast<GLint>(t_saliva.vertices.size()));
    }

    void draw(const SalivaCircle& t_salivaCircle) {
        m_gpuProgram.setUniform(t_salivaCircle.color, "color");

        glBindBuffer(GL_ARRAY_BUFFER, t_salivaCircle.vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_LINE_STRIP,
                     0,
                     static_cast<GLint>(t_salivaCircle.vertices.size()));
    }

    void draw(const hyp::Circle& circle) {
        m_gpuProgram.setUniform(circle.color, "color");
        glBindBuffer(GL_ARRAY_BUFFER, circle.vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(
            GL_TRIANGLE_FAN, 0, static_cast<GLint>(circle.vertices.size()));
    }

    ///-------------///
    ///  Variables  ///
    ///-------------///
    unsigned int m_vao{};

    GPUProgram m_gpuProgram;
};


namespace engine {

class Engine {
public:
    void init(int t_windowWidth, int t_windowHeight) {
        m_renderer.init(t_windowWidth, t_windowHeight);
        m_scene.init();
    }

    void on_display() {
        m_renderer.render(m_scene);

        glutSwapBuffers();
    }

    void on_key_down(unsigned char t_key, vec2) {
        switch (t_key) {
            case 'd': glutPostRedisplay(); break;
            case 'e': m_physics.startMoving(); break;
            case 's': m_physics.startRightTurn(); break;
            case 'f': m_physics.startLeftTurn(); break;
            default: break;
        }
    }

    void on_key_up(unsigned char t_key, vec2) {
        switch (t_key) {
            case 'e': m_physics.stopMoving(); break;
            case 's': m_physics.endRightTurn(); break;
            case 'f': m_physics.endLeftTurn(); break;
            default: break;
        }
    }

    void on_mouse_motion(vec2) {}

    void on_mouse_click(int, int, vec2) {}

    void on_idle() {
        u_long now = glutGet(GLUT_ELAPSED_TIME);
        u_long dt = now - m_lastTime;
        m_lastTime = now;
        while (dt != 0) {
            if (dt >= 10) {
                m_physics.advance(m_scene, 10);
                dt -= 10;
            } else {
                m_physics.advance(m_scene, dt);
                dt = 0;
            }
        }
        glutPostRedisplay();
    }

private:
    ///-------------///
    ///  Variables  ///
    ///-------------///
    u_long m_lastTime{};

    Scene m_scene;
    PhysicsSystem m_physics{};
    RenderSystem m_renderer{};
};

}   // namespace engine

engine::Engine g_engine{};

void onInitialization() {
    g_engine.init(windowWidth, windowHeight);
}

void onDisplay() {
    g_engine.on_display();
}

void onKeyboard(unsigned char key, int pX, int pY) {
    g_engine.on_key_down(
        key, vec2{ static_cast<float>(pX), static_cast<float>(pY) });
}

void onKeyboardUp(unsigned char key, int pX, int pY) {
    g_engine.on_key_up(key,
                       vec2{ static_cast<float>(pX), static_cast<float>(pY) });
}

void onMouseMotion(int pX, int pY) {
    g_engine.on_mouse_motion(
        vec2{ static_cast<float>(pX), static_cast<float>(pY) });
}

void onMouse(int button, int state, int pX, int pY) {
    g_engine.on_mouse_click(
        button, state, vec2{ static_cast<float>(pX), static_cast<float>(pY) });
}

void onIdle() {
    g_engine.on_idle();
}
