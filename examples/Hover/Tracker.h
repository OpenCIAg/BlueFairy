#ifndef __TRACKER_H__
#define __TRACKER_H__

#include <bluefairy.h>

using bluefairy::List;

typedef struct track_sample_t {
  unsigned int deltaTime;
  int rightSteps;
  int leftSteps;
} TrackSample;

typedef void (*TrackSampleListener)(const TrackSample&);

class TrackRecorder {
public:
  virtual void record(const TrackSample& sample);
};

class InListRecorder : public TrackRecorder {
protected:
  List<TrackSample>& trackList;

public:
  InListRecorder(List<TrackSample>& trackList);
  void record(const TrackSample& sample);
};

InListRecorder::InListRecorder(List<TrackSample>& trackList)
    : trackList(trackList) {
}

void InListRecorder::record(const TrackSample& sample) {
  this->trackList.add(sample);
}

template <typename E>
class Trail {
public:
  virtual void emit(E trackSampleListener) = 0;
};

#endif