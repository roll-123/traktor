/*
================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION/NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright 2017 Doctor Entertainment AB. All Rights Reserved.
================================================================================================
*/
#ifndef traktor_model_LwsDocument_H
#define traktor_model_LwsDocument_H

#include "Core/Object.h"
#include "Core/Ref.h"

namespace traktor
{

class IStream;

	namespace model
	{

class LwsGroup;

/*! \brief
 * \ingroup Model
 */
class LwsDocument : public Object
{
	T_RTTI_CLASS;

public:
	static Ref< LwsDocument > parse(IStream* stream);

	Ref< LwsGroup > getRootGroup();

private:
	Ref< LwsGroup > m_rootGroup;
};

	}
}

#endif	// traktor_model_LwsDocument_H
