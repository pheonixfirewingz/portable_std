#include <vector.h>
#include <string.h>
#include <stdexcept.h>
#include "test.h"

void test_default_constructor()
{
    std::vector<int> v;
    TEST_CHECK(v.empty());
}

void test_constructor_with_size()
{
    std::vector<int> v(10);
    TEST_CHECK(v.size() == 10);
}

void test_constructor_with_size_and_value()
{
    std::vector<int> v(10, 5);
    TEST_CHECK(v.size() == 10);
    for (auto i : v)
    {
        TEST_CHECK(i == 5);
    }
}

void test_at()
{
    std::vector<int> v = {1, 2, 3};
    TEST_CHECK(v.at(0) == 1);
    TEST_CHECK(v.at(1) == 2);
    TEST_CHECK(v.at(2) == 3);
    TEST_EXCEPTION(v.at(3), std::out_of_range);
}

void test_operator_square_bracket()
{
    std::vector<int> v = {1, 2, 3};
    TEST_CHECK(v[0] == 1);
    TEST_CHECK(v[1] == 2);
    TEST_CHECK(v[2] == 3);
}

void test_front()
{
    std::vector<int> v = {1, 2, 3};
    TEST_CHECK(v.front() == 1);
}

void test_back()
{
    std::vector<int> v = {1, 2, 3};
    TEST_CHECK(v.back() == 3);
}

void test_data(void)
{
    std::vector<int> v = {1, 2, 3};
    int *data = v.data();
    TEST_CHECK(data[0] == 1);
    TEST_CHECK(data[1] == 2);
    TEST_CHECK(data[2] == 3);
}

void test_empty(void)
{
    std::vector<int> v;
    TEST_CHECK(v.empty());
    v.push_back(1);
    TEST_CHECK(!v.empty());
}

void test_size(void)
{
    std::vector<int> v;
    TEST_CHECK(v.size() == 0);
    v.push_back(1);
    TEST_CHECK(v.size() == 1);
    v.push_back(2);
    TEST_CHECK(v.size() == 2);
}

void test_max_size(void)
{
    std::vector<int> v;
    TEST_CHECK(v.max_size() > 0);
}

void test_resize(void)
{
    std::vector<int> v;
    v.resize(5);
    TEST_CHECK(v.size() == 5);
    v.resize(3);
    TEST_CHECK(v.size() == 3);
}

void test_reserve(void)
{
    std::vector<int> v;
    v.reserve(5);
    TEST_CHECK(v.capacity() == 5);
    v.reserve(10);
    TEST_CHECK(v.capacity() == 10);
}

void test_capacity(void)
{
    std::vector<int> v;
    TEST_CHECK(v.capacity() == 0);
    v.push_back(1);
    TEST_CHECK(v.capacity() > 0);
}

void test_clear(void)
{
    std::vector<int> v = {1, 2, 3};
    v.clear();
    TEST_CHECK(v.empty());
}

void test_insert(void)
{
    std::vector<int> v = {1, 2, 3};
    v.insert(v.begin() + 1, 4);
    TEST_CHECK(v[1] == 4);
}

void test_erase(void)
{
    std::vector<int> v = {1, 2, 3};
    v.erase(v.begin() + 1);
    TEST_CHECK(v[1] == 3);
}

void test_push_back(void)
{
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    TEST_CHECK(v[0] == 1);
    TEST_CHECK(v[1] == 2);
    TEST_CHECK(v[2] == 3);
}

void test_pop_back(void)
{
    std::vector<int> v = {1, 2, 3};
    v.pop_back();
    TEST_CHECK(v[1] == 2);
    v.pop_back();
    TEST_CHECK(v[0] == 1);
    v.pop_back();
    TEST_CHECK(v.empty());
}

void test_swap(void)
{
    std::vector<int> v1 = {1, 2, 3};
    std::vector<int> v2 = {4, 5, 6};
    v1.swap(v2);
    TEST_CHECK(v1[0] == 4);
    TEST_CHECK(v1[1] == 5);
    TEST_CHECK(v1[2] == 6);
    TEST_CHECK(v2[0] == 1);
    TEST_CHECK(v2[1] == 2);
    TEST_CHECK(v2[2] == 3);
}

void test_emplace_back(void)
{
    std::vector<std::string> v;
    v.emplace_back("hello");
    TEST_CHECK(v[0] == "hello");
}

TEST("default constructor", default_constructor, test_default_constructor);
TEST("constructor with size", constructor_with_size, test_constructor_with_size);
TEST("constructor with size and value", constructor_with_size_and_value, test_constructor_with_size_and_value);
TEST("at", at, test_at);
TEST("operator square bracket", operator_square_bracket, test_operator_square_bracket);
TEST("front", front, test_front);
TEST("back", back, test_back);
TEST("data", data, test_data);
TEST("empty", empty, test_empty);
TEST("size", size, test_size);
TEST("max size", max_size, test_max_size);
TEST("resize", resize, test_resize);
TEST("reserve", reserve, test_reserve);
TEST("capacity", capacity, test_capacity);
TEST("clear", clear, test_clear);
TEST("insert", insert, test_insert);
TEST("erase", erase, test_erase);
TEST("push back", push_back, test_push_back);
TEST("pop back", pop_back, test_pop_back);
TEST("swap", swap, test_swap);
TEST("emplace back", emplace_back, test_emplace_back)
