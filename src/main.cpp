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

#include "functions.hpp"
#include "math.hpp"

#include "Engine.hpp"
using namespace engine;

Engine g_engine{};

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
