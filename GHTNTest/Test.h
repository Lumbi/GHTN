#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <initializer_list>
#include <memory>
#include <vector>

namespace GHTNTest
{
    struct TestEntry
    {
        std::string m_Name;
        std::function<void(void)> m_Func;
    };

    class TestRegistry
    {
    public:
        static TestRegistry& Instance()
        {
            static TestRegistry instance;
            return instance;
        }

        void Register(std::string name, std::function<void()> func)
        {
            m_TestsEntries.emplace_back(std::move(name), std::move(func));
        }

        void RunAllTests()
        {
            for (auto&& entry : m_TestsEntries)
            {
                std::string testDisplayName(entry.m_Name);
                std::replace(std::begin(testDisplayName), std::end(testDisplayName), '_', ' ');

                std::cout << "Testing: '" << testDisplayName << "'\n";
                entry.m_Func();
            }
        }

    private:
        std::vector<TestEntry> m_TestsEntries;
    };

    void RunAllTests()
    {
        TestRegistry::Instance().RunAllTests();
    }
}

#define TEST(TestName) \
void TestName(); \
namespace \
{ \
    [[maybe_unused]] static auto TestRegistrationHandler_##TestName = []() \
    { \
        using namespace GHTNTest; \
        TestRegistry::Instance().Register(#TestName, []() { TestName(); }); \
        return nullptr; \
    } \
    (); \
} \
void TestName()

#define EXPECT(expression) \
{ \
    if (!(expression)) \
    { \
        printf("Expectation failure @ %s::%d - expected '%s'\n", __FUNCTION__, __LINE__, #expression); \
        return; \
    } \
}
