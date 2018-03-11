// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <AFMotor.h>
#include <Servo.h> 
#include <Wire.h>

class MegatronCfg{
public:
  MegatronCfg()
    : left_forward_speed(default_speed)
    , left_backward_speed(default_speed)
    , right_forward_speed(default_speed)
    , right_backward_speed(default_speed)
    , left_led_light(default_light)
    , right_led_light(default_light)
    , horizontal_min(default_h_min)
    , horizontal_max(default_h_max)
    , vertical_min(default_v_min)
    , vertical_max(default_v_max)
    , v_step(default_step)
    , h_step(default_step)
    , factor(default_factor)
  {
    current_horizontal = (default_h_min + default_h_max)/2;
    current_vertical = (default_v_min + default_h_max)/2;
  }

  ~MegatronCfg()
  {}
  
  void restore() {
    left_forward_speed = default_speed;
    left_backward_speed = default_speed;
    right_forward_speed = default_speed;
    right_backward_speed = default_speed;
    left_led_light = default_light;
    right_led_light = default_light;
    horizontal_min = default_h_min;
    horizontal_max = default_h_max;
    vertical_min = default_v_min;
    vertical_max = default_v_max;
    current_horizontal = (default_h_min + default_h_max)/2;
    current_vertical = (default_v_min + default_h_max)/2;
    v_step = default_step;
    h_step = default_step;
    factor = default_factor;    
  }
  
  const static unsigned char default_speed = 0xff;
  const static unsigned char default_light = 50;
  const static unsigned char default_step = 5;
  const static unsigned char default_factor = 4;
  const static unsigned char default_h_min = 20;
  const static unsigned char default_h_max = 180;
  const static unsigned char default_v_min = 40;
  const static unsigned char default_v_max = 200;
  
  unsigned char left_forward_speed;
  unsigned char right_forward_speed;
  unsigned char left_backward_speed;
  unsigned char right_backward_speed;

  unsigned char left_led_light;
  unsigned char right_led_light;

  unsigned char horizontal_max, horizontal_min; // min means right, max mean left
  unsigned char vertical_max, vertical_min; // min means up, max means down

  unsigned char current_horizontal;
  unsigned char current_vertical;

  unsigned char v_step, h_step;
  unsigned char factor;
};

MegatronCfg m_cfg;

AF_DCMotor _left_motor(3);
AF_DCMotor _right_motor(1);
AF_DCMotor _left_led(4);
AF_DCMotor _right_led(2);

Servo _horizontal_servo;
Servo _vertical_servo;

void light_up_level0(unsigned int times) {
  _left_led.setSpeed(m_cfg.left_led_light);
  _right_led.setSpeed(m_cfg.right_led_light);
  if (times == 0){
     _left_led.run(FORWARD);
     _right_led.run(FORWARD);
  } else {
    _left_led.run(FORWARD);
    _right_led.run(FORWARD);
    delay(times << m_cfg.factor);
    _left_led.run(RELEASE);
    _right_led.run(RELEASE);
  }
}

void light_up_level1(unsigned int times) {
  _left_led.setSpeed(m_cfg.left_led_light);
  _right_led.setSpeed(m_cfg.right_led_light);
  if (times == 0){
     _left_led.run(BACKWARD);
     _right_led.run(BACKWARD);
  } else {
    _left_led.run(BACKWARD);
    _right_led.run(BACKWARD);
    delay(times << m_cfg.factor);
    _left_led.run(RELEASE);
    _right_led.run(RELEASE);
  }
}

void light_down(unsigned int times) {
  _left_led.run(RELEASE);
  _right_led.run(RELEASE);
}

void move_forward(unsigned int times){
  if (times == 0){
    _left_motor.run(FORWARD);
    _right_motor.run(FORWARD);
  } else {
    _left_motor.setSpeed(m_cfg.left_forward_speed);
    _right_motor.setSpeed(m_cfg.right_forward_speed);
    _left_motor.run(FORWARD);
    _right_motor.run(FORWARD);
    delay(times <<m_cfg.factor);
    _left_motor.run(RELEASE);
    _right_motor.run(RELEASE);
  }
}

