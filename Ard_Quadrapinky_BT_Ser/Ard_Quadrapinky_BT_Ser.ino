#include <Servo.h>
#include <SoftwareSerial.h>

#define SERIAL_BEGIN 9600
#define PIN_BLUETOOTH_RX 11
#define PIN_BLUETOOTH_TX 12
SoftwareSerial mySerial(PIN_BLUETOOTH_RX, PIN_BLUETOOTH_TX);

#define PIN_LEG_LEFT_BACK_HORIZONTAL 17
#define PIN_LEG_LEFT_BACK_VERTICAL 16
#define PIN_LEG_LEFT_FRONT_HORIZONTAL 15
#define PIN_LEG_LEFT_FRONT_VERTICAL 14
#define PIN_LEG_RIGHT_FRONT_HORIZONTAL 5
#define PIN_LEG_RIGHT_FRONT_VERTICAL 4
#define PIN_LEG_RIGHT_BACK_HORIZONTAL 3
#define PIN_LEG_RIGHT_BACK_VERTICAL 2

#define LEG_LEFT_BACK_HORIZONTAL 0
#define LEG_LEFT_BACK_VERTICAL 1
#define LEG_LEFT_FRONT_HORIZONTAL 2
#define LEG_LEFT_FRONT_VERTICAL 3
#define LEG_RIGHT_FRONT_HORIZONTAL 4
#define LEG_RIGHT_FRONT_VERTICAL 5
#define LEG_RIGHT_BACK_HORIZONTAL 6
#define LEG_RIGHT_BACK_VERTICAL 7


#define TIMEOUT 3000

#define FORWARD 'W'
#define BACKWARD 'S'
#define LEFT 'D'
#define RIGHT 'A'
#define STAND 'F'
#define STANDD 'G'
#define SLEEP 'K'
#define NACL_VNIZ 'E'
//#define NACL_VVERH 'R'

#define ROBOT_SLEEP 'R'
#define SPEED_SLOW 'Z'
#define SPEED_MEDIUM 'Y'
#define SPEED_FAST 'X'

#define SPEED_PAUSE_SLOW 500
#define SPEED_PAUSE_MEDIUM 200
#define SPEED_PAUSE_FAST 100

//#define LEG_0 'k'
//#define LEG_1 'i'
//#define LEG_2 'o'
//#define LEG_3 'l'


#define CONTROL_OFF 'P'
#define CONTROL_ON  'M'

char comands [11] = {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  STAND,
  STANDD,
  SLEEP,
  NACL_VNIZ,
  ROBOT_SLEEP,
  CONTROL_OFF,
  CONTROL_ON,
};

int k=90;
int j=90;

typedef struct {
  int index;
  int degree;
  int delay;
} angle_t;

angle_t *sleep = (angle_t []) {
  {LEG_LEFT_BACK_HORIZONTAL, 85, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 85, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 85, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 85, 0},
  {LEG_LEFT_BACK_VERTICAL, 110, 0},
  {LEG_LEFT_FRONT_VERTICAL, 70, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 110, 0},
  {LEG_RIGHT_BACK_VERTICAL, 70, 150},
  { -1, -1, -1}
};

angle_t *nacl_vniz = (angle_t []) {
  {LEG_LEFT_BACK_HORIZONTAL, 125, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 55, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 125, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 55, 0},  
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 90, 150},  
  { -1, -1, -1}
};

angle_t *leg_1 = (angle_t []) {
  {LEG_LEFT_FRONT_HORIZONTAL, 10, 2000},
  {LEG_LEFT_FRONT_HORIZONTAL, 110, 2000},
  {LEG_LEFT_FRONT_VERTICAL, 70, 2000},
  {LEG_LEFT_FRONT_VERTICAL, 140, 2000},
  { -1, -1, -1}
};

angle_t *leg_2 = (angle_t []) {
  {LEG_RIGHT_FRONT_HORIZONTAL, 70, 2000},
  {LEG_RIGHT_FRONT_HORIZONTAL, 170, 2000},
  {LEG_RIGHT_FRONT_VERTICAL, 120, 2000},
  {LEG_RIGHT_FRONT_VERTICAL, 50, 2000},
  { -1, -1, -1}
};

angle_t *leg_3 = (angle_t []) {
  {LEG_RIGHT_BACK_HORIZONTAL, 10, 2000},
  {LEG_RIGHT_BACK_HORIZONTAL, 110, 2000},
  {LEG_RIGHT_BACK_VERTICAL, 70, 2000},
  {LEG_RIGHT_BACK_VERTICAL, 140, 2000},
  { -1, -1, -1}
};

