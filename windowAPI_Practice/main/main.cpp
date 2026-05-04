#include"../Manager/windowManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	
	windowManager manager;
	if (!manager.initalize(hInstance)) {
		return -1;
	}
	if (!manager.create_window(ID::class_id::main)) {
		return -1;
	}
	if (!manager.create_window(ID::class_id::sub)) {
		return -1;
	}
	while (manager.message_loop())
	{

	}

	return 0;
}