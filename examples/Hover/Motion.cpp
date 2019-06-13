#include "Motion.h"

void PIDMotionController::setCurrentStatus(const MotionStatus& currentStatus) {
  this->currentStatus = currentStatus;
  this->leftPid.setInput(currentStatus.leftSpeed);
  this->rightPid.setInput(currentStatus.rightSpeed);
  this->update();
}

void PIDMotionController::setDesiredStatus(const MotionStatus& desiredStatus) {
  this->leftPid.setTarget(desiredStatus.leftSpeed);
  this->rightPid.setTarget(desiredStatus.rightSpeed);
  this->setCurrentStatus(this->currentStatus);
  this->update();
}

void PIDMotionController::update() {
  float leftCorrection = this->leftPid.getOutput();
  float rightCorrection = this->rightPid.getOutput();
  this->leftSpeed += leftCorrection;
  this->rightSpeed += rightCorrection;
}