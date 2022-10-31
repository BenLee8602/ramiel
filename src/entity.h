#pragma once

#include "physicsobj.h"
#include "mesh.h"
#include "vertexshader.h"
#include "pixelshader.h"

namespace ramiel {

	class Entity {
		MeshBase* mesh;
		VertexShader* vertexShader;
		PixelShader* pixelShader;
	public:
		Entity(
			MeshBase* mesh,
			float scale,
			PhysicsObj* physicsObj,
			VertexShader* vertexShader,
			PixelShader* pixelShader
		);
		void pipeline() const;
	};

}
