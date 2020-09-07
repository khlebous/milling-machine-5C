#pragma once

namespace fe
{
	class IMesh
	{
	public:
		virtual void Init(void* vertices, int vertStride, int vertSize, std::vector<DWORD>& indices) = 0;

		virtual int VerticesCount() = 0;
		virtual int IndicesCount() = 0; 
		virtual int TrianglesCount() = 0;
		virtual void Draw() = 0;
	};
}
