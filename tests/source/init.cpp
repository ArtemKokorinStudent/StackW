#include <catch.hpp>

#include "stack.hpp"

TEST_CASE("Stack can be instantiated by various types", "[instantiation]") {
	REQUIRE_NOTHROW(stack<int> st1);
	REQUIRE_NOTHROW(stack<double> st1);
	REQUIRE_NOTHROW(stack<char> st1);
	REQUIRE_NOTHROW(stack<int*> st1);
	REQUIRE_NOTHROW(stack<stack<int>> st1);
}
TEST_CASE("Push, pop, top", "[push_pop_top]") {
	stack<int> st;
	st.push(1);
	st.push(2);
	REQUIRE(st.top() == 2);
	st.pop();
	REQUIRE(st.top() == 1);
}
TEST_CASE("count", "[count]") {
	stack<int> st;
	REQUIRE(st.count() == 0);
	st.push(1);
	REQUIRE(st.count() == 1);
	st.push(2);
	REQUIRE(st.count() == 2);
	st.pop();
	REQUIRE(st.count() == 1);
	st.pop();
	REQUIRE(st.count() == 0);
}
TEST_CASE("Copy constructor, =", "[copy_ctr, =]") {
	stack<int> st1;
	st1.push(1);
	st1.push(2);
	REQUIRE_NOTHROW(stack<int> st2 = st1);
	REQUIRE_NOTHROW(stack<int> st2;
	st2 = st1);

	stack<int> st3 = st1;
	stack<int> st4 = st1;
	REQUIRE(st1.top() == 2);
	REQUIRE(st3.top() == 2);
	st3.pop();
	REQUIRE(st3.top() == 1);
	REQUIRE(st4.top() == 2);
	st4.pop();
	REQUIRE(st4.top() == 1);
}
TEST_CASE("Empty", "[empty]") {
	stack<int> st1;
	REQUIRE(st1.empty());
	st1.push(38);
	REQUIRE(!st1.empty());
}
