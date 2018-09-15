#include <Stepper.h>

#define reductionratio 64

unsigned long loopIntervalTime = (60 * 60 * 12); // 루프 인터벌 시간(초).  아두이노 우노 // max 4294967295 = (2^32-1) // 대략 50일 기록가능
unsigned long previousMillis = 0;        // will store last time LED was updated

// full 스텝은 11.25도/64, half 스텝은 5.625/64
// 기본은 full 스텝인 듯. 
const int stepsPerRevolution = 2048; // 360도 회전하려면 2048스텝이 필요.

// initialize the stepper library on pins 8 through 11:
// 배선을 "사물인터넷을 위한 아두이노" 로 변경함.
Stepper myStepper(stepsPerRevolution, 11, 9, 10, 8);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  myStepper.setSpeed(0.25 * reductionratio); // ~ 0.26 까지. 0.27 부터는 소리만 나고, 동작하지 않음.
  // initialize the serial port:
  Serial.begin(9600);

  // 스텝모터 바로 세우기 !!! - 필요할 경우만 사용
  //executeStepByAngle(-30);
  //delay(20 * 1000);
}

void executeStepByAngle(int angle) {
  int offset = angle % 360;
  if(offset == 0) {
    return;
  }
  int offsetStep = map(offset, 0, 359, 0, 2048);
  myStepper.step(offsetStep);
}

void loop() {
  // looping
  if(stepCount == 0) {
    previousMillis = millis();       
  } else {
    unsigned long currentMillis = millis();  
    if ((currentMillis - previousMillis) < (loopIntervalTime * 1000)) { // loop 간격. wifi 만 체크하고 그외 시간은 pass 한다.
      return;
    } else {
      previousMillis = currentMillis;
    }     
  }

  stepCount++;
  //Serial.println(stepCount);
  
  int loopCnt = 4;
  if(stepCount = 1) { // 최초는 1회만, 리셋을 누르면 한 번식 들어감.
     loopCnt = 1;
  }
  
  // 바퀴 +  // 한바퀴 6초 정도: speed: 150, step: 128 
  for(int i = 0; i < loopCnt; i++) {
    myStepper.step(stepsPerRevolution);
  }

  int resetYn = stepCount % 4; // 4번(하루 4번) 마다 리셋한다.
  if(resetYn > 0 ) {
    executeStepByAngle(30);
  } else {
    executeStepByAngle(-90);  
  }
}
