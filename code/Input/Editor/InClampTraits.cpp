/*
 * TRAKTOR
 * Copyright (c) 2022 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Core/Misc/String.h"
#include "Input/Binding/InClamp.h"
#include "Input/Editor/InClampTraits.h"

namespace traktor::input
{

std::wstring InClampTraits::getHeader(const IInputNode* node) const
{
	return L"Clamp";
}

std::wstring InClampTraits::getDescription(const IInputNode* node) const
{
	const InClamp* inClamp = checked_type_cast< const InClamp*, false >(node);
	return toString(inClamp->m_limit[0]) + L" -> " + toString(inClamp->m_limit[1]);
}

Ref< IInputNode > InClampTraits::createNode() const
{
	return new InClamp();
}

void InClampTraits::getInputNodes(const IInputNode* node, std::map< const std::wstring, Ref< const IInputNode > >& outInputNodes) const
{
	const InClamp* inClamp = checked_type_cast< const InClamp*, false >(node);
	outInputNodes[L"Input"] = inClamp->m_source;
}

void InClampTraits::connectInputNode(IInputNode* node, const std::wstring& inputName, IInputNode* sourceNode) const
{
	InClamp* inClamp = checked_type_cast< InClamp*, false >(node);
	inClamp->m_source = sourceNode;
}

void InClampTraits::disconnectInputNode(IInputNode* node, const std::wstring& inputName) const
{
	InClamp* inClamp = checked_type_cast< InClamp*, false >(node);
	inClamp->m_source = nullptr;
}

}
