/*
 * TRAKTOR
 * Copyright (c) 2023-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Model/ModelRasterizer.h"

#include "Core/Math/Triangle.h"
#include "Drawing/Image.h"
#include "Model/Model.h"

#include <functional>

namespace traktor::model
{
namespace
{

const Vector4 c_sunDirection = Vector4(0.0f, -0.5f, 1.0f).normalized();

int32_t wrap(int32_t v, int32_t l)
{
	const int32_t c = v % l;
	return (c < 0) ? c + l : c;
}

Color4f lighting(const Vector4& p, const Vector4& n, const Color4f& materialColor, const Scalar& metalness, const Scalar& specularTerm)
{
	const Vector4 viewDirection = p.xyz0().normalized();
	const Vector4 halfWay = (c_sunDirection + viewDirection).normalized();
	const Scalar diffuse = clamp(dot3(c_sunDirection, -n), 0.4_simd, 1.0_simd) + 0.1_simd;
	const Scalar specular = power(abs(dot3(halfWay, -n)), 8.0_simd);
	return materialColor * diffuse * (1.0_simd - metalness) + lerp(Color4f(1.0f, 1.0f, 1.0f, 0.0f) * specularTerm, materialColor, metalness) * specular;
}

}

T_IMPLEMENT_RTTI_CLASS(L"traktor.model.ModelRasterizer", ModelRasterizer, Object)

bool ModelRasterizer::generate(const Model* model, const Matrix44& modelView, drawing::Image* outImage) const
{
	const float hw = (float)(outImage->getWidth() / 2.0f);
	const float hh = (float)(outImage->getHeight() / 2.0f);
	const float r = (float)outImage->getWidth() / outImage->getHeight();

	const Aabb3 boundingBox = model->getBoundingBox();
	const Scalar maxExtent = (boundingBox.getExtent() * Vector4(1.0f, 1.0f, 0.0f, 0.0f)).max();
	const Scalar invMaxExtent = 1.0_simd / maxExtent;

	const Matrix44 projection = perspectiveLh(deg2rad(70.0f), r, 0.1f, 100.0f);

	const auto& materials = model->getMaterials();
	const auto& polygons = model->getPolygons();
	const auto& vertices = model->getVertices();
	const auto& positions = model->getPositions();
	const auto& normals = model->getNormals();
	const auto& texCoords = model->getTexCoords();

	AlignedVector< float > zbuffer(outImage->getWidth() * outImage->getHeight(), 1.0f);
	AlignedVector< Vector4 > positionsClip(positions.size());
	AlignedVector< Vector4 > normalsView(normals.size());

	(projection * modelView).transform(positions.c_ptr(), positionsClip.ptr(), positions.size());
	modelView.transform(normals.c_ptr(), normalsView.ptr(), normals.size());

	Vector4 cp[3];
	Vector4 nm[3];
	Vector2 sp[3];
	Vector2 uv[3];

	for (const auto& polygon : polygons)
	{
		if (polygon.getMaterial() == model::c_InvalidIndex)
			continue;

		const auto& polygonVertices = polygon.getVertices();
		if (polygonVertices.size() != 3)
			continue;

		for (size_t i = 0; i < 3; ++i)
		{
			if (polygonVertices[i] >= vertices.size())
				return false;

			const model::Vertex& vertex = vertices[polygonVertices[i]];
			if (vertex.getPosition() == model::c_InvalidIndex || vertex.getNormal() == model::c_InvalidIndex)
				return false;

			const Vector4& position = positionsClip[vertex.getPosition()];
			const Vector4& normal = normalsView[vertex.getNormal()];

			cp[i] = position / position.w();
			nm[i] = normal.normalized();
			sp[i] = Vector2(
				cp[i].x() * hw + hw,
				hh - (cp[i].y() * hh));

			if (vertex.getTexCoord(0) != model::c_InvalidIndex)
				uv[i] = texCoords[vertex.getTexCoord(0)];
			else
				uv[i].set(0.0f, 0.0f);
		}

		// Discard too large triangles, to prevent broken meshes to halt editor.
		const float bw = std::max< float >({ sp[0].x, sp[1].x, sp[2].x }) - std::min< float >({ sp[0].x, sp[1].x, sp[2].x });
		const float bh = std::max< float >({ sp[0].y, sp[1].y, sp[2].y }) - std::min< float >({ sp[0].y, sp[1].y, sp[2].y });
		if (bw > 2.0f * hw || bh > 2.0f * hh)
			continue;

		const auto& polygonMaterial = materials[polygon.getMaterial()];
		const Scalar metalness = Scalar(polygonMaterial.getMetalness());
		const Scalar specularTerm = Scalar(polygonMaterial.getSpecularTerm());

		const drawing::Image* texture = polygonMaterial.getDiffuseMap().image;
		if (texture != nullptr)
		{
			const int32_t tw = texture->getWidth();
			const int32_t th = texture->getHeight();

			const bool textureLinear = (texture->getImageInfo() != nullptr) ? (std::abs(texture->getImageInfo()->getGamma() - 1.0f) < 0.1f) : false;

			triangle(sp[0], sp[1], sp[2], [&, tw, th](int32_t x, int32_t y, float alpha, float beta, float gamma) {
				if (x < 0 || x >= outImage->getWidth() || y < 0 || y >= outImage->getHeight())
					return;

				const int32_t offset = x + y * outImage->getWidth();
				const Scalar salpha(alpha);
				const Scalar sbeta(beta);
				const Scalar sgamma(gamma);

				const Vector4 p = cp[0] * salpha + cp[1] * sbeta + cp[2] * sgamma;
				if (p.z() < zbuffer[offset])
				{
					Color4f color;
					const Vector2 tc = uv[0] * alpha + uv[1] * beta + uv[2] * gamma;
					const int32_t tu = wrap((int32_t)(tc.x * tw), tw);
					const int32_t tv = wrap((int32_t)(tc.y * th), th);
					texture->getPixel(tu, tv, color);

					if (!textureLinear)
						color = color.linear();

					const Vector4 n = (nm[0] * salpha + nm[1] * sbeta + nm[2] * sgamma).normalized();
					const Color4f d = lighting(p, n, color, metalness, specularTerm);

					outImage->setPixelUnsafe(x, y, d.sRGB().rgb1());
					zbuffer[offset] = p.z();
				}
			});
		}
		else
		{
			triangle(sp[0], sp[1], sp[2], [&](int32_t x, int32_t y, float alpha, float beta, float gamma) {
				if (x < 0 || x >= outImage->getWidth() || y < 0 || y >= outImage->getHeight())
					return;

				const int32_t offset = x + y * outImage->getWidth();
				const Scalar salpha(alpha);
				const Scalar sbeta(beta);
				const Scalar sgamma(gamma);

				const Vector4 p = cp[0] * salpha + cp[1] * sbeta + cp[2] * sgamma;
				if (p.z() < zbuffer[offset])
				{
					const Color4f color = polygonMaterial.getColor().linear();

					const Vector4 n = (nm[0] * salpha + nm[1] * sbeta + nm[2] * sgamma).normalized();
					const Color4f d = lighting(p, n, color, metalness, specularTerm);

					outImage->setPixelUnsafe(x, y, d.sRGB().rgb1());
					zbuffer[offset] = p.z();
				}
			});
		}
	}

	return true;
}

}
