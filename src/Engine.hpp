#include "shaders.hpp"

#include "Scene.hpp"
#include "Systems.hpp"

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
