/*
================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION/NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright 2017 Doctor Entertainment AB. All Rights Reserved.
================================================================================================
*/
#ifndef traktor_model_LwsGroup_H
#define traktor_model_LwsGroup_H

#include "Model/Formats/Lws/LwsNode.h"
#include "Core/Ref.h"
#include "Core/RefArray.h"

namespace traktor
{
	namespace model
	{

/*! \brief
 * \ingroup Model
 */
class LwsGroup : public LwsNode
{
	T_RTTI_CLASS;

public:
	LwsGroup(const std::wstring& name);

	void add(LwsNode* node);

	uint32_t getCount() const;

	Ref< const LwsNode > get(uint32_t index) const;

	Ref< const LwsNode > find(const std::wstring& name, uint32_t from = 0) const;

private:
	RefArray< LwsNode > m_nodes;
};

	}
}

#endif	// traktor_model_LwsGroup_H
