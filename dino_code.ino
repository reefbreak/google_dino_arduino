#include <Servo.h>

Servo space_servo;
Servo dn_servo;

unsigned long currentMillis = 0;
unsigned long startMillis = 0;
unsigned long previousMillis = 0;
unsigned long press_spacebar_slot1 = 0;
unsigned long press_spacebar_slot2 = 0;
unsigned long press_dn_slot1 = 0;
unsigned long press_dn_slot2 = 0;
int space_pressed_count = 0;
int dn_pressed_count = 0;
const int ldr_low = A0;
const int ldr_high = A1;
int ldr_low_val = 0;
int ldr_high_val = 0;
int ldr_low_previous = 0;
int ldr_high_previous = 0;
int ldr_low_dif = 0;
int ldr_high_dif = 0;
const int led1 = 50;
const int led2 = 51;
int led1_count = 0;
int led2_count = 0;
const int space_servo_pin = 9;
const int dn_servo_pin = 10;
const int edge_diff = 10;
const int edge_low_bounce = 100;
const int edge_high_bounce = 100;
int edge_low_count = 0;
int edge_high_count = 0;
const int led_flash_length = 3;
int edge_to_jump_length = 85;
bool space_pressed;
bool dn_pressed;
bool ldr_low_edge_detected;
bool ldr_high_edge_detected;
bool debug = true;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  space_servo.attach(space_servo_pin);
  dn_servo.attach(dn_servo_pin);
  space_pressed = false;
  dn_pressed = false;
  ldr_low_val = analogRead(ldr_low);
  ldr_high_val = analogRead(ldr_high);
  ldr_low_previous = ldr_low_val;
  ldr_high_previous = ldr_high_val;
  currentMillis = millis();
  startMillis = currentMillis;
  ldr_low_edge_detected = false;
  ldr_high_edge_detected = false;
  Serial.begin(9600);
  space_pressed_count = 50; // < auto start game
  
}

void press_keys(){

  if (press_spacebar_slot1 > 0) {
  space_pressed_count = 50 + edge_to_jump_length;
  press_spacebar_slot1 = 0;
  } 
  
  if (press_spacebar_slot2 > 0) {
  space_pressed_count = 50 + edge_to_jump_length;
  press_spacebar_slot2 = 0;
  } 
  
  if (space_pressed_count > 0) {
    if (space_pressed_count <= 50) {
     space_servo.write(0);
    }
     space_pressed_count--;
  } else {
     space_servo.write(25); 
  }

  if (press_dn_slot1 > 0) {
  dn_pressed_count = 150 + edge_to_jump_length;
  press_dn_slot1 = 0;
  } 
  
  if (press_dn_slot2 > 0) {
  dn_pressed_count = 150 + edge_to_jump_length;
  press_dn_slot2 = 0;
  } 
  
  if (dn_pressed_count > 0) {
    if (dn_pressed_count <= 150) {
     dn_servo.write(0);
    }
     dn_pressed_count--;
  } else {
     dn_servo.write(25); 
  }
  
}

void ldr_edge_detect() {
  // read ldr sensor values
  ldr_low_val = analogRead(ldr_low);
  // check for changes and flag
  ldr_low_dif = ldr_low_val - ldr_low_previous;
  ldr_low_val = ldr_low_previous;
  //check sign and invert negative
  if (ldr_low_dif >= 0 ) {
  } else {
  ldr_low_dif = -ldr_low_dif;    
  }
  //trigger based on difference from one reading to the next
  if (ldr_low_dif > edge_diff){
    if (edge_low_count == 0) {
    ldr_low_edge_detected = true;
    edge_low_count = edge_low_bounce;
    } else {
      edge_low_count--;
      ldr_low_edge_detected = false;
    }
  } else {
    ldr_low_edge_detected = false;
    edge_low_count = 0;
  }


  ldr_high_val = analogRead(ldr_high);
  ldr_high_dif = ldr_high_val - ldr_high_previous;
  if (ldr_high_dif >= 0 ){
  } else {
  ldr_high_dif = -ldr_high_dif;    
  }
  if (ldr_high_dif > edge_diff){
    ldr_high_edge_detected = true;
  } else {
    ldr_high_edge_detected = false;
  }
}

void flash_leds(){
  if (led1_count > 0) {
    digitalWrite(led1,HIGH);
    led1_count--;
  } else {
    digitalWrite(led1,LOW);
    led1_count = 0;
  }

  if (led2_count > 0) {
    digitalWrite(led1,HIGH);
    led2_count--;
  } else {
    digitalWrite(led2,LOW);
    led2_count = 0;
  }
}



// the loop function runs over and over again forever
void loop() {
  currentMillis = millis();
  ldr_edge_detect();
  flash_leds();
  press_keys();

  if (ldr_low_edge_detected == true ) {
    led1_count = led1_count + led_flash_length;
    if (press_spacebar_slot1 > 0) {
    press_spacebar_slot2 = currentMillis;
    } else {
    press_spacebar_slot1 = currentMillis;
    }
  }

  if (ldr_high_edge_detected == true ) {
    led2_count = led2_count + led_flash_length;
        if (press_dn_slot1 > 0) {
    press_dn_slot2 = currentMillis;
    } else {
    press_dn_slot1 = currentMillis;
    }
  }

  if (debug == true ) {
    Serial.println(space_pressed_count);
  }

  if ((currentMillis - startMillis) > 15000) {
    edge_to_jump_length = 65;
    //speedup
  }

}