void move_backward(unsigned int times){
  if (times == 0) {
    _left_motor.run(BACKWARD);
    _right_motor.run(BACKWARD);
  } else {
    _left_motor.setSpeed(m_cfg.left_backward_speed);
    _right_motor.setSpeed(m_cfg.right_backward_speed);
    _left_motor.run(BACKWARD);
    _right_motor.run(BACKWARD);
    delay(times <<m_cfg.factor);
    _left_motor.run(RELEASE);
    _right_motor.run(RELEASE);
  }
}

void move_left(unsigned int times){
  if (times == 0) {
    _left_motor.run(BACKWARD);
    _right_motor.run(FORWARD);
  } else {
    _left_motor.setSpeed(m_cfg.left_backward_speed);
    _right_motor.setSpeed(m_cfg.right_forward_speed);
    _left_motor.run(BACKWARD);
    _right_motor.run(FORWARD);
    delay(times <<m_cfg.factor);
    _left_motor.run(RELEASE);
    _right_motor.run(RELEASE);
  }
}

void move_right(unsigned int times){
  if (times == 0) {
    _left_motor.run(FORWARD);
    _right_motor.run(BACKWARD);
  } else {
    _left_motor.setSpeed(m_cfg.left_forward_speed);
    _right_motor.setSpeed(m_cfg.right_backward_speed);
    _left_motor.run(FORWARD);
    _right_motor.run(BACKWARD);
    delay(times <<m_cfg.factor);
    _left_motor.run(RELEASE);
    _right_motor.run(RELEASE);
  }
}

void servo_up(unsigned int times) {
  if(times == 0) {
    if (m_cfg.current_vertical - m_cfg.v_step >=m_cfg.vertical_min) {
      m_cfg.current_vertical -= m_cfg.v_step;
    }
  } else {
    if(times < m_cfg.vertical_min)
      times = m_cfg.vertical_min;
    if (times > m_cfg.vertical_max)
      times = m_cfg.vertical_max;
    m_cfg.current_vertical = times;
  }
  _vertical_servo.write(m_cfg.current_vertical);
}

void servo_down(unsigned int times) {
  if (times == 0) {
    if (m_cfg.current_vertical + m_cfg.v_step <= m_cfg.vertical_max) {
      m_cfg.current_vertical += m_cfg.v_step;
    }
  } else {
    if(times < m_cfg.vertical_min)
      times = m_cfg.vertical_min;
    if (times > m_cfg.vertical_max)
      times = m_cfg.vertical_max;
    m_cfg.current_vertical = times;
  }
  _vertical_servo.write(m_cfg.current_vertical);
}

void servo_left(unsigned int times) {
  if (times == 0) {
    if (m_cfg.current_horizontal + m_cfg.h_step <= m_cfg.horizontal_max) {
      m_cfg.current_horizontal += m_cfg.h_step;
    }
  } else {
    if(times < m_cfg.horizontal_min)
      times = m_cfg.horizontal_min;
    if (times > m_cfg.horizontal_max)
      times = m_cfg.horizontal_max;
    m_cfg.current_horizontal = times;
  }
  _horizontal_servo.write(m_cfg.current_horizontal);
}

void servo_right(unsigned int times) {
  if (times == 0) {
    if (m_cfg.current_horizontal - m_cfg.h_step >= m_cfg.horizontal_min) {
      m_cfg.current_horizontal -= m_cfg.h_step;
    }
  } else {
    if(times < m_cfg.horizontal_min)
      times = m_cfg.horizontal_min;
    if (times > m_cfg.horizontal_max)
      times = m_cfg.horizontal_max;
    m_cfg.current_horizontal = times;
  }
  _horizontal_servo.write(m_cfg.current_horizontal);
}

void halt(unsigned int times) {
  _left_motor.run(RELEASE);
  _right_motor.run(RELEASE);
  _left_motor.run(RELEASE);
  _right_motor.run(RELEASE);
}

