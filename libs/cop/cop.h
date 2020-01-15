#ifndef __LIBS__COP_H
#define DllImport   __declspec( dllimport )

#pragma comment(lib, "cop32.lib")
namespace LEGO {
	typedef void COPScene;
}
#include <memory>


/*
DllImport unsigned int ConvertModel(char const*, char const*);
DllImport unsigned int ConvertModel(char const*, unsigned int, unsigned int&, char const*&, unsigned int const*&);
DllImport unsigned int ConvertModel(char const*, unsigned int, std::uintptr_t* copScene, char const*&, unsigned int&, float, bool, bool, unsigned int, unsigned int, bool);
DllImport std::uintptr_t* CreateCOPScene(void);
DllImport void DestroyCOPScene(std::uintptr_t* copScene);
DllImport void SetHKXCollisionIds(unsigned int, unsigned int, unsigned int, unsigned int);
DllImport unsigned int SetupCOP(char const*, bool, char const*, unsigned int);*/


#endif // !__LIBS__COP_H