angle_t *forward = (angle_t []) {
  {LEG_LEFT_BACK_HORIZONTAL, 140, 0},
  {LEG_LEFT_BACK_VERTICAL, 60, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 40, 0},
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 130, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 60, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 55, 0},
  {LEG_RIGHT_BACK_VERTICAL, 90, 200},
  {LEG_RIGHT_FRONT_HORIZONTAL, 90, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 95, 200},
  {LEG_LEFT_BACK_VERTICAL, 90, 0},
  {LEG_LEFT_FRONT_VERTICAL, 120, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 110, 0},
  {LEG_RIGHT_BACK_VERTICAL, 120, 200},
  {LEG_LEFT_BACK_HORIZONTAL, 80, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 90, 0},
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 125, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 55, 200},
  { -1, -1, -1}
};

angle_t *left = (angle_t []) {
  //{LEG_LEFT_FRONT_VERTICAL, 120, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 130, 0},
  {LEG_RIGHT_BACK_VERTICAL, 130, 100},
  {LEG_LEFT_FRONT_HORIZONTAL, 100, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 100, 100},
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_BACK_VERTICAL, 90, 100},
  {LEG_LEFT_BACK_VERTICAL, 50, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 30, 100},
  {LEG_LEFT_FRONT_HORIZONTAL, 55, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 55, 100},
  {LEG_LEFT_BACK_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 90, 100},
  { -1, -1, -1}
};

angle_t *right = (angle_t []) {
  {LEG_LEFT_BACK_VERTICAL, 50, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 50, 100},
  {LEG_LEFT_BACK_HORIZONTAL, 50, 30}, // 0 т.к поворот направо невозможен даже в "в воздухе" градусная мера смещена на 30 градусов
  {LEG_RIGHT_FRONT_HORIZONTAL, 50, 70}, //// 100 градусная мера смещена для синхронности
  {LEG_LEFT_BACK_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 90, 100},
  {LEG_LEFT_FRONT_VERTICAL, 120, 0},
  {LEG_RIGHT_BACK_VERTICAL, 120, 100},
  {LEG_LEFT_BACK_HORIZONTAL, 125, 30}, // 0 т.к поворот направо невозможен даже в "в воздухе" градусная мера смещена на 30 градусов
  {LEG_RIGHT_FRONT_HORIZONTAL, 125, 70}, //// 100 градусная мера смещена для синхронности
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_BACK_VERTICAL, 90, 100},
  { -1, -1, -1}
};

angle_t *backward = (angle_t []) {
  {LEG_LEFT_BACK_HORIZONTAL, 125, 0},
  {LEG_LEFT_BACK_VERTICAL, 60, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 55, 0},
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 130, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 60, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 55, 0},
  {LEG_RIGHT_BACK_VERTICAL, 90, 200},
  {LEG_LEFT_BACK_HORIZONTAL, 80, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 90, 200},
  {LEG_LEFT_BACK_VERTICAL, 90, 0},
  {LEG_LEFT_FRONT_VERTICAL, 120, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_BACK_VERTICAL, 120, 200},
  {LEG_LEFT_BACK_HORIZONTAL, 125, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 55, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 90, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 95, 200},
  { -1, -1, -1}
};

angle_t *stand = (angle_t []) {
  {LEG_LEFT_BACK_HORIZONTAL, 125, 0},
  {LEG_LEFT_FRONT_HORIZONTAL, 55, 0},
  {LEG_RIGHT_FRONT_HORIZONTAL, 125, 0},
  {LEG_RIGHT_BACK_HORIZONTAL, 55, 0},
  {LEG_LEFT_BACK_VERTICAL, 90, 0},
  {LEG_LEFT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_FRONT_VERTICAL, 90, 0},
  {LEG_RIGHT_BACK_VERTICAL, 90, 150},
  { -1, -1, -1}
};

void locomotion(angle_t angles[]);

Servo servo[8];
unsigned long cur_time, prev_time;
char action = '\0';
char prev_action = '\0';
boolean auto_mode = false; // Сменить на true, есле нужен авто-режим сразу после включения
unsigned long lastReceivedTime; // Время последнего принятого действия

static int stepCount = 0;
static int stepTotal = 0;
static int turnCount = 0;
static int speedPause = SPEED_PAUSE_MEDIUM;

void setup() {
  Serial.begin(SERIAL_BEGIN); 
  mySerial.begin(SERIAL_BEGIN);
  
  servo[LEG_LEFT_BACK_HORIZONTAL].attach(PIN_LEG_LEFT_BACK_HORIZONTAL);
  servo[LEG_LEFT_BACK_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_LEFT_BACK_VERTICAL].attach(PIN_LEG_LEFT_BACK_VERTICAL);
  servo[LEG_LEFT_BACK_VERTICAL].write(90);
  delay(speedPause);
  
  servo[LEG_LEFT_FRONT_HORIZONTAL].attach(PIN_LEG_LEFT_FRONT_HORIZONTAL);
  servo[LEG_LEFT_FRONT_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_LEFT_FRONT_VERTICAL].attach(PIN_LEG_LEFT_FRONT_VERTICAL);
  servo[LEG_LEFT_FRONT_VERTICAL].write(90);
  delay(speedPause);
  
  servo[LEG_RIGHT_FRONT_HORIZONTAL].attach(PIN_LEG_RIGHT_FRONT_HORIZONTAL);
  servo[LEG_RIGHT_FRONT_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_RIGHT_FRONT_VERTICAL].attach(PIN_LEG_RIGHT_FRONT_VERTICAL);
  servo[LEG_RIGHT_FRONT_VERTICAL].write(90);
  delay(speedPause);
  
  servo[LEG_RIGHT_BACK_HORIZONTAL].attach(PIN_LEG_RIGHT_BACK_HORIZONTAL);
  servo[LEG_RIGHT_BACK_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_RIGHT_BACK_VERTICAL].attach(PIN_LEG_RIGHT_BACK_VERTICAL);
  servo[LEG_RIGHT_BACK_VERTICAL].write(90);
  delay(speedPause);
   
  locomotion(stand);
  randomSeed(analogRead(0));
  cur_time = prev_time = millis();
  prev_action = action = SLEEP; //Было FORWARD

  lastReceivedTime = millis(); // Инициализируем время последнего принятого действия
  delay(TIMEOUT);
}

