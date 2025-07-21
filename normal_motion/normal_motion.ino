#include "Mouse.h"

#define DELAY 30 // motion sending cycle
#define DEAD_THRES 20 // threshold to ignore small motion
#define SENSITIVITY 500 // adjust sensitivity as needed

#define DOF 6
#define TX 0 // translation X
#define TY 1 // translation Y
#define TZ 2 // translation Z
#define RX 3 // rotation X
#define RY 4 // rotation Y
#define RZ 5 // rotation Z

// ports of analog input for joysticks
int port[DOF] = {A0, A2, A4, A1, A3, A5};

// conversion matrix from sensor input to rigid motion
int coeff[DOF][DOF] = {
  { 0,  0,  0,-10,-10, 20}, // TX
  { 0,  0,  0,-17, 17,  0}, // TY
  {-3, -3, -3,  0,  0,  0}, // TZ
  {-6,  6,  0,  0,  0,  0}, // RY
  { 3,  3, -6,  0,  0,  0}, // RX
  { 0,  0,  0,  3,  3,  3}, // RZ
};

int origin[DOF]; // initial sensor values

void setup() {
  Mouse.begin();
  delay(300);
  for(int i = 0; i < DOF; i++) {
    origin[i] = analogRead(port[i]);
  }
}

void loop() {
  int sv[DOF]; // sensor value
  int mv[DOF]; // motion vector
  int moveX = 0, moveY = 0;

  for(int i = 0; i < DOF; i++) {
    sv[i] = analogRead(port[i]) - origin[i];
  }

  for(int i = 0; i < DOF; i++) {
    mv[i] = 0;
    for(int j = 0; j < DOF; j++) {
      mv[i] += coeff[i][j] * sv[j];
    }
    mv[i] /= SENSITIVITY;
  }

  // Map joystick movements to mouse movements
  moveX = mv[TX];
  moveY = mv[TY];

  // Move the mouse
  Mouse.move(moveX, moveY);

  // Add a small delay to reduce serial output
  delay(DELAY);
}
