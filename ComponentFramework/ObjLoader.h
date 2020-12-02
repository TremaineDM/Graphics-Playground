#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#include "Vector.h"

using namespace MATH;

	class ObjLoader {
	public:

		/// C11 precautions 
		ObjLoader(const ObjLoader&) = delete;  /// Copy constructor
		ObjLoader(ObjLoader&&) = delete;       /// Move constructor
		ObjLoader& operator=(const ObjLoader&) = delete; /// Copy operator
		ObjLoader& operator=(ObjLoader&&) = delete;      /// Move operator

		static std::vector<Vec3> vertices;
		static std::vector<Vec2> uvCoords;
		static std::vector<Vec3> normals;
		static bool loadOBJ(const char* path);
		static void ClearVectors();

	};

#endif