void reset(unsigned int times) {
  m_cfg.current_horizontal =   (m_cfg.horizontal_max + m_cfg.horizontal_min ) / 2;
  m_cfg.current_vertical =  (m_cfg.vertical_max + m_cfg.vertical_min ) / 2;
  _horizontal_servo.write(m_cfg.current_horizontal);
  _vertical_servo.write(m_cfg.current_vertical);

  _left_motor.setSpeed(m_cfg.left_forward_speed);
  _right_motor.setSpeed(m_cfg.right_forward_speed);
  _left_led.setSpeed(m_cfg.left_led_light);
  _right_led.setSpeed(m_cfg.right_led_light);
  _left_motor.run(RELEASE);
  _right_motor.run(RELEASE);
  _left_led.run(RELEASE);
  _right_led.run(RELEASE);
}

unsigned char flush_data = 250;

// !!!TODO there is a bug here , Once I can't read numbers greater than 127, It seems to be a hardware connection bug!!
void prepare_flush_data(unsigned int times){
  switch(times){
    case 1: flush_data = m_cfg.left_forward_speed; break;
    case 2: flush_data = m_cfg.left_backward_speed; break;
    case 3: flush_data = m_cfg.right_forward_speed; break;
    case 4: flush_data = m_cfg.right_backward_speed; break;
    case 5: flush_data = m_cfg.horizontal_min; break;
    case 6: flush_data = m_cfg.horizontal_max; break;
    case 7: flush_data = m_cfg.vertical_min; break;
    case 8: flush_data = m_cfg.vertical_max; break;
    case 9: flush_data = m_cfg.current_horizontal; break;
    case 10: flush_data = m_cfg.current_vertical; break;
    case 11: flush_data = m_cfg.v_step; break;
    case 12: flush_data = m_cfg.h_step; break;
    case 13: flush_data = m_cfg.factor; break;
    case 14: flush_data = m_cfg.left_led_light; break;
    case 15: flush_data = m_cfg.right_led_light; break;
    default: flush_data = 0xff; break;    
  }
}

// next two op is a pair of update var ops
void update_flush_data(unsigned int times) {
  flush_data =  (unsigned char)times;
}

void update_m_cfg(unsigned int times) {
  switch(times){
    case 0: m_cfg.restore(); break;
    case 1: m_cfg.left_forward_speed = flush_data; break;
    case 2: m_cfg.left_backward_speed = flush_data; break;
    case 3: m_cfg.right_forward_speed = flush_data; break;
    case 4: m_cfg.right_backward_speed = flush_data; break;
    case 5: m_cfg.horizontal_min = flush_data; break;
    case 6: m_cfg.horizontal_max = flush_data; break;
    case 7: m_cfg.vertical_min = flush_data; break;
    case 8: m_cfg.vertical_max = flush_data; break;
    case 9: m_cfg.current_horizontal = flush_data; break;
    case 10: m_cfg.current_vertical = flush_data; break;
    case 11: m_cfg.v_step = flush_data; break;
    case 12: m_cfg.h_step = flush_data; break;
    case 13: m_cfg.factor = flush_data; break;
    case 14: m_cfg.left_led_light = flush_data; break;
    case 15: m_cfg.right_led_light = flush_data; break;
    default: flush_data = 0xfe; break;    
  }
}

unsigned char c_code = 0x00;
unsigned char c_data = 0x00;
bool c_flag = false;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  
  _vertical_servo.attach(10);
  _horizontal_servo.attach(9);
  reset(0);
}


typedef void (*Opfunc)(unsigned int);
Opfunc ops[16] = {reset, prepare_flush_data, update_flush_data, update_m_cfg, move_forward, move_backward, move_left, move_right,
                servo_up, servo_down, servo_left, servo_right, halt, light_up_level0, light_up_level1, light_down};

void loop() {
  if (c_flag) {
    c_flag = false;
    Serial.println((int)c_code);
    
    if (c_code <= 0x0f){
      ops[c_code](c_data);
    }
  }
}

void requestEvent() {
  Wire.write(flush_data); // respond with flush_data
  Serial.println((int)flush_data);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    c_code = Wire.read(); // receive byte as a character
    c_flag = true;
  }
  c_data = Wire.read();    // receive byte as an integer
  
}

