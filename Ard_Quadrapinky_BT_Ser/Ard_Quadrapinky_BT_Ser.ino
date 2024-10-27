#include <Servo.h>
#include <SoftwareSerial.h>
// TX на блютуз к пину RX на ардуино
// RX на блютуз к пину TX на ардуино
SoftwareSerial mySerial(11, 12); // RX, TX Пины для блютуз модуля мижно изменить

#define TIMEOUT 5000

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
  {0, 85, 0},
  {2, 85, 0},
  {4, 85, 0},
  {6, 85, 0},
  {1, 110, 0},
  {3, 70, 0},
  {5, 110, 0},
  {7, 70, 150},
  { -1, -1, -1}
};

angle_t *nacl_vniz = (angle_t []) {
  {0, 125, 0},
  {2, 55, 0},
  {4, 125, 0},
  {6, 55, 0},  
  {3, 90, 0},
  {5, 90, 150},  
  { -1, -1, -1}
};

angle_t *leg_1 = (angle_t []) {
  {2, 10, 2000},
  {2, 110, 2000},
  {3, 70, 2000},
  {3, 140, 2000},
  { -1, -1, -1}
};

angle_t *leg_2 = (angle_t []) {
  {4, 70, 2000},
  {4, 170, 2000},
  {5, 120, 2000},
  {5, 50, 2000},
  { -1, -1, -1}
};

angle_t *leg_3 = (angle_t []) {
  {6, 10, 2000},
  {6, 110, 2000},
  {7, 70, 2000},
  {7, 140, 2000},
  { -1, -1, -1}
};

angle_t *forward = (angle_t []) {
  {0, 140, 0},
  {1, 60, 0},
  {2, 40, 0},
  {3, 90, 0},
  {4, 130, 0},
  {5, 60, 0},
  {6, 55, 0},
  {7, 90, 200},
  {4, 90, 0},
  {6, 95, 200},
  {1, 90, 0},
  {3, 120, 0},
  {5, 110, 0},
  {7, 120, 200},
  {0, 80, 0},
  {2, 90, 0},
  {3, 90, 0},
  {4, 125, 0},
  {6, 55, 200},
  { -1, -1, -1}
};

angle_t *left = (angle_t []) {
  //{3, 120, 0},
  {4, 130, 0},
  {7, 130, 100},
  {2, 100, 0},
  {6, 100, 100},
  {3, 90, 0},
  {7, 90, 100},
  {1, 50, 0},
  {5, 30, 100},
  {2, 55, 0},
  {6, 55, 100},
  {1, 90, 0},
  {5, 90, 100},
  { -1, -1, -1}
};

angle_t *right = (angle_t []) {
  {1, 50, 0},
  {5, 50, 100},
  {0, 50, 0},
  {4, 50, 100},
  {1, 90, 0},
  {5, 90, 100},
  {3, 120, 0},
  {7, 120, 100},
  {0, 125, 0},
  {4, 125, 100},
  {3, 90, 0},
  {7, 90, 100},
  { -1, -1, -1}
};

angle_t *backward = (angle_t []) {
  {0, 125, 0},
  {1, 60, 0},
  {2, 55, 0},
  {3, 90, 0},
  {4, 130, 0},
  {5, 60, 0},
  {6, 55, 0},
  {7, 90, 200},
  {0, 80, 0},
  {2, 90, 200},
  {1, 90, 0},
  {3, 120, 0},
  {5, 90, 0},
  {7, 120, 200},
  {0, 125, 0},
  {2, 55, 0},
  {4, 90, 0},
  {6, 95, 200},
  { -1, -1, -1}
};

angle_t *stand = (angle_t []) {
  {0, 125, 0},
  {2, 55, 0},
  {4, 125, 0},
  {6, 55, 0},
  {1, 90, 0},
  {3, 90, 0},
  {5, 90, 0},
  {7, 90, 150},
  { -1, -1, -1}
};

void locomotion(angle_t angles[]);

Servo servo[8];
unsigned long cur_time, prev_time;
char action = '\0';
char prev_action = '\0';
boolean auto_mode = false; // Сменить на true, есле нужен авто-режим сразу после включения

