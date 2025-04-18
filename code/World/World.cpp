/*
 * TRAKTOR
 * Copyright (c) 2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "Render/IRenderSystem.h"
#include "World/Entity.h"
#include "World/IWorldComponent.h"
#include "World/World.h"
#include "World/Entity/CullingComponent.h"
#include "World/Entity/EventManagerComponent.h"
#include "World/Entity/IrradianceGridComponent.h"
#include "World/Entity/RTWorldComponent.h"

namespace traktor::world
{

T_IMPLEMENT_RTTI_CLASS(L"traktor.world.World", World, Object)

World::World(resource::IResourceManager* resourceManager, render::IRenderSystem* renderSystem)
{
	setComponent(new CullingComponent(resourceManager, renderSystem));
	setComponent(new EventManagerComponent(512));
	setComponent(new IrradianceGridComponent());
	if (renderSystem->supportRayTracing())
		setComponent(new RTWorldComponent(renderSystem));
}

void World::destroy()
{
	T_FATAL_ASSERT(m_deferredAdd.empty());
	T_FATAL_ASSERT(m_deferredRemove.empty());

	for (auto entity : m_entities)
	{
		entity->setWorld(nullptr);
		entity->destroy();
	}
	m_entities.clear();

	for (auto component : m_components)
		component->destroy();
	m_components.clear();
}

void World::setComponent(IWorldComponent* component)
{
	T_FATAL_ASSERT (component);

	// Replace existing component of same type.
	for (auto comp : m_components)
	{
		if (is_type_of(type_of(comp), type_of(component)))
		{
			comp = component;
			return;
		}
	}

	// No such component, add last.
	m_components.push_back(component);
}

IWorldComponent* World::getComponent(const TypeInfo& componentType) const
{
	for (auto component : m_components)
	{
		if (is_type_of(componentType, type_of(component)))
			return component;
	}
	return nullptr;
}

void World::addEntity(Entity* entity)
{
	T_FATAL_ASSERT(entity->getWorld() == nullptr);
	if (m_update)
		m_deferredAdd.push_back(entity);
	else
		m_entities.push_back(entity);
	entity->setWorld(this);
}

void World::removeEntity(Entity* entity)
{
	if (entity->getWorld() == nullptr)
		return;

	T_FATAL_ASSERT(entity->getWorld() == this);
	if (m_update)
		m_deferredRemove.push_back(entity);
	else
	{
		const bool removed = m_entities.remove(entity);
		T_FATAL_ASSERT(removed);
	}
	entity->setWorld(nullptr);
}

bool World::haveEntity(const Entity* entity) const
{
	if (entity->getWorld() == this)
		return std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end();
	else
		return false;
}

Entity* World::getEntity(const Guid& id) const
{
	for (auto entity : m_entities)
	{
		if (entity->getId() == id)
			return entity;
	}
	return nullptr;
}

Entity* World::getEntity(const std::wstring& name, int32_t index) const
{
	for (auto entity : m_entities)
	{
		if (entity->getName() == name)
		{
			if (index-- <= 0)
				return entity;
		}
	}
	return nullptr;
}

RefArray< Entity > World::getEntities(const std::wstring& name) const
{
	RefArray< Entity > entities;
	for (auto entity : m_entities)
	{
		if (entity->getName() == name)
			entities.push_back(entity);
	}
	return entities;
}

RefArray< Entity > World::getEntitiesWithinRange(const Vector4& position, float range) const
{
	RefArray< Entity > entities;
	for (auto entity : m_entities)
	{
		const Scalar distance = (entity->getTransform().translation() - position).xyz0().length();
		if (distance <= range)
			entities.push_back(entity);
	}
	return entities;
}

void World::update(const UpdateParams& update)
{
	// Update all world components.
	for (auto component : m_components)
		component->update(this, update);

	// Update all entities.
	m_update = true;
	for (auto entity : m_entities)
	{
		if (entity->getWorld() != nullptr)
			entity->update(update);
	}
	m_update = false;
	
	// Add entities which has been added during entity update.
	if (!m_deferredAdd.empty())
	{
		m_entities.insert(m_entities.end(), m_deferredAdd.begin(), m_deferredAdd.end());
		m_deferredAdd.resize(0);
	}

	// Remove entities which has been removed during entity update.
	if (!m_deferredRemove.empty())
	{
		for (auto entity : m_deferredRemove)
		{
			const bool removed = m_entities.remove(entity);
			T_FATAL_ASSERT(removed);
		}
		m_deferredRemove.resize(0);
	}
}

}
