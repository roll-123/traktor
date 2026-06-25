/*
================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION/NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright 2017 Doctor Entertainment AB. All Rights Reserved.
================================================================================================
*/
#ifndef traktor_model_LwsNode_H
#define traktor_model_LwsNode_H

#include <string>
#include "Core/Object.h"

namespace traktor
{
	namespace model
	{

/*! \brief
 * \ingroup Model
 */
class LwsNode : public Object
{
	T_RTTI_CLASS;

public:
	LwsNode(const std::wstring& name);

	const std::wstring& getName() const;

private:
	std::wstring m_name;
};

	}
}

#endif	// traktor_model_LwsNode_H
