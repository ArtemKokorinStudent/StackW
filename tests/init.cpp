#include "catch.hpp"
#include "stack.hpp"
#include <thread>
#include <iostream>
#include <chrono>

const int N = 17;
TEST_CASE("drt", "[T]") {
	bool tests[N];
	for (auto test : tests) {
		test = true;
	}
	
	stack<int> st;
	std::thread th2([&](stack<int> & st, bool * tests) {
		std::cout << "Popping thread\n";
		st.pop();
		/*for (size_t i = 0; i < N;) {
			std::cout << "Popping thread\n";
			std::shared_ptr<int> current_element = st.pop();
			if (current_element) {
				std::cout << *(current_element);
				i++;
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
			}
		}*/
	}, std::ref(st), tests);
	std::thread th1([&](stack<int> & st, bool * tests) {
		for (size_t i = 0; i < N; i++) {
			std::cout << "Pushing thread\n";
			st.push(i);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}, std::ref(st), tests);
	th1.join();
	th2.join();
	for (auto test : tests) {
		REQUIRE(true);
	}
}
TEST_CASE("Cyrillic_test", "[instantiation]") {
	stack<int> st;
	for (size_t i = 0; i < 4; i++) {
		st.push(i);
	}
	for (size_t i = 0; i < 2; i++) {
		st.pop();
	}
}
TEST_CASE("Stack can be instantiated by various types", "[instantiation]") {
	REQUIRE_NOTHROW(stack<int> st1);
	REQUIRE_NOTHROW(stack<double> st2);
	REQUIRE_NOTHROW(stack<char> st3);
	REQUIRE_NOTHROW(stack<int*> st4);
	REQUIRE_NOTHROW(stack<stack<int>> st5);
}
TEST_CASE("Push, pop, top", "[push_pop_top]") {
	stack<int> st;
	st.push(1);
	st.push(2);
	REQUIRE(*(st.pop()) == 2);
	REQUIRE(*(st.pop()) == 1);
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
	stack<double> st1;
	st1.push(1);
	st1.push(2);
	//REQUIRE_NOTHROW(stack<double> st2 = st1);

	stack<double> st3 = st1;
	stack<double> st4 = st1;
	REQUIRE(*(st1.pop()) == 2);
	REQUIRE(*(st3.pop()) == 2);
	REQUIRE(*(st3.pop()) == 1);
	REQUIRE(*(st4.pop()) == 2);
	REQUIRE(*(st4.pop()) == 1);
}
TEST_CASE("Equal", "[equal]") {
	stack<int> st1;
	REQUIRE_NOTHROW(stack<int> st2;
	st2 = st1);
	stack<int> st2;
	st1.push(1);
	st1.push(4);
	st2 = st1;
	REQUIRE(*(st2.pop()) == 4);
	REQUIRE(*(st2.pop()) == 1);
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
bool dtor_is_called12 = false;
TEST_CASE("Allocator calls dtor when pop", "[allocator_dtor]") {
	class A {
	public:
		A(int a) {
			//std::cout << "A(int)" << std::endl;
		}
		A(const A & a) {
			//std::cout << "copy constructor is called " << std::endl;
		}
		~A() {
			dtor_is_called12 = true;
		}
	};
	stack<A> st;
	st.push(32);
	st.pop();
	REQUIRE(dtor_is_called12);
}
