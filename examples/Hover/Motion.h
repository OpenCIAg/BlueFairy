#ifndef __MOTION_H__
#define __MOTION_H__

typedef struct motion_status_t {
  float rightSpeed;
  float leftSpeed;
} MotionStatus;

class MotionController {
public:
  virtual void setDesiredStatus(const MotionStatus& desiredStatus) = 0;
  virtual void setCurrentStatus(const MotionStatus& currentStatus) = 0;
};

#endif