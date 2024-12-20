/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Serialization/ISerializer.h"
#include "Core/Serialization/MemberEnum.h"
#include "Core/Serialization/MemberRefArray.h"
#include "Input/Binding/CombinedInputSourceData.h"

namespace traktor::input
{

T_IMPLEMENT_RTTI_FACTORY_CLASS(L"traktor.input.CombinedInputSourceData", 0, CombinedInputSourceData, IInputSourceData)

CombinedInputSourceData::CombinedInputSourceData()
:	m_mode(CombinedInputSource::CmAny)
{
}

CombinedInputSourceData::CombinedInputSourceData(CombinedInputSource::CombineMode mode)
:	m_mode(mode)
{
}

CombinedInputSourceData::CombinedInputSourceData(const RefArray< IInputSourceData >& sources, CombinedInputSource::CombineMode mode)
:	m_sources(sources)
,	m_mode(mode)
{
}

void CombinedInputSourceData::addSource(IInputSourceData* source)
{
	m_sources.push_back(source);
}

const RefArray< IInputSourceData >& CombinedInputSourceData::getSources() const
{
	return m_sources;
}

Ref< IInputSource > CombinedInputSourceData::createInstance(DeviceControlManager* deviceControlManager) const
{
	RefArray< IInputSource > sources(m_sources.size());
	for (uint32_t i = 0; i < m_sources.size(); ++i)
	{
		Ref< IInputSource > source = m_sources[i]->createInstance(deviceControlManager);
		if (source)
			sources[i] = source;
		else
			return nullptr;
	}
	return new CombinedInputSource(sources, m_mode);
}

void CombinedInputSourceData::serialize(ISerializer& s)
{
	const MemberEnum< CombinedInputSource::CombineMode >::Key c_CombineMode_Keys[] =
	{
		{ L"CmAny", CombinedInputSource::CmAny },
		{ L"CmExclusive", CombinedInputSource::CmExclusive },
		{ L"CmAll", CombinedInputSource::CmAll },
		{ 0 }
	};

	s >> MemberRefArray< IInputSourceData >(L"sources", m_sources);
	s >> MemberEnum< CombinedInputSource::CombineMode >(L"mode", m_mode, c_CombineMode_Keys);
}

}
