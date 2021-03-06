#ifndef ANGER_H_
#define ANGER_H_

#include "emotion.h"
#include "controller.h"

using namespace std;

class Anger: public Emotion{

public:
	Anger(Controller* controller, unsigned long start);
	~Anger();

	void motorAction();
	void servoAction();
	void musicAction();
	void ledAction();
  void sonarAction();
	void stop();

private:
	Controller* controller;
  	bool isServoSwapped;
  	bool isMotorSwapped;
    int steps[3];
    int initColor[3];
    int endColor[3];
    int currentColor[3];

    int distance;
	int consecutive;
	int threshold;
	int obstacleFound;
};

#endif // ANGER_H_
