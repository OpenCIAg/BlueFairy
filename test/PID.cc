#include <bluefairy.h>
#include <cmath>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>

using namespace bluefairy;

TEST(PID, FindAcceleration) {
  double target = 30.0;
  unsigned long t = 0;
  PID<double> pid(1.0, 0.5, 0.9);
  pid.setInput(0, t);
  double g = 9.8;
  double ac = 0;
  double ds = 0;
  int inc = 10;
  pid.setTarget(target);
  for (int i = 0; i < 1000; i++) {
    t += inc;
    pid.setInput(ds, t);
    double correction = pid.getOutput();
    ac += correction;
    ds += (ac - g) * ((double)inc / 1000.0);
    printf("c=%f,\tac=%f,\tds=%f,\tt=%d\n", correction, ac, ds, t);
  }
  EXPECT_EQ(round(ds), target);
}

TEST(PID, GotoPoint) {
  double target = 2500.0;
  unsigned long t = 0;
  PID<double> pid(0.1, 0.1, 0.9);
  pid.setInput(0, t);
  double g = 9.8;
  double ac = 0;
  double ds = 0;
  double de = 0;
  int inc = 50;
  pid.setTarget(target);
  for (int i = 0; i < 10000; i+=1) {
    t += inc;
    pid.setInput(de, t);
    double correction = pid.getOutput();
    double lastAccelaration = ac;
    ac = std::min(std::max(ac + correction, 0.0), 50.0);
    double lastSpeed = ds;
    ds += (ac - g) * ((double)inc / 1000.0);
    de += (ds * 2   + lastSpeed) / 3.0;
    //de += ds;
    printf("c=%f,\tac=%f,\tds=%f,\tde=%f,\tt=%d\n", correction, ac, ds, de, t);
  }
  EXPECT_EQ(round(de / 10), target / 10);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}