void setup() {
  Serial.begin(9600); 
  mySerial.begin(9600);
  
  servo[0].attach(17);
  servo[0].write(85);
  delay(200);
  
  servo[1].attach(16);
  servo[1].write(90);
  delay(200);
  
  servo[2].attach(15);
  servo[2].write(85);
  delay(200);
  
  servo[3].attach(14);
  servo[3].write(90);
  delay(200);
  
  servo[4].attach(5);
  servo[4].write(85);
  delay(200);
  
  servo[5].attach(4);
  servo[5].write(90);
  delay(200);
  
  servo[6].attach(3);
  servo[6].write(85);
  delay(200);
  
  servo[7].attach(2);
  servo[7].write(90);
  delay(200);
  
  //Serial.begin(57600);
  //mySerial.begin(57600);
   
  locomotion(stand);
  randomSeed(analogRead(0));
  cur_time = prev_time = millis();
//  prev_action = action = SLEEP; //Было FORWARD
  delay(3000);
}

void robot_sleep(){
  locomotion(stand);
  prev_action = action = SLEEP;
}

void robot_stand(){
  servo[0].write(85);
  delay(200);
  
  servo[1].write(90);
  delay(200);
  
  servo[2].write(85);
  delay(200);
  
  servo[3].write(90);
  delay(200);
  
  servo[4].write(85);
  delay(200);
  
  servo[5].write(90);
  delay(200);
  
  servo[6].write(85);
  delay(200);
  
  servo[7].write(90);
  delay(200);
  locomotion(stand);
}

void loop() {
  if (mySerial.available () > 0) {
    action = mySerial.read();
    Serial.println(action);            
    
    switch (action) {
      case CONTROL_ON: auto_mode = false; action = STAND;
        Serial.println("control on");
        break;
      case CONTROL_OFF: auto_mode = true; action = STAND;
        Serial.println("control off");
        break;
    }
  }

  if (auto_mode) {
    cur_time = millis();
    if (cur_time - prev_time >= TIMEOUT) {
      prev_time = cur_time;
      do {
        action = (int)random(FORWARD, BACKWARD + 1);
      } while (action == prev_action || action == STAND);
      Serial.println(action);
    }
  }

  if (1) {
//    action = LEFT;  
    if(!checkContains(comands, sizeof(comands)/sizeof(comands[0]), action)){
        action = prev_action;
    }
    switch (action) {
      case FORWARD: if (action != prev_action) {
          prev_action = action;
          //locomotion(stand);
          Serial.println("forward");
        }
        locomotion(forward);
        Serial.println("f forward");
        break;
      case BACKWARD: if (action != prev_action) {
          prev_action = action;
          //locomotion(stand);
          Serial.println("backward");
        }
        locomotion(backward);
        break;
        
      case LEFT: if (action != prev_action) {
          prev_action = action;
          //locomotion(stand);
          Serial.println("left");
        }
        locomotion(left);
        break;
        
      case RIGHT: if (action != prev_action) {
          prev_action = action;
          //locomotion(stand);
          Serial.println("right");
        }
        locomotion(right);
        break;

       /*
       case NACL_VNIZ: if (action != prev_action) {
         if(prev_action != NACL_VVERH){
         //locomotion(stand);
         k=90;
         j=90; 
         }          
         prev_action = action;                  
        }
        if ( k<120){
         k++;         
         servo[7].write(k);
         delay(50);                        
        }
        if ( j>60){
         j--;         
         servo[1].write(j);
         delay(50);                        
        }
        break;


       case NACL_VVERH: if (action != prev_action) {
         if(prev_action != NACL_VNIZ){
         //locomotion(stand);
         k=90;
         j=90; 
         }         
         prev_action = action;                  
       }
       if ( k>70){
        k--;        
        servo[7].write(k);
        delay(50);                        
       }
       if ( j<110){
        j++;        
        servo[1].write(j);
        delay(50);                        
       }
       break;
       */
       //case LEG_1: if (action != prev_action) {
       //   prev_action = action;
       //   locomotion(stand);
       //   //Serial.println("leg_1");
       // }
       // locomotion(leg_1);        
       // break;

       //case LEG_2: if (action != prev_action) {
       //   prev_action = action;
       //   locomotion(stand);
       //   //Serial.println("leg_2");
       // }
       // locomotion(leg_2);        
       // break;
        
       //case LEG_3: if (action != prev_action) {
       //   prev_action = action;
       //   locomotion(stand);
       //   //Serial.println("leg_3");
       // }
       // locomotion(leg_3);        
       // break;
        
       case SLEEP: if (action != prev_action) {
          prev_action = action;
          locomotion(stand);
          //Serial.println("sleep");
        }
        locomotion(sleep);        
        break;
        
      case STAND:
        robot_stand();
        //locomotion(stand);
        break;

     case ROBOT_SLEEP:
        robot_sleep();
        //locomotion(stand);
        break;
      
//      case STANDD:
//        locomotion(stand);
//        break;
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
  delay(1000);
}
