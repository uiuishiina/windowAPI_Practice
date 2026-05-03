#include"../App/windowManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    windowManager manager;
    if (!manager.initalize(hInstance)) {
        return -1;
    }
    if (!manager.create_window(windowclassID::Main)) {
        return -1;
    }
    if (!manager.create_window(windowclassID::Sub)) {
        return -1;
    }
    if (!manager.create_window(windowclassID::Sub)) {
        return -1;
    }
    while (manager.message_loop())
    {

    }

    return 0;
}