#include "sumovore.h"
#include "motor_control.h"
#define SPEED medium


void spin_left(void);
void turn_left(void);
void straight_fwd(void);
void turn_right(void);
void spin_right(void);

void motor_control(void)
{
     // very simple motor control
     
     if ( SeeLine.b.Center ) straight_fwd();
     else if (SeeLine.b.Left) spin_left();
     else if (SeeLine.b.CntLeft) turn_left();
     else if (SeeLine.b.CntRight) turn_right();
     else if (SeeLine.b.Right) spin_right();

     if ( (SeeLine.B ) == 0b00000u) motors_brake_all();
}


void spin_left(void)
{
  set_motor_speed(left, rev_fast, 0); 
  set_motor_speed(right, SPEED, 0); 
}

void turn_left(void)
{
  set_motor_speed(left, stop, 0); 
  set_motor_speed(right, fast, 0); 
}
void straight_fwd(void)
{
  set_motor_speed(left, SPEED, 0); 
  set_motor_speed(right, SPEED, 0); 
}
void spin_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, rev_fast, 0); 
}
void turn_right(void)
{
  set_motor_speed(left, fast, 0); 
  set_motor_speed(right, stop, 0); 
}