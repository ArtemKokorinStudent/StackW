#include "catch.hpp"

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
	st.pop();
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
TEST_CASE("Copy constructor", "[copy_ctr]") {
	stack<int> st1;
	st1.push(1);
	st1.push(2);
	REQUIRE_NOTHROW(stack<int> st2 = st1);

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
TEST_CASE("Equal", "[equal]") {
	stack<int> st1;
	REQUIRE_NOTHROW(stack<int> st2;
	st2 = st1);
	stack<int> st2;
	st1.push(1);
	st1.push(4);
	st2 = st1;
	REQUIRE(st2.top() == 4);
	st2.pop();
	REQUIRE(st2.top() == 1);
}
TEST_CASE("Empty", "[empty]") {
	stack<int> st1;
	REQUIRE(st1.empty());
	st1.push(38);
	REQUIRE(!st1.empty());
}
TEST_CASE("Allocator doesn't (use or need) default ctors", "[allocator]") {
	class A {
	public:
		A() {
			throw "ctor is called";
		}
		A(int a) { 
			;
		}
	};
	REQUIRE_NOTHROW(
		stack<A> st;
		st.push(4);
		st.push(20);
	);
	class B {
	public:
		B(int a) {
			;
		}
	};
	stack<B> st;
	st.push(40);
}
bool dtor_is_called1 = false;
TEST_CASE("Allocator calls dtors", "[allocator]") {
	class A {
	public:
		A(int a) {
			;
		}
		~A() {
			dtor_is_called1 = true;
		}
	};
	stack<A> st;
	st.push(32);
	st.pop();
	REQUIRE(dtor_is_called1);
}
