/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <functional>
#include "Core/Log/Log.h"
#include "Core/Math/Format.h"
#include "Core/Misc/TString.h"
#include "Model/Model.h"
#include "Model/Pose.h"
#include "Model/Formats/Fbx/Conversion.h"
#include "Model/Formats/Fbx/SkeletonConverter.h"

namespace traktor::model
{
	namespace
	{

ufbx_node* search(ufbx_node* node, const std::function< bool (ufbx_node* node) >& predicate)
{
	if (predicate(node))
		return node;

	for (size_t i = 0; i < node->children.count; ++i)
	{
		ufbx_node* child = node->children.data[i];
		if (child)
		{
			ufbx_node* foundNode = search(child, predicate);
			if (foundNode)
				return foundNode;
		}
	}

	return nullptr;
}

bool traverse(ufbx_node* parent, ufbx_node* node, const std::function< bool (ufbx_node* parent, ufbx_node* node) >& visitor)
{
	if (!node)
		return true;

	if (!visitor(parent, node))
		return false;

	for (size_t i = 0; i < node->children.count; ++i)
	{
		ufbx_node* child = node->children.data[i];
		if (child)
		{
			if (!traverse(node, child, visitor))
				return false;
		}
	}

	return true;
}

std::wstring getJointName(ufbx_node* node)
{
	std::wstring jointName = mbstows(node->name.data);

	const size_t p = jointName.find(L':');
	if (p != std::wstring::npos)
		jointName = jointName.substr(p + 1);

	return jointName;
}

// GearUp port: GU rigs author joint chains in file units (cm) behind a
// compensating scale on the rig root node, while geometry is authored in
// meters. Every local joint translation must be scaled by the rig root's
// local scaling (replicates the 2018 FBX-SDK importer's GetNodeLocalScaling
// handling).
Vector4 scaledLocalTranslation(const ufbx_node* node, const ufbx_vec3& rootScale)
{
	const ufbx_vec3& t = node->local_transform.translation;
	return Vector4(
		(float)(t.x * rootScale.x),
		(float)(t.y * rootScale.y),
		(float)(t.z * rootScale.z),
		0.0f
	);
}

	}

bool convertSkeleton(
	Model& outModel,
	ufbx_scene* scene,
	ufbx_node* skeletonNode,
	const Matrix44& axisTransform
)
{
	// GearUp port: replicate the 2018 FBX-SDK importer's skeleton convention,
	// which GU rigs are authored for: a joint is its authored LOCAL translation
	// only (identity rotation), scaled by the rig root node's local scaling,
	// and the rig root itself sits at the origin -- aligned with the pivot
	// re-based geometry (see MeshConverter). Upstream's bind_pose/world-matrix
	// read yields cm translations against meter geometry, which mangles
	// skinning the moment a controller writes a real pose.
	const ufbx_vec3 rootScale = skeletonNode->local_transform.scale;

	const bool result = traverse(nullptr, skeletonNode, [&](ufbx_node* parent, ufbx_node* node) {
		const std::wstring jointName = getJointName(node);

		Vector4 translation = Vector4::zero();
		uint32_t parentId = c_InvalidIndex;

		if (parent != nullptr)
		{
			translation = axisTransform * scaledLocalTranslation(node, rootScale);

			const std::wstring parentJointName = getJointName(parent);
			parentId = outModel.findJointIndex(parentJointName);
			if (parentId == c_InvalidIndex)
				log::warning << L"Unable to bind parent joint; no such joint \"" << parentJointName << L"\"." << Endl;
		}

		Joint joint;
		joint.setParent(parentId);
		joint.setName(jointName);
		joint.setTransform(Transform(translation, Quaternion::identity()));
		outModel.addJoint(joint);
		return true;
	});

	return result;
}

Ref< Pose > convertPose(
	const Model& model,
	ufbx_scene* scene,
	ufbx_node* skeletonNode,
	ufbx_anim* anim,
	double time,
	const Matrix44& axisTransform
)
{
	ufbx_evaluate_opts opts = {};
	ufbx_scene* escene = ufbx_evaluate_scene(scene, anim, time, &opts, nullptr);
	if (!escene)
		return nullptr;

	// Find evaluated skeleton.
	ufbx_node* eskeletonNode = search(escene->root_node, [&](ufbx_node* node) {
		return node->element_id == skeletonNode->element_id;
	});
	if (!eskeletonNode)
	{
		ufbx_free_scene(escene);
		return nullptr;
	}

	// GearUp port: poses follow the same convention as convertSkeleton --
	// evaluated LOCAL translations scaled by the BIND rig root scale, local
	// rotations axis-converted, and the root's bind translation re-based away.
	const ufbx_vec3 rootScale = skeletonNode->local_transform.scale;
	const Vector4 rootBind = axisTransform * scaledLocalTranslation(skeletonNode, rootScale);

	const Matrix44 axisTransformInv = axisTransform.inverse();

	Ref< Pose > pose = new Pose();
	const bool result = traverse(nullptr, eskeletonNode, [&](ufbx_node* parent, ufbx_node* node) {
		const std::wstring jointName = getJointName(node);
		const uint32_t jointId = model.findJointIndex(jointName);
		if (jointId == c_InvalidIndex)
		{
			log::warning << L"Unable to find joint \"" << jointName << L"\" in skeleton; unable to save pose for joint." << Endl;
			return true;
		}

		Vector4 translation = axisTransform * scaledLocalTranslation(node, rootScale);
		if (parent == nullptr)
			translation -= rootBind;

		// Local rotation, axis-converted by conjugation.
		ufbx_transform rt = {};
		rt.rotation = node->local_transform.rotation;
		rt.scale.x = rt.scale.y = rt.scale.z = 1.0;
		const Matrix44 R = axisTransform * convertMatrix(ufbx_transform_to_matrix(&rt)) * axisTransformInv;

		pose->setJointTransform(jointId, Transform(translation, Quaternion(R).normalized()));
		return true;
	});

	ufbx_free_scene(escene);
	return pose;
}

}
