/******************************************************************************
 * Project    : Three-Directional Obstacle Detection System
 * Controller : AT89C51 (8051)
 * Crystal    : 11.0592 MHz
 * Sensors    : 3 × HC-SR04
 * Author     : Ganesh
 *
 * Description:
 * Measures obstacle distance in the front, left, and right directions
 * using ultrasonic sensors and Timer0. LEDs indicate the presence of
 * nearby obstacles within the predefined threshold distance.
 ******************************************************************************/
 
 
#include <reg51.h>
#include <intrins.h>

/* Port 1 */
sbit FRONT_TRIG = P1^0;
sbit FRONT_ECHO = P1^1;
sbit LEFT_TRIG  = P1^2;
sbit LEFT_ECHO  = P1^3;
sbit RIGHT_TRIG = P1^4;
sbit RIGHT_ECHO = P1^5;

/* Port 2 */
sbit LED_RIGHT = P2^0;
sbit LED_FRONT = P2^1;
sbit LED_LEFT  = P2^2;

#define FRONT  0
#define LEFT   1
#define RIGHT  2
//#define REAR 3

#define OBSTACLE_DISTANCE 20

#define ECHO_START_TIMEOUT 30000
#define ECHO_END_TIMEOUT   60000

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
void timer0_init(void);
void trigger_sensor(unsigned char sensor);
unsigned int get_distance(unsigned char sensor);
void clear_leds(void);

void clear_leds(void)
{
    LED_FRONT = 0;
    LED_LEFT  = 0;
    LED_RIGHT = 0;
}

void delay_us(unsigned int us) {
    while (us--) {
        // Approximate 1us delay at 11.0592 MHz
        _nop_(); _nop_(); _nop_(); _nop_();
    }
}

// Function to create delay in milliseconds
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1275; j++);
}

void timer0_init(void)
{
    TMOD &= 0xF0;      // Clear Timer0 bits
    TMOD |= 0x01;      // Timer0 Mode1 (16-bit)
}

// Send trigger pulse

void trigger_sensor(unsigned char sensor)
{
    switch(sensor)
    {
        case FRONT:
            FRONT_TRIG = 1;
            delay_us(10);
            FRONT_TRIG = 0;
            break;

        case LEFT:
            LEFT_TRIG = 1;
            delay_us(10);
            LEFT_TRIG = 0;
            break;

        case RIGHT:
            RIGHT_TRIG = 1;
            delay_us(10);
            RIGHT_TRIG = 0;
            break;
    }
}

unsigned int get_distance(unsigned char sensor)
{
    unsigned int echo_time;
    unsigned int timeout;

    trigger_sensor(sensor);

    switch(sensor)
    {
        case FRONT:

            timeout = ECHO_START_TIMEOUT;
            while(!FRONT_ECHO && timeout)
                timeout--;

            if(timeout == 0)
                return 0;

            TH0 = 0;
            TL0 = 0;
            TF0 = 0;

            TR0 = 1;

            timeout = ECHO_END_TIMEOUT;
            while(FRONT_ECHO && timeout)
            {
							
                if(TF0)
								{
                  TR0=0;  
									return 0;
								}
                timeout--;
            }
							if(timeout == 0)
							{
								TR0 = 0;
								return 0;
							}
            
						TR0 = 0;
            break;

        case LEFT:

            timeout = ECHO_START_TIMEOUT;
            while(!LEFT_ECHO && timeout)
                timeout--;

            if(timeout == 0)
                return 0;

            TH0 = 0;
            TL0 = 0;
            TF0 = 0;

            TR0 = 1;

            timeout = ECHO_END_TIMEOUT;
            while(LEFT_ECHO && timeout)
            {
							 if(TF0)
								{
                  TR0=0;  
									return 0;
								}
                timeout--;
            }
							if(timeout == 0)
							{
								TR0 = 0;
								return 0;
							}
						TR0 = 0;
            break;

        case RIGHT:

            timeout = ECHO_START_TIMEOUT;
            while(!RIGHT_ECHO && timeout)
                timeout--;

            if(timeout == 0)
                return 0;

            TH0 = 0;
            TL0 = 0;
            TF0 = 0;

            TR0 = 1;

            timeout = ECHO_END_TIMEOUT;
            while(RIGHT_ECHO && timeout)
            {
							  if(TF0)
								{
                  TR0=0;  
									return 0;
								}
                timeout--;
            }
							if(timeout == 0)
							{
								TR0 = 0;
								return 0;
							}
							TR0 = 0;
            break;
    }

    echo_time = ((unsigned int)TH0 << 8) | TL0;

		echo_time = (echo_time * 1085UL) / 1000;

		return echo_time / 58;
}


void main(void) 
	{
	
    unsigned int front_dist, left_dist, right_dist;

		timer0_init(); 
	
    while (1) {
        front_dist = get_distance(FRONT);
			  delay_ms(60);

			  left_dist = get_distance(LEFT);
			  delay_ms(60);

			  right_dist = get_distance(RIGHT);
			  delay_ms(60);

        // Default: Turn off all LEDs
        clear_leds();

        // Check distances and turn on appropriate LEDs
        if (front_dist > 0 && front_dist < OBSTACLE_DISTANCE)
            LED_FRONT = 1;
        if (left_dist > 0 && left_dist < OBSTACLE_DISTANCE)
            LED_LEFT = 1;
        if (right_dist > 0 && right_dist < OBSTACLE_DISTANCE)
            LED_RIGHT = 1;

        delay_ms(200);
    }
}
