// Copyright 2021 GHA Test Team

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include "TimedDoor.h"
#include <thread>
#include <chrono>

using namespace testing;

class MockTimerClient : public TimerClient {
public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class MockDoor : public Door {
public:
    MOCK_METHOD(void, lock, (), (override));
    MOCK_METHOD(void, unlock, (), (override));
    MOCK_METHOD(bool, isDoorOpened, (), (override));
};

class TimedDoorTest : public ::testing::Test {
protected:
    void SetUp() override {
        door = new TimedDoor(2);
        timer = new Timer();
        adapter = new DoorTimerAdapter(*door);
    }

    void TearDown() override {
        delete door;
        delete timer;
        delete adapter;
    }

    TimedDoor* door;
    Timer* timer;
    DoorTimerAdapter* adapter;
};

TEST_F(TimedDoorTest, DoorCreation) {
    EXPECT_FALSE(door->isDoorOpened());
    EXPECT_EQ(door->getTimeOut(), 2);
}

TEST_F(TimedDoorTest, DoorOpenClose) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());

    door->lock();
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, NoExceptionWhenDoorClosed) {
    door->lock();
    EXPECT_NO_THROW(door->throwState());
}

TEST_F(TimedDoorTest, ExceptionWhenDoorOpened) {
    door->unlock();
    EXPECT_THROW(door->throwState(), std::runtime_error);
}

TEST_F(TimedDoorTest, AdapterWithClosedDoor) {
    door->lock();
    EXPECT_NO_THROW(adapter->Timeout());
}

TEST_F(TimedDoorTest, AdapterWithOpenedDoor) {
    door->unlock();
    EXPECT_THROW(adapter->Timeout(), std::runtime_error);
}

TEST(TimerTest, TimerCallsTimeout) {
    MockTimerClient mockClient;
    Timer timer;

    EXPECT_CALL(mockClient, Timeout())
        .Times(1);

    std::thread timerThread([&timer, &mockClient]() {
        timer.tregister(1, &mockClient);
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    timerThread.join();
}

TEST_F(TimedDoorTest, FullScenarioDoorClosedOnTime) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());

    std::this_thread::sleep_for(std::chrono::seconds(1));
    door->lock();
    EXPECT_FALSE(door->isDoorOpened());

    EXPECT_NO_THROW(adapter->Timeout());
}

TEST_F(TimedDoorTest, FullScenarioDoorNotClosed) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());

    std::this_thread::sleep_for(std::chrono::seconds(3));

    EXPECT_THROW(adapter->Timeout(), std::runtime_error);
}

TEST_F(TimedDoorTest, MultipleOperations) {
    for (int i = 0; i < 5; i++) {
        door->unlock();
        EXPECT_TRUE(door->isDoorOpened());

        door->lock();
        EXPECT_FALSE(door->isDoorOpened());
    }
}

TEST(TimerTest, TimerWithDifferentValues) {
    MockTimerClient mockClient;
    Timer timer;

    EXPECT_CALL(mockClient, Timeout())
        .Times(2);

    std::thread timerThread1([&timer, &mockClient]() {
        timer.tregister(1, &mockClient);
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    timerThread1.join();

    std::thread timerThread2([&timer, &mockClient]() {
        timer.tregister(2, &mockClient);
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    timerThread2.join();
}

TEST_F(TimedDoorTest, ZeroTimeoutDoor) {
    TimedDoor* zeroDoor = new TimedDoor(0);
    EXPECT_EQ(zeroDoor->getTimeOut(), 0);

    zeroDoor->unlock();
    EXPECT_THROW(zeroDoor->throwState(), std::runtime_error);

    delete zeroDoor;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}