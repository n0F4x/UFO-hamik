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
    unsigned int m_vao;

    GPUProgram m_gpuProgram;
};
