#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t counter = 0; 

void satu (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x06;
	else if (prt == 2)
		PORTB = 0x06;
	else if (prt == 3)
		PORTC = 0x06;
	else if (prt == 4)
		PORTD = 0x06;
}

void dua (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x5B;
	else if (prt == 2)
		PORTB = 0x5B;
	else if (prt == 3)
		PORTC = 0x5B;
	else if (prt == 4)
		PORTD = 0x5B;
}

void tiga (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x4F;
	else if (prt == 2)
		PORTB = 0x4F;
	else if (prt == 3)
		PORTC = 0x4F;
	else if (prt == 4)
		PORTD = 0x4F;
}

void empat (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x66;
	else if (prt == 2)
		PORTB = 0x66;
	else if (prt == 3)
		PORTC = 0x66;
	else if (prt == 4)
		PORTD = 0x66;
}

void lima (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x6D;
	else if (prt == 2)
		PORTB = 0x6D;
	else if (prt == 3)
		PORTC = 0x6D;
	else if (prt == 4)
		PORTD = 0x6D;
}

void enam (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x7D;
	else if (prt == 2)
		PORTB = 0x7D;
	else if (prt == 3)
		PORTC = 0x7D;
	else if (prt == 4)
		PORTD = 0x7D;
	
}

void tujuh (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x07;
	else if (prt == 2)
		PORTB = 0x07;
	else if (prt == 3)
		PORTC = 0x07;
	else if (prt == 4)
		PORTD = 0x07;
}

void delapan (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x7F;
	else if (prt == 2)
		PORTB = 0x7F;
	else if (prt == 3)
		PORTC = 0x7F;
	else if (prt == 4)
		PORTD = 0x7F;
	
}

void sembilan (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x6F;
	else if (prt == 2)
		PORTB = 0x6F;
	else if (prt == 3)
		PORTC = 0x6F;
	else if (prt == 4)
		PORTD = 0x6F;
}

void nol (uint8_t prt)
{
	if (prt == 1)
		PORTA = 0x3F;
	else if (prt == 2)
		PORTB = 0x3F;
	else if (prt == 3)
		PORTC = 0x3F;
	else if (prt == 4)
		PORTD = 0x3F;
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
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS11);
}

ISR (TIMER1_COMPA_vect)
{   
	counter++;
	if (counter > 9)
		counter = 0;
	if (counter == 0)
		nol(4);
	else if (counter == 1)
		satu(4);
	else if (counter == 2)
		dua(4);
	else if (counter == 3)
		tiga(4);
	else if (counter == 4)
		empat(4);
	else if (counter == 5)
		lima(4);
	else if (counter == 6)
		enam(4);
	else if (counter == 7)
		tujuh(4);
	else if (counter == 8)
		delapan(4);
	else if (counter == 9)
		sembilan(4);
	else if (counter == 0)
		nol(4);
}

int main (void)
{
	DDRD = 0xFF;
	init_timer1();	    // init Timer1
    sei(); // enable global interrupts
	while (1) {
		
	}

	return 1;
}

