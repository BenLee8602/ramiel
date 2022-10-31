#include "entity.h"

namespace ramiel {
	
	Entity::Entity(
		MeshBase* mesh,
		VertexShader* vertexShader,
		PixelShader* pixelShader
	) {
		this->mesh = mesh;
		this->vertexShader = vertexShader;
		this->pixelShader = pixelShader;
	}


	void Entity::pipeline() const {
		mesh->pipeline(vertexShader, pixelShader);
	}

}
