#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define BIT_GET(p,m) ((p) & (m))
#define BIT_SET(p,m) ((p) |= (m))
#define BIT_CLEAR(p,m) ((p) &= ~(m))
#define BIT_FLIP(p,m) ((p) ^= (m))
#define BIT_WRITE(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))
#define DIGIT 4
#define ACT 1
#define DEACT 2
#define JAM 1
#define MENIT 2

//define timing for watch
#define CTC_COMPARE 31250
#define DELAY_POOL_BTN 725

//port a - sebagai output jam with 22 ohm
//portb 0 - 3 untuk switch transistor with 4.7k ohm
//PB4 sebagai switch mode with sebarang ohm
//PB5 sebagai change jam with sebarang ohm
//PB6 sebagai change menit with sebarang ohm

volatile uint8_t detik = 0;
volatile uint8_t menit = 0;
volatile uint8_t jam = 0;
volatile uint8_t state = 0; //state melepas tombol
volatile uint8_t anychange = 0; //menetukan untuk merubah tampilan
volatile uint8_t chk = 0; //state menekan tombol
volatile uint8_t i;
volatile uint8_t digit [DIGIT] = {0};
volatile uint8_t stat_edit = 0;

void init_timer1 () 
{
    // Timer Mode 4: Clear Timer on Compare match (CTC)
	TCCR1B |= (1<<WGM12); 
    // Initialize Timer staring value
	TCNT1 = 0;
    // Set Compare value for 1s overflow
	OCR1A = CTC_COMPARE;
    // Enable Timer Compare A Match interrupt
	TIMSK |= (1<<OCIE1A);
    // Start Timer & Clock Select: Prescale I/O clock by 256
	TCCR1B |= (1 << (CS12));
}

void pause_timer ()
{
	BIT_CLEAR(TCCR1B, BIT(CS12));
}

void resume_timer ()
{
	detik = 0;
	TCNT1 = 0;
	TCCR1B |= (1 << (CS12));
}



void set_digit ()
{
	digit [0] = menit % 10;
	digit [1] = menit / 10;
	digit [2] = jam % 10;
	digit [3] = jam /10;
}

void showin (uint8_t angka)
{
	if (angka == 0)
		PORTA = 0x3F;
	else if (angka == 1)
		PORTA = 0x06;
	else if (angka == 2)
		PORTA = 0x5B;
	else if (angka == 3)
		PORTA = 0x4F;
	else if (angka == 4)
		PORTA = 0x66;
	else if (angka == 5)
		PORTA = 0x6D;
	else if (angka == 6)
		PORTA = 0x7D;
	else if (angka == 7)
		PORTA = 0x07;
	else if (angka == 8)
		PORTA = 0x7F;
	else if (angka == 9)
		PORTA = 0x6F;
}

/*ISR (INT0_vect)
{
	detik++;
	if (detik > 60) {
		anychange = 1;
		detik = 0;
	}
}*/


ISR (TIMER1_COMPA_vect)
{   
	detik++;
	if (detik > 59) {
		anychange = 1;
		detik = 0;
	}
}

int main () {
	DDRA = 0x7F;
	DDRB = 0x0F;
	//GICR = 1 << INT0;				// Enable INT0
	//MCUCR = 1 << ISC01 | 1 << ISC00;	// Trigger INT0 on changing state
	init_timer1();	    // init Timer1
    sei(); // enable global interrupts
	while (1) {
		if (BIT_GET(PINB, BIT(4))) {
			if (chk) { //tekan kedua
				state = DEACT;
			} else { //tekan pertama
				state = ACT;
				pause_timer(); //pause timer
			}
		} else {
			if (state == ACT) { //masuk mode set
				chk = 1;
				// edit jam (button press)
				if (BIT_GET(PINB, BIT(5))) {
					_delay_ms(DELAY_POOL_BTN); //debouncing polling
					if (BIT_GET(PINB, BIT(5))) {
						stat_edit = JAM;
					}
				}
				//edit menit (button press)
				if (BIT_GET(PINB, BIT(6))) {
					_delay_ms(DELAY_POOL_BTN);
					if (BIT_GET(PINB, BIT(6))) {
						stat_edit = MENIT;
					}
				}
				// eksekusi edit
				if (stat_edit == JAM) {
					jam++;
					if (jam > 23)
						jam = 0;
					stat_edit = 0;
				}
				else if (stat_edit == MENIT) {
					menit++;
					if (menit > 60)
						menit = 0;
					stat_edit = 0;
				}
				set_digit();
			} else if (state == DEACT) { //keluar mode set
				resume_timer(); //resume timer
				state = 0;
				chk = 0;
			}
			else {
				if (anychange) {
					menit++;
					if (menit > 59) {
						jam++;
						menit = 0;
						if (jam > 23) {
							jam = 0;
						}
					}
					set_digit();
					anychange = 0;
				}
			}
		}
			//display 7 segment
		for (i = 0; i < DIGIT; i++) {
				PORTB =(1 << i); // 'i'th PORTB GIVEN HIGH
				showin(digit [i]);
				_delay_ms(3);
				PORTB = (0 << i); //'i'th PORTB GIVEN LOW
		}
	}
	return 1;
}
