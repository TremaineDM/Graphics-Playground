#include <stdlib.h>  
#include <crtdbg.h>
#include <string>
#include "SceneManager.h"
#include "Debug.h"
#include "ObjLoader.h"
#define _CRTDBG_MAP_ALLOC  
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
  
int main(int argc, char* args[]) {

	std::string name = { "Graphics Game Engine" };
	Debug::DebugInit(name + "_Log");
	Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);
	
	SceneManager* gsm = new SceneManager();
	if (gsm->Initialize(name, 1280, 720) ==  true) {
		gsm->Run();
	} 
	delete gsm;
	gsm = nullptr;
	name.empty();
	name.~basic_string();
	Debug::ClearDebug();
	ObjLoader::ClearVectors();
	_CrtDumpMemoryLeaks();
	exit(0);

}