#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define BIT_GET(p,m) ((p) & (m))
#define BIT_SET(p,m) ((p) |= (m))
#define BIT_CLEAR(p,m) ((p) &= ~(m))
#define BIT_FLIP(p,m) ((p) ^= (m))
#define BIT_WRITE(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))

volatile uint8_t counter = 0;
volatile uint8_t change = 0;
volatile uint8_t i;
volatile uint8_t digit [2] = {0};

void showin (uint8_t angka)
{
	if (angka == 0)
		PORTD = 0x3F;
	else if (angka == 1)
		PORTD = 0x06;
	else if (angka == 2)
		PORTD = 0x5B;
	else if (angka == 3)
		PORTD = 0x4F;
	else if (angka == 4)
		PORTD = 0x66;
	else if (angka == 5)
		PORTD = 0x6D;
	else if (angka == 6)
		PORTD = 0x7D;
	else if (angka == 7)
		PORTD = 0x07;
	else if (angka == 8)
		PORTD = 0x7F;
	else if (angka == 9)
		PORTD = 0x6F;
}

void init_timer1 () 
{
    // Timer Mode 4: Clear Timer on Compare match (CTC)
	TCCR1B |= (1<<WGM12); 
    // Initialize Timer staring value
	TCNT1 = 0;
    // Set Compare value for 1s overflow
	OCR1A = 15625;
    // Enable Timer Compare A Match interrupt
	TIMSK |= (1<<OCIE1A);
    // Start Timer & Clock Select: Prescale I/O clock by 64
	TCCR1B |= (1 << (CS10));
	TCCR1B |= (1 << (CS11));
}

ISR (TIMER1_COMPA_vect)
{   
	counter++;
	if (counter > 99)
		counter = 0;
	change = 1;
}

int main ()
{
	DDRD = 0x7F;
	DDRB = 0x03;
	init_timer1();	    // init Timer1
    sei(); // enable global interrupts
	while (1) {
		if (change) {
			digit [0] = counter % 10;
			digit [1] = counter / 10;
			change = 0;
		}
		//display 7 segment
		for(i = 0; i < 2; i++) {
			PORTB =(1 << i); // 'i'th PORTD GIVEN HIGH
			showin(digit [i]);
			_delay_ms(5);
			PORTB = (0 << i); //'i'th PORTD GIVEN LOW
		}
	}
	return 1;
}
