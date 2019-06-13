#ifndef __MOTION_H__
#define __MOTION_H__

#include "bluefairy.h"

using bluefairy::PID;

typedef struct motion_status_t {
  float rightSpeed;
  float leftSpeed;
} MotionStatus;

class MotionController {
public:
  virtual void setDesiredStatus(const MotionStatus& desiredStatus) = 0;
  virtual void setCurrentStatus(const MotionStatus& currentStatus) = 0;
};

class PIDMotionController : public MotionController {
protected:
  PID<float> leftPid;
  PID<float> rightPid;
  MotionStatus currentStatus;
  float leftSpeed = 0;
  float rightSpeed = 0;

  void update();

public:
  void setCurrentStatus(const MotionStatus& currentStatus);
  void setDesiredStatus(const MotionStatus& desiredStatus);
};



#endif
