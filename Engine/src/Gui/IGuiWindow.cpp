#include "pch.h"
#include "IGuiWindow.h"

namespace fe
{
	IGuiWindow::IGuiWindow(const char* _name, bool _open)
		: 
		name(_name), 
		open(_open) 
	{ }
}
