/*
================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION/NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright 2017 Doctor Entertainment AB. All Rights Reserved.
================================================================================================
*/
#ifndef traktor_model_LwsEmpty_H
#define traktor_model_LwsEmpty_H

#include "Model/Formats/Lws/LwsNode.h"

namespace traktor
{
	namespace model
	{

/*! \brief
 * \ingroup Model
 */
class LwsEmpty : public LwsNode
{
	T_RTTI_CLASS;

public:
	LwsEmpty();
};

	}
}

#endif	// traktor_model_LwsEmpty_H
