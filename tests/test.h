#pragma once
using InitFunc = void (*)();

extern "C"
{
    extern InitFunc __start_myinit_funcs[];
    extern InitFunc __stop_myinit_funcs[];
}

void print(const char *str);



#define TEST(name,v_name, func) __attribute__((used, section("myinit_funcs"))) static InitFunc v_name = []() { \
    print("test: ");\
    print(name);\
    func(); };

#define TEST_CHECK(expr) \
    if (!(expr)) { \
        print("test failed: "); \
        print(#expr); \
        return; \
    }

#define TEST_EXCEPTION(expr, exception) \
    try { \
        expr; \
        print("test failed no exception thrown: "); \
        print(#expr); \
    } catch (exception &) { \
        print("test passed: "); \
        print(#expr); \
    } catch (...) { \
        print("test failed: "); \
        print(#expr); \
        return; \
    }
