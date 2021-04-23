//
// Created by konstantin on 02/02/2021.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../src/Entity/Component.h"
#include "../src/Entity/Entity.h"
#include "../src/Entity/EntityManager.h"

struct TestComponent1 : public Component {
	int x;
	TestComponent1() { x = -1; };
	explicit TestComponent1(int x)
		: x(x) {};
};
struct TestComponent2 : public Component {
};

TEST_CASE("Given Entity with Components of type TestComponent1, EntityManager "
		  "returns it")
{
	EntityManager::getManager().clearEntities();

	auto e1 = std::make_unique<Entity>();
	e1->addComponent<TestComponent1>(15);

	auto e2 = std::make_unique<Entity>();
	e2->addComponent<TestComponent2>();

	EntityManager::getManager().addEntity(std::move(e1));
	EntityManager::getManager().addEntity(std::move(e2));

	REQUIRE(EntityManager::getManager()
				.getEntitiesWithComponent<TestComponent1>()
				.size() == 1);
}

TEST_CASE("Given Entities with different components, getAllComponents returns "
		  "the right ones")
{
	EntityManager::getManager().clearEntities();

	auto e1 = std::make_unique<Entity>();
	e1->addComponent<TestComponent1>(15);

	auto e2 = std::make_unique<Entity>();
	e2->addComponent<TestComponent2>();

	auto e3 = std::make_unique<Entity>();
	e3->addComponent<TestComponent2>();

	EntityManager::getManager().addEntity(std::move(e1));
	EntityManager::getManager().addEntity(std::move(e2));
	EntityManager::getManager().addEntity(std::move(e3));

	REQUIRE(
		EntityManager::getManager().getAllComponents<TestComponent2>().size() ==
		2);
}