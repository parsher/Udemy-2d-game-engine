#include <iostream>
#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData()
{
  for (auto &entity : entities)
  {
    entity->Destroy();
  }
}

bool EntityManager::HasNoEntities()
{
  return entities.size() == 0;
}

void EntityManager::Update(float deltaTime)
{
  for (auto &entity : entities)
  {
    entity->Update(deltaTime);
  }
}

void EntityManager::Render()
{
  for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++)
  {
    for (auto &entity : GetEntitiesByLayer(static_cast<LayerType>(layerNumber)))
    {
      entity->Render();
    }
  }
}

std::string EntityManager::CheckEntityCollisions(Entity &myEntity) const
{
  ColliderComponent *myCollider = myEntity.GetComponent<ColliderComponent>();
  for (auto &entity : entities)
  {
    if (entity->name.compare(myEntity.name) != 0 && entity->name.compare("Tile") != 0) // not tile && my self 
    {
      if (entity->HasComponent<ColliderComponent>())
      {
        ColliderComponent *otherCollider = entity->GetComponent<ColliderComponent>();
        if (Collision::CheckRectangleCollision(myCollider->collider, otherCollider->collider))
        {
          return otherCollider->colliderTag;
        }
      }
    }
  }
  return std::string();
}

Entity &EntityManager::AddEntity(std::string entityName, LayerType layer)
{
  Entity *entity = new Entity(*this, entityName, layer);
  entities.emplace_back(entity);
  return *entity;
}

std::vector<Entity *> EntityManager::GetEntities() const
{
  return entities;
}

std::vector<Entity *> EntityManager::GetEntitiesByLayer(LayerType layer) const
{
  std::vector<Entity *> selectedEntities;
  for (auto &entity : entities)
  {
    if (entity->layer == layer)
    {
      selectedEntities.emplace_back(entity);
    }
  }

  return selectedEntities;
}

unsigned int EntityManager::GetEntityCount()
{
  return entities.size();
}