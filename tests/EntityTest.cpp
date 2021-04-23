//
// Created by konstantin on 02/02/2021.
//

#include "catch.hpp"
#include "../src/Entity/Component.h"
#include "../src/Entity/Entity.h"

struct TestComponent1 : public Component {
	int x;
	TestComponent1() { x = -1; };
	explicit TestComponent1(int x)
		: x(x) {};
};
struct TestComponent2 : public Component {
};

TEST_CASE("Given Entity with Components of type TestComponent1, hasComponent "
		  "returns true")
{
	Entity e = Entity();
	e.addComponent<TestComponent1>();

	REQUIRE(e.hasComponent<TestComponent1>());
}

TEST_CASE("Given Entity with Components of type TestComponent2, hasComponent "
		  "returns false")
{
	Entity e = Entity();
	e.addComponent<TestComponent1>();

	REQUIRE(!e.hasComponent<TestComponent2>());
}

TEST_CASE("Given Entity with Components of type TestComponent1, getComponent "
		  "returns it")
{
	Entity e = Entity();
	int x = 9;
	e.addComponent<TestComponent1>(x);

	auto c = e.getComponent<TestComponent1>();
	REQUIRE(c != nullptr);
	REQUIRE(c->x == x);
}

TEST_CASE("Given Entity with Components of type TestComponent1, getComponent "
		  "returns null for TC2")
{
	Entity e = Entity();
	int x = 9;
	e.addComponent<TestComponent1>(x);

	auto c = e.getComponent<TestComponent2>();
	REQUIRE(c == nullptr);
}