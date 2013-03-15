//robot_test.c
#include "sumovore.h"
#include "motor_control.h"
#include "my_delay_functions.h"

void robot_test_left(void)
{
	set_motor_speed(left, slow, 0);
	delay_1S(1);
	set_motor_speed(left, medium, 0); 
	delay_1S(1);
	set_motor_speed(left, fast, 0);
	delay_1S(1);
	set_motor_speed(left, stop, 0);
	delay_1S(1);
	set_motor_speed(left, rev_slow, 0);
	delay_1S(1);
	set_motor_speed(left, rev_medium, 0);
	delay_1S(1);
	set_motor_speed(left, rev_fast, 0);
	delay_1S(1);
	set_motor_speed(left, stop, 0);
}

void robot_test_right(void)
{
	set_motor_speed(right, slow, 0);
	delay_1S(1);
	set_motor_speed(right, medium, 0); 
	delay_1S(1);
	set_motor_speed(right, fast, 0);
	delay_1S(1);
	set_motor_speed(right, stop, 0);
	delay_1S(1);
	set_motor_speed(right, rev_slow, 0);
	delay_1S(1);
	set_motor_speed(right, rev_medium, 0);
	delay_1S(1);
	set_motor_speed(right, rev_fast, 0);
	delay_1S(1);
	set_motor_speed(right, stop, 0);
}