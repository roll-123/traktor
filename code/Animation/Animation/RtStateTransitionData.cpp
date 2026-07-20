/*
 * TRAKTOR
 * Copyright (c) 2025 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Animation/Animation/RtStateTransitionData.h"

#include "Animation/Animation/RtStateGraph.h"
#include "Animation/Animation/RtStateTransition.h"
#include "Core/Log/Log.h"
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/Member.h"
#include "Core/Serialization/MemberAlignedVector.h"
#include "Core/Serialization/MemberComposite.h"
#include "Core/Serialization/MemberEnum.h"
#include "Core/Serialization/MemberRef.h"

namespace traktor::animation
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.animation.RtStateTransitionData", 0, RtStateTransitionData, ISerializable)

Ref< RtStateTransition > RtStateTransitionData::createInstance(RtStateGraph* stateGraph) const
{
	// A transition referencing a state outside the graph means the resource is
	// corrupt or from a mismatched compiler; fail creation instead of reading
	// out of bounds (a stray null here crashes evaluation much later).
	const int32_t stateCount = (int32_t)stateGraph->m_states.size();
	if (m_from < 0 || m_from >= stateCount || m_to < 0 || m_to >= stateCount)
	{
		log::error << L"RtStateTransitionData: state index out of range (from " << m_from << L", to " << m_to << L", " << stateCount << L" state(s))." << Endl;
		return nullptr;
	}

	Ref< RtStateTransition > instance = new RtStateTransition();

	instance->m_from = stateGraph->m_states[m_from];
	instance->m_to = stateGraph->m_states[m_to];
	instance->m_moment = m_moment;
	instance->m_duration = m_duration;

	for (const auto& condition : m_conditions)
		instance->m_conditions.push_back({ condition.parameter, condition.inverted });

	return instance;
}

void RtStateTransitionData::serialize(ISerializer& s)
{
	s >> Member< int32_t >(L"from", m_from);
	s >> Member< int32_t >(L"to", m_to);
	s >> MemberEnumByValue< Moment >(L"moment", m_moment);
	s >> Member< float >(L"duration", m_duration);
	s >> MemberAlignedVector< Condition, MemberComposite< Condition > >(L"conditions", m_conditions);
}

void RtStateTransitionData::Condition::serialize(ISerializer& s)
{
	s >> Member< int32_t >(L"parameter", parameter);
	s >> Member< bool >(L"inverted", inverted);
}

}
