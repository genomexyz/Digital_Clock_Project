#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BIT_GET(p,m) ((p) & (m))
#define BIT_SET(p,m) ((p) |= (m))
#define BIT_CLEAR(p,m) ((p) &= ~(m))
#define BIT_FLIP(p,m) ((p) ^= (m))
#define BIT_WRITE(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m))
#define BIT(x) (0x01 << (x))

int main ()
{
	DDRB = 0x03;
	PORTB = 0x02;
	while (1) {
		if (BIT_GET(PINB, BIT(7))) {
			BIT_SET(PORTB, BIT(0));
		}
		BIT_CLEAR(PORTB, BIT(0));
	}
	return 1;
}
