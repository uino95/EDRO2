#include "search.h"

Search::Search(Controller* controller, unsigned long start){

	Serial.println("enter Search state");

  this->controller = controller;

	this->emotion_duration = 10000;
	this->emotion_started = start;

  this->motor_interval = 50;
  this->sonar_interval = 25;
  this->servo_interval = 0;
  this->led_interval = 0;
  this->music_interval = 0;

  this->motor_last_millis = 0;
  this->servo_last_millis = 0;
  this->music_last_millis = 0;
  this->led_last_millis = 0;
  this->sonar_last_millis = 0;

  this->isMotorSwapped = 1;

  this->threshold = 50;
  this->obstacleFound = 1;
  controller->next_emotion = 5; //default the next emotion is setted to sadness;
}

Search::~Search(){
	Serial.println("exiting Search state");
}

void Search::motorAction(){
  Serial.println("motor action search");
  //go backward for 500 millisecond in order to avoid obstacle
  if(millis() - this->emotion_started < 1000){
    Serial.println("going backward");
    controller->motor[0]->reverse(200);
    controller->motor[1]->reverse(200); 
    this->motor_interval = 1000;
  } else if (millis() - this->emotion_started >= 1000 && millis() - this->emotion_started < 1500){ 
    Serial.println("turning right");
    controller->motor[0]->forward(200);
    controller->motor[1]->reverse(200); 
    this->motor_interval = 1500;
  } else if (millis() - this->emotion_started >= 1500){
    Serial.println("go straight in that direction until an obstacle is found or the search period is finished");
    controller->motor[0]->forward(200);
    controller->motor[1]->forward(200); 
    this->motor_interval = this->emotion_duration;
  }
  this->motor_last_millis = millis();

  //code to do the 8 shape
  // } else {
  //   if (isMotorSwapped){
  //     //turn left for rotation seconds
  //     controller->motor[0]->reverse(150);
  //     controller->motor[1]->forward(255);
  //     isMotorSwapped = 0;
  //   }
  //   else {
  //     //turn right for rotaiton seconds
  //     controller->motor[0]->forward(255);
  //     controller->motor[1]->reverse(150);
  //     isMotorSwapped = 1;
  //   }
  //   this->motor_interval = 2500 + rotation;
  //   this->motor_last_millis = 0;
  // }
}

void Search::servoAction(){
  if(millis() - this->emotion_started < 1000){
    controller->servo[0]->close(80);
    controller->servo[1]->close(80);
    controller->servo[2]->close(80);
    this->servo_interval = 1000;
  }
  else {
    controller->servo[0]->move(20,80);
    controller->servo[1]->move(30,80);
    controller->servo[2]->move(30,80);
    this->servo_interval = this->emotion_duration;
  }
  this->servo_last_millis = millis();
}
	
void Search::musicAction(){
  this->music_last_millis = millis();
  this->music_interval = this->emotion_duration;
}

void Search::ledAction(){
  this->led_last_millis = millis();
  this->led_interval = this->emotion_duration;
  controller->led->light(150,150,150);
}

void Search::sonarAction(){
  this->sonar_last_millis = millis();
  this->distance = controller->sonar->computeDistance();
  if(this->distance < this->threshold && this->distance > 0){
    this->consecutive ++;
    if (this->consecutive = this->obstacleFound){
      //change in joy
      controller->next_emotion = 1;
      this->emotion_started = millis() + this->emotion_duration;
    }
  } else {
    this->consecutive = 0;
  }
}

void Search::stop(){
  controller->motor[0]->stop();
  controller->motor[1]->stop();
  controller->led->light(0,0,0);
	delete this;
}
