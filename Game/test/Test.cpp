#include "InputTests.h"
#include "PhysicsTests.h"
#include "SceneTests.h"

#include <gtest/gtest.h>

using namespace testing;

class ConfigurableEventListener : public TestEventListener {
protected:
    TestEventListener* eventListener;
    
public:
    
    // Show the names of each test case.
    bool showTestCases;
    
    // Show the names of each test.
    bool showTestNames;
    
    // Show each success.
    bool showSuccesses;
    
    // Show each failure as it occurs. You will also see it at the bottom after the full suite is run.
    bool showInlineFailures;
    
    // Show the setup of the global environment.
    bool showEnvironment;
    
    explicit ConfigurableEventListener(TestEventListener* theEventListener) : eventListener(theEventListener) {
        showTestCases = true;
        showTestNames = true;
        showSuccesses = true;
        showInlineFailures = true;
        showEnvironment = true;
    }
    
    virtual ~ConfigurableEventListener() {
        delete eventListener;
    }
    
    virtual void OnTestProgramStart(const UnitTest& unit_test) {
        eventListener->OnTestProgramStart(unit_test);
    }
    
    virtual void OnTestIterationStart(const UnitTest& unit_test, int iteration) {
        eventListener->OnTestIterationStart(unit_test, iteration);
    }
    
    virtual void OnEnvironmentsSetUpStart(const UnitTest& unit_test) {
        if(showEnvironment) {
            eventListener->OnEnvironmentsSetUpStart(unit_test);
        }
    }
    
    virtual void OnEnvironmentsSetUpEnd(const UnitTest& unit_test) {
        if(showEnvironment) {
            eventListener->OnEnvironmentsSetUpEnd(unit_test);
        }
    }
    
    virtual void OnTestCaseStart(const TestCase& test_case) {
        if(showTestCases) {
            eventListener->OnTestCaseStart(test_case);
        }
    }
    
    virtual void OnTestStart(const TestInfo& test_info) {
        if(showTestNames) {
            eventListener->OnTestStart(test_info);
        }
    }
    
    virtual void OnTestPartResult(const TestPartResult& result) {
        eventListener->OnTestPartResult(result);
    }
    
    virtual void OnTestEnd(const TestInfo& test_info) {
        if((showInlineFailures && test_info.result()->Failed()) || (showSuccesses && !test_info.result()->Failed())) {
            eventListener->OnTestEnd(test_info);
        }
    }
    
    virtual void OnTestCaseEnd(const TestCase& test_case) {
        if(showTestCases) {
            eventListener->OnTestCaseEnd(test_case);
        }
    }
    
    virtual void OnEnvironmentsTearDownStart(const UnitTest& unit_test) {
        if(showEnvironment) {
            eventListener->OnEnvironmentsTearDownStart(unit_test);
        }
    }
    
    virtual void OnEnvironmentsTearDownEnd(const UnitTest& unit_test) {
        if(showEnvironment) {
            eventListener->OnEnvironmentsTearDownEnd(unit_test);
        }
    }
    
    virtual void OnTestIterationEnd(const UnitTest& unit_test, int iteration) {
        eventListener->OnTestIterationEnd(unit_test, iteration);
    }
    
    virtual void OnTestProgramEnd(const UnitTest& unit_test) {
        eventListener->OnTestProgramEnd(unit_test);
    }
    
};

int main(int argc, char **argv) 
{
    // Initialize
    ::testing::InitGoogleTest(&argc, argv);
    
    // Remove the default listener
    testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
    auto default_printer = listeners.Release(listeners.default_result_printer());
    
    // Configure Output
    ConfigurableEventListener *listener = new ConfigurableEventListener(default_printer);
    listener->showEnvironment = false;
    listener->showTestCases = true;
    listener->showTestNames = true;
    listener->showSuccesses = false;
    listener->showInlineFailures = false;
    listeners.Append(listener);
    
    // Run
    return RUN_ALL_TESTS();
}
