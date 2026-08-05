#include "Scene.h"
#include "Actor.h"
#include <iostream>
namespace nu
{
	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor->m_scene = this;
		m_pendingActors.push_back(std::move(actor));
	}
	void Scene::Update(float dt)
	{
		// 1. Flush pending actors into main collection
		for (auto& actor : m_pendingActors) {
			m_actors.push_back(std::move(actor));
		}
		m_pendingActors.clear();

		// 2. Update actors
		for (auto& actor : m_actors) {
			actor->Update(dt);
		}

		// 3. Resolve collisions
		UpdateCollisions();

		// 4. Remove destroyed actors safely
		std::erase_if(m_actors, [](const auto& actor) {
			return actor->m_destroyed;
			});
	}

	void Scene::Draw(const Renderer& renderer)
	{
		for (const auto& actor : m_actors)
		{
			if(actor)
			actor->Draw(renderer);
		}

	}
	void Scene::RemoveAllActors()
	{
		//for (auto& actor : m_actors)
		//{
		//	actor.reset();
		//}
		m_actors.clear();

	}
	void Scene::UpdateCollisions()
	{
		for (auto& actorA : m_actors) {

			for (auto& actorB : m_actors) {
				if (actorA == actorB || actorA->m_destroyed || actorB->m_destroyed) {
					//()
					continue;
				}
				float distance = (actorA->m_transform.position - actorB->m_transform.position).Length();
				if (distance <= actorA->GetRadious() + actorB->GetRadious()) {
					actorA->OnCollision(actorB.get());
					actorB->OnCollision(actorA.get());
				}

			}
		}

	}
	//void Scene::AddActor(Actor* actor) {
	//	actor->m_scene=this;
	//	m_pendingActors.push_back(actor);
	//}


}