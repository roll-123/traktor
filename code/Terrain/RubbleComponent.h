#pragma once

#include "Core/Containers/AlignedVector.h"
#include "Core/Math/Vector4.h"
#include "Mesh/Instance/InstanceMesh.h"
#include "Resource/Proxy.h"
#include "Terrain/TerrainLayerComponent.h"
#include "Terrain/RubbleComponentData.h"

namespace traktor
{
	namespace render
	{

class IRenderSystem;

	}

	namespace resource
	{

class IResourceManager;

	}

	namespace terrain
	{

class RubbleComponent : public TerrainLayerComponent
{
	T_RTTI_CLASS;

public:
	RubbleComponent();

	bool create(
		resource::IResourceManager* resourceManager,
		render::IRenderSystem* renderSystem,
		const RubbleComponentData& layerData
	);

	virtual void destroy() override final;

	virtual void setOwner(world::Entity* owner) override final;

	virtual void setTransform(const Transform& transform) override final;

	virtual Aabb3 getBoundingBox() const override final;

	virtual void update(const world::UpdateParams& update) override final;

	virtual void build(
		const world::WorldBuildContext& context,
		const world::WorldRenderView& worldRenderView,
		const world::IWorldRenderPass& worldRenderPass
	) override final;

	virtual void updatePatches() override final;

private:
	struct RubbleMesh
	{
		resource::Proxy< mesh::InstanceMesh > mesh;
		uint8_t material;
		int32_t density;
		float randomScaleAmount;
	};

	struct Instance
	{
		Vector4 position;
		Quaternion rotation;
		float scale;
	};

	struct Cluster
	{
		RubbleMesh* rubbleDef;
		Vector4 center;
		float distance;
		bool visible;
		int32_t from;
		int32_t to;
	};

	world::Entity* m_owner;
	AlignedVector< RubbleMesh > m_rubble;
	AlignedVector< Instance > m_instances;
	AlignedVector< Cluster > m_clusters;
	float m_spreadDistance;
	float m_clusterSize;
	Vector4 m_eye;
	AlignedVector< mesh::InstanceMesh::RenderInstance > m_instanceData;
};

	}
}