void waySqware(){
  if (stepCount < 5) {
      action = FORWARD;  // Идём вперед
      locomotion(forward);
      stepCount++;
      stepTotal++;
    } else {
      action = LEFT;  // Поворот налево после 5 шагов
      locomotion(left);
      turnCount++;
    }

    if (turnCount == 8) {
      stepCount = 0;  // Сбрасываем повороты после прохождения квадрата
      turnCount = 0;
    }

    if (stepTotal == 20){
       auto_mode = false;  // Останавливаемся после завершения маршрута квадрата
       prev_action = action = SLEEP;
    }

    delay(300);  
}

void robot_sleep(){
  locomotion(stand);
  prev_action = action = SLEEP;
}

void robot_stand(){
  servo[LEG_LEFT_BACK_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_LEFT_BACK_VERTICAL].write(90);
  delay(speedPause);
  
  servo[LEG_LEFT_FRONT_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_LEFT_FRONT_VERTICAL].write(90);
  delay(speedPause);
  
  servo[LEG_RIGHT_FRONT_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_RIGHT_FRONT_VERTICAL].write(90);
  delay(speedPause);
  
  servo[LEG_RIGHT_BACK_HORIZONTAL].write(85);
  delay(speedPause);
  
  servo[LEG_RIGHT_BACK_VERTICAL].write(90);
  delay(speedPause);
  locomotion(stand);
}

void loop() {
  if (mySerial.available () > 0) {
    action = mySerial.read();           
    
    switch (action) {
      case CONTROL_ON: auto_mode = false; action = STAND;
        break;
      case CONTROL_OFF: auto_mode = true; action = STAND;
        break;
    }
  }

  if (auto_mode) {
    waySqware();
  }

  if (1) {
    switch (action) {
      case SPEED_SLOW:
      	speedPause = SPEED_PAUSE_SLOW;
        action = prev_action;
        break;
      case SPEED_MEDIUM:
        speedPause = SPEED_PAUSE_MEDIUM;
        action = prev_action;
        break;
      case SPEED_FAST:
      	speedPause = SPEED_PAUSE_FAST;
        action = prev_action;
        break;
     }
    if(!checkContains(comands, sizeof(comands)/sizeof(comands[0]), action)){
        action = prev_action;
    }
    switch (action) {
      case FORWARD: if (action != prev_action) {
          prev_action = action;
        }
        locomotion(forward);
        break;
      case BACKWARD: if (action != prev_action) {
          prev_action = action;
        }
        locomotion(backward);
        break;
        
      case LEFT: if (action != prev_action) {
          prev_action = action;
        }
        locomotion(left);
        break;
        
      case RIGHT: if (action != prev_action) {
          prev_action = action;
        }
        locomotion(right);
        break;
       
       case SLEEP: if (action != prev_action) {
          prev_action = action;
          locomotion(stand);
        }
        locomotion(sleep);        
        break;
        
      case STAND:
        robot_stand();
        action = '\0';
        prev_action = '\0';
        break;

     case ROBOT_SLEEP:
        robot_sleep();
        break;
    }
  }

}

bool checkContains(char commands[], int size, char checkSymbol) {
    for (int i = 0; i < size; i++) {
        if (commands[i] == checkSymbol) {
            return true; 
        }
    }
    return false;
}

void locomotion(angle_t angles[]) {
  int i = 0;
  while (1) {
    if (angles[i].degree == -1 ||
        angles[i].index == -1 ||
        angles[i].delay == -1) {
      break;
    }
    servo[angles[i].index].write(angles[i].degree);
    if (angles[i].delay > 0) delay(angles[i].delay);
    i++;
   
     // Serial.print("loop ");
     // Serial.print(i);
     // Serial.print("-");
     // Serial.print(angles[i].index);
     // Serial.print(" ");
     // Serial.print(angles[i].degree);
     // Serial.print(" ");
     // Serial.print(angles[i].delay);
     // Serial.println();
    
  }
  delay(speedPause);
}
