#include "Core/Serialization/AttributeRange.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/Member.h"
#include "Core/Serialization/MemberAlignedVector.h"
#include "Core/Serialization/MemberComposite.h"
#include "Mesh/Instance/InstanceMesh.h"
#include "Resource/Member.h"
#include "Terrain/RubbleComponentData.h"

namespace traktor
{
	namespace terrain
	{

T_IMPLEMENT_RTTI_EDIT_CLASS(L"traktor.terrain.RubbleComponentData", 1, RubbleComponentData, TerrainLayerComponentData)

RubbleComponentData::RubbleComponentData()
:	m_spreadDistance(100.0f)
{
}

void RubbleComponentData::serialize(ISerializer& s)
{
	T_ASSERT(s.getVersion() >= 1);

	s >> Member< float >(L"spreadDistance", m_spreadDistance);
	s >> MemberAlignedVector< RubbleMesh, MemberComposite< RubbleMesh > >(L"rubble", m_rubble);
}

RubbleComponentData::RubbleMesh::RubbleMesh()
:	material(1)
,	density(10)
,	randomScaleAmount(0.5f)
{
}

void RubbleComponentData::RubbleMesh::serialize(ISerializer& s)
{
	s >> resource::Member< mesh::InstanceMesh >(L"mesh", mesh);
	s >> Member< uint8_t >(L"material", material);
	s >> Member< int32_t >(L"density", density, AttributeRange(0.0f));
	s >> Member< float >(L"randomScaleAmount", randomScaleAmount, AttributeRange(0.0f, 1.0f));
}

	}
}