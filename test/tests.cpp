// Copyright 2021 GHA Test Team

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include "TimedDoor.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;


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
  TimedDoor* door;

  void SetUp() override {
    door = new TimedDoor(1);
  }

  void TearDown() override {
    delete door;
  }
};


TEST_F(TimedDoorTest, DoorInitiallyClosed) {
  EXPECT_FALSE(door->isDoorOpened());
}


TEST_F(TimedDoorTest, DoorCanBeLocked) {
  door->lock();
  EXPECT_FALSE(door->isDoorOpened());
}


TEST_F(TimedDoorTest, DoorOpensWhenUnlocked) {
  EXPECT_THROW(door->unlock(), std::runtime_error);
}


TEST_F(TimedDoorTest, TimeoutValueCorrect) {
  EXPECT_EQ(door->getTimeOut(), 1);
}


TEST_F(TimedDoorTest, DoorStateTransition) {
  door->lock();
  EXPECT_FALSE(door->isDoorOpened());
  EXPECT_THROW(door->unlock(), std::runtime_error);
}


TEST_F(TimedDoorTest, ExceptionThrownWhenDoorLeftOpen) {
  EXPECT_THROW({
    door->unlock();
  }, std::runtime_error);
}


TEST_F(TimedDoorTest, NoExceptionWhenDoorClosedBeforeTimeout) {
  door->lock();
  EXPECT_NO_THROW(door->lock());
}


TEST_F(TimedDoorTest, ThrowStateThrowsException) {
  EXPECT_THROW(door->throwState(), std::runtime_error);
}


TEST(TimedDoorParameterTest, DifferentTimeoutValues) {
  TimedDoor door1(5);
  TimedDoor door2(10);

  EXPECT_EQ(door1.getTimeOut(), 5);
  EXPECT_EQ(door2.getTimeOut(), 10);
}


TEST(MockDoorTest, DoorInterfaceMocking) {
  MockDoor mockDoor;

  EXPECT_CALL(mockDoor, lock())
      .Times(1);
  EXPECT_CALL(mockDoor, isDoorOpened())
      .Times(1)
      .WillOnce(Return(false));

  mockDoor.lock();
  EXPECT_FALSE(mockDoor.isDoorOpened());
}


TEST(MockTimerClientTest, TimerClientInterfaceMocking) {
  MockTimerClient mockClient;

  EXPECT_CALL(mockClient, Timeout())
      .Times(AtLeast(1));

  mockClient.Timeout();
}


TEST_F(TimedDoorTest, MultipleOperations) {
  door->lock();
  EXPECT_FALSE(door->isDoorOpened());
  door->lock();
  EXPECT_FALSE(door->isDoorOpened());
}


TEST(TimedDoorCreationTest, AdapterCreation) {
  TimedDoor door(3);
  EXPECT_EQ(door.getTimeOut(), 3);
  EXPECT_FALSE(door.isDoorOpened());
}
