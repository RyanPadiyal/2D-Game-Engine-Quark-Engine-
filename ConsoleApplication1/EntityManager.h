
#pragma once

#include "Entity.h"

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <algorithm>
#include <cstddef>

using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;

    std::map<std::string, EntityVec> m_entityMap;

    std::size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec)
    {
        vec.erase(
            std::remove_if(
                vec.begin(),
                vec.end(),
                [](const std::shared_ptr<Entity>& entity)
                {
                    return !entity->IsActive();
                }
            ),
            vec.end()
        );
    }

public:

    EntityManager() = default;

    void Update()
    {
        // Remove destroyed entities from the main collection.
        removeDeadEntities(m_entities);

        // Remove destroyed entities from each tag collection.
        for (auto& [tag, entityVec] : m_entityMap)
        {
            removeDeadEntities(entityVec);
        }

        // Move newly created entities into the main collection.
        for (auto& entity : m_entitiesToAdd)
        {
            m_entities.push_back(entity);
            m_entityMap[entity->Tag()].push_back(entity);
        }

        // Clear the waiting list.
        m_entitiesToAdd.clear();
    }

    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        auto entity = std::shared_ptr<Entity>(
            new Entity(tag,m_totalEntities++)
        );

        m_entitiesToAdd.push_back(entity);

        return entity;
    }

    const EntityVec& getEntities() const
    {
        return m_entities;
    }

    const EntityVec& getEntities(const std::string& tag)
    {
        return m_entityMap[tag];
    }

    const std::map<std::string, EntityVec>& getEntityMap() const
    {
        return m_entityMap;
    }
};
