#pragma once

#include <algorithm>
#include "entitybase.h"
#include "mesh.h"
#include "triangle.h"

namespace ramiel {

	template<class Vertex, class VertexShader, class PixelShader>
	class Entity : public EntityBase {
		const Mesh<Vertex>& mesh;
		VertexShader vertexShader;
		PixelShader pixelShader;
	public:
		Entity(
			const Mesh<Vertex>& mesh,
			const VertexShader& vertexShader,
			const PixelShader& pixelShader
		) :
			mesh(mesh),
			vertexShader(vertexShader),
			pixelShader(pixelShader)
		{}


		virtual void draw(Camera& camera) const override {
			// run vertex shader
			const std::vector<Vertex>& v_in = mesh.getVertices();
			std::vector<typename VertexShader::Vertex_Out> v_out(v_in.size());
			std::transform(v_in.begin(), v_in.end(), v_out.begin(), vertexShader);

			const std::vector<Vec3u>& triangles = mesh.getTriangles();
			Triangle<typename VertexShader::Vertex_Out, PixelShader> t(camera, pixelShader);
			for (auto& tri : triangles) {
				// backface culling
				if (dotProduct(
					crossProduct(
						v_out[tri[1]].cameraPos - v_out[tri[0]].cameraPos,
						v_out[tri[2]].cameraPos - v_out[tri[0]].cameraPos
					),  v_out[tri[0]].cameraPos
				) >= 0.0f) continue;
				
				// assemble and draw triangle
				t.draw(v_out[tri[0]], v_out[tri[1]], v_out[tri[2]]);
			}
		}

	};

}
