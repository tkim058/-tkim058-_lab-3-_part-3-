/*
 * [tkim058]_lab[3]_part[3].c
 * Partner(s) Name & E-mail: MinHwan Oh
 * Lab Section: B01
 * Assignment: Lab #3  Exercise #3
 * Exercise Description: [Door Lock]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */


#include <avr/io.h>

enum States{Locked, PA2_prsd, PA2_rlsd, Unlocked} state;
unsigned char tmpB = 0x00; 
unsigned char tmpC = 0x00;
unsigned char A = 0x00;

// Bit-access function

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {

	return (b ? x | (0x01 << k) : x & ~(0x01 << k));

}

unsigned char GetBit(unsigned char x, unsigned char k) {

	return ((x & (0x01 << k)) != 0);

}


void Tick() {
	switch (state) { //Transitions
		case Locked:
		if (GetBit(A, 2))
		{
			state = PA2_prsd;
		}
		else if((GetBit(A,1)) && (GetBit(A,2)) && (GetBit(A,3)))
		{
			state = Locked;
		}
		else if ((GetBit(A,1)) && (GetBit(A,2)))
		{
			state = Locked;
		}
		else if ((GetBit(A,1)) &&  (GetBit(A,3)))
		{
			state = Locked;
		}
		else 
		{
			state = Locked;
		}
		break;
		
		case PA2_prsd:
		if (GetBit(A, 2))
		{
			state = PA2_prsd;
		}
		
		else 
		{
			state = PA2_rlsd;
		}
		break;
		
		case PA2_rlsd:
		if(GetBit(A, 1))
		{
			state = Unlocked;
		}
		else if ((GetBit(A,1)) && (GetBit(A,2)) && (GetBit(A,3)))
		{
			state = Locked;
		}
		else if ((GetBit(A,1)) && (GetBit(A,2)))
		{
			state = Locked;
		}
		else if ((GetBit(A,1)) &&  (GetBit(A,3)))
		{
			state = Locked;
		}
		else 
		{
			state = PA2_rlsd;
		}
		break;
		
		case Unlocked:
		if(GetBit(A, 7))
		{
			state = Locked;
		}

		else
		{
			state = Unlocked;
		}
		break;
		
		default:
		break;
	}
	switch (state) { //State Actions
		case Locked: // initial state, Locked
		tmpB = SetBit(tmpB, 0, 0);
		tmpC = SetBit(tmpC, 0, 0);
		tmpC = SetBit(tmpC, 1, 0);
		break;
		case PA2_prsd: // Pressing #
		tmpB = SetBit(tmpB, 0, 0);
		tmpC = SetBit(tmpC, 0, 1);
		tmpC = SetBit(tmpC, 1, 0);
		
		break;
		case PA2_rlsd: // # released
		tmpB = SetBit(tmpB, 0, 0);
		tmpC = SetBit(tmpC, 0, 0);
		tmpC = SetBit(tmpC, 1, 1);
		break;
		case Unlocked: // Unlocked PB0 = 0x01
		tmpB = SetBit(tmpB, 0, 1);
		tmpC = SetBit(tmpC, 0, 1);
		tmpC = SetBit(tmpC, 1, 1);
		break;
		default:
		break;
	}
}

int main()
{
	state = Locked; // initial state, Locked
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as input
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00;

	while(1)
	{	A = PINA;
		Tick();
		// 3) write results to port
		PORTB = tmpB;
		PORTC = tmpC;
	}
	return 0;
}