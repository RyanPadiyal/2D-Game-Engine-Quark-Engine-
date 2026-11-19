
#pragma once

#include "Components.h"
#include <string>
#include <tuple>
#include <utility>
#include <cstddef>

class EntityManager;

using ComponentTuple = std::tuple<
    CTransform,
    CShape,
    CCollision,
    CInput,
    CScore,
    CLifespan
>;

class Entity
{
    friend class EntityManager;

    ComponentTuple m_tuple;
    bool m_active = true;
    std::string m_tag = "default";
    std::size_t m_id = 0;

    Entity(const std::string& tag, const std::size_t& id)
        : m_tag(tag), m_id(id)
    {
    }

public:

    bool IsActive() const
    {
        return m_active;
    }

    void Destroy()
    {
        m_active = false;
    }

    std::size_t Id() const
    {
        return m_id;
    }

    const std::string& Tag() const
    {
        return m_tag;
    }

    template <typename T>
    T& get()
    {
        return std::get<T>(m_tuple);
    }

    template <typename T>
    const T& get() const
    {
        return std::get<T>(m_tuple);
    }

    template <typename T>
    bool has() const
    {
        return get<T>().exists;
    }

    template <typename T, typename... TArgs>
    T& add(TArgs&&... mArgs)
    {
        T& component = get<T>();

        component = T(std::forward<TArgs>(mArgs)...);

        component.exists = true;

        return component;
    }

    template <typename T>
    void remove()
    {
        get<T>() = T();
    }
};
