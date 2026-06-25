/*
================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION/NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright 2017 Doctor Entertainment AB. All Rights Reserved.
================================================================================================
*/
#include "Model/Formats/Lws/LwsEmpty.h"

namespace traktor
{
	namespace model
	{

T_IMPLEMENT_RTTI_CLASS(L"traktor.model.LwsEmpty", LwsEmpty, LwsNode)

LwsEmpty::LwsEmpty()
:	LwsNode(L"")
{
}

	}
}
