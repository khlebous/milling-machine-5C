#pragma once

namespace fe
{
	class IGuiWindow
	{
	public:
		IGuiWindow(const char* _name, bool _open);
		virtual void Render() = 0;
		
		bool open;
		const char* name;
	};
}
