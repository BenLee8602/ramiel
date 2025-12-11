#include <ramiel/graphics.h>
#include "camera.h"
#include "menu.h"
#include "window.h"
#include "physics.h"
#include "graphics.h"
#include "terminal.h"
using namespace ramiel;

namespace {

    class CameraMenu : public Menu {
    public:
        static CameraMenu& get();
        virtual void render() const override;
        virtual void controls() const override;
    private:
        CameraMenu() = default;
    };

    CameraMenu& CameraMenu::get() {
        static CameraMenu menu;
        return menu;
    }

    void CameraMenu::render() const {
        Vec4ui8* menuBuf = getMenuBuf();
        std::fill(menuBuf, menuBuf + getBufferSize(), Vec4ui8{});
    }

    void CameraMenu::controls() const {
        constexpr float cameraSpeedNormal = 2.0f;
        constexpr float cameraSpeedFaster = 10.0f;
        constexpr float cameraRotationSpeed = 1.57079f;

        float dtime = getDtime();

        // reset pos and rot
        if (keyUp(Key::Q)) {
            setPos(Vec3f());
            setRot(Vec3f());
            renderNeeded();
            return;
        }

        // move faster
        float cameraSpeed = (
            keyPressed(Key::SHIFT) ?
            cameraSpeedFaster : cameraSpeedNormal
        );
        float dPos = cameraSpeed * dtime;
        float dRot = cameraRotationSpeed * dtime;

        Vec3f pos = getPos();
        Vec3f rot = getRot();

        // move left
        if (keyPressed(Key::A)) {
            pos[X] -= dPos * std::cos(rot[Y]);
            pos[Z] -= dPos * std::sin(rot[Y]);
            renderNeeded();
        }

        // move right
        if (keyPressed(Key::D)) {
            pos[X] += dPos * std::cos(rot[Y]);
            pos[Z] += dPos * std::sin(rot[Y]);
            renderNeeded();
        }

        // move down
        if (keyPressed(Key::CONTROL)) {
            pos[Y] -= dPos;
            renderNeeded();
        }

        // move up
        if (keyPressed(Key::SPACE)) {
            pos[Y] += dPos;
            renderNeeded();
        }

        // move backward
        if (keyPressed(Key::S)) {
            pos[X] += dPos * std::sin( rot[Y]);
            pos[Y] += dPos * std::sin(-rot[X]);
            pos[Z] -= dPos * std::cos( rot[Y]);
            renderNeeded();
        }

        // move forward
        if (keyPressed(Key::W)) {
            pos[X] -= dPos * std::sin( rot[Y]);
            pos[Y] -= dPos * std::sin(-rot[X]);
            pos[Z] += dPos * std::cos( rot[Y]);
            renderNeeded();
        }

        // turn down
        if (keyPressed(Key::DOWN)) {
            rot[X] = std::max(rot[X] - dRot, -1.57079f);
            renderNeeded();
        }

        // turn up
        if (keyPressed(Key::UP)) {
            rot[X] = std::min(rot[X] + dRot, 1.57079f);
            renderNeeded();
        }

        // turn right
        if (keyPressed(Key::RIGHT)) {
            rot[Y] -= dRot;
            renderNeeded();
        }

        // turn left
        if (keyPressed(Key::LEFT)) {
            rot[Y] += dRot;
            renderNeeded();
        }

        setPos(pos);
        setRot(rot);

        if (keyUp(Key::TICK)) {
            termOpen();
        }
    }

}

namespace ramiel {

    Menu* getCameraMenu() {
        return &CameraMenu::get();
    }

}

