/*
 * main.c
 *
 * Created: 5/4/2022 6:04:50 PM
 *  Author: j00wa
 */ 

#include <xc.h>
#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include "avr.h"
#include "lcd.h"
#include "avr.c"
#include "lcd.c"

#define A_NOTE 220.00
#define A_SHARP_NOTE 233.08
#define C_NOTE 261.63
#define C_SHARP_NOTE 277.18
#define D_NOTE 293.66
#define D_SHARP_NOTE 311.13
#define E_NOTE 329.63
#define F_NOTE 350.23
#define F_SHARP_NOTE 369.99
#define G_NOTE 393.00
#define G_SHARP_NOTE 415.30
#define A_UP_NOTE 434.50
#define A_SHARP_UP_NOTE 466.16
#define B_NOTE 480.88
#define C_UP_NOTE 524

#define W_DURATION 1
#define H_DURATION 0.5
#define Q_DURATION 0.25
#define Ei_DURATION 0.125

int pitch = 0;
int tempo_change = 1;

typedef enum {
	A, As, B, C, Cs, D, Ds, Ee, F, Fs, G, Gs, Au, Asu, Cu
} Note;

typedef enum {
	W, H, Q, Ei
} Duration;

typedef struct {
	Note note;
	Duration duration;
} PlayingNote;


PlayingNote mary_lamb[] = {
	{Ee, Q},
	/* Wait for half */
	{D, Q},
	{C, Q},
	/* Wait for half */
	{D, Q},
	/* Wait for quarter */
	{Ee, Q},
	/* Keep going... */
	{Ee, Q},
	
	{Ee, H},
	{D, Q},
	{D, Q},
	/* Wait for half */
	{D, H},
	/* Wait for quarter */
	{Ee, Q},
	/* Keep going... */
	{Ee, Q},
	
	{Ee, H},
	{Ee, Q},
	/* Wait for half */
	{D, Q},
	{C, Q},
	/* Wait for half */
	{D, Q},
	/* Wait for quarter */
	{Ee, Q},
	/* Keep going... */
	{Ee, Q},
	{Ee, Q},
	{Ee, Q},
	{D, Q},
	{D, Q},
	{Ee, Q},
	{D, Q},
	{C, H},
		

	

};

PlayingNote happy_birthday[] = {
	{C, Q},
	/* Wait for half */
	{C, Q},
	{D, Q},
	/* Wait for half */
	{C, Q},
	/* Wait for quarter */
	{F, Q},
	/* Keep going... */
	{Ee, H},
		
	{C, Q},
	{C, Q},
	{D, Q},
	/* Wait for half */
	{C, Q},
	/* Wait for quarter */
	{G, Q},
	/* Keep going... */
	{F, H},
		
	{C, Q},
	{C, Q},
	{Cu, Q},
	/* Wait for half */
	{Au, Q},
	/* Wait for quarter */
	{G, Q},
	/* Keep going... */
	{Ee, Q},
	{D, H},
		
	{Asu, Q},
	{Asu, Q},
	{Au, Q},
	{F, Q},
	{G, Q},
	{F, H},
};

PlayingNote twinkle[] = {
	{C, Q},
	/* Wait for half */
	{C, Q},
	{G, Q},
	/* Wait for half */
	{G, Q},
	/* Wait for quarter */
	{Au, Q},
	/* Keep going... */
	{Au, Q},
	
	{G, Q},
	{F, Q},
	{F, Q},
	/* Wait for half */
	{Ee, Q},
	/* Wait for quarter */
	{Ee, Q},
	/* Keep going... */
	{D, Q},
	{D, Q},
	{C, Q},
};

int is_pressed(int r, int c);
int get_key();
void play_song(PlayingNote song[], int length, int choice);
void play_note(PlayingNote* note);
void wait(unsigned short msec);

int main(void)
{
	SET_BIT(DDRB,3);
	avr_init();
	lcd_init();
	lcd_clr();
	
	lcd_pos(0,3);
	lcd_puts2("MUSIC BOX");
	lcd_pos(1,2);
	lcd_puts2("Songs: 1,2,3");
	
	while (1)
	{
		int key = get_key();
		if (key) {
			switch (key) {
				case 1:
					play_song(happy_birthday, 25, 1);
					break;
				case 2:
					play_song(twinkle, 14, 2);
					break;
				case 3:
					play_song(mary_lamb, 26, 3);
					break;
				default:
					break;
			}			
		}	
	} 

}

void wait(unsigned short msec) {
	TCCR0 = 2;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 8) * 0.0001);
		SET_BIT(TIFR, TOV0);
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

int is_pressed(int r, int c) {
	DDRC = 0;
	PORTC = 0;
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);
	CLR_BIT(DDRC,c+4);
	SET_BIT(PORTC, c+4);
	
	avr_wait(1);
	if (!GET_BIT(PINC, c+4)) {
		return 1;
	}
	return 0;
}

int get_key() {
	int i, j;
	for (i=0; i < 4; i++) {
		for (j=0; j < 4; j++) {
			if (is_pressed(i, j)) {
				return (4 * i) + j + 1;
			}
		}
	}
	return 0;
}

void play_song(PlayingNote song[], int length, int choice) {
	int i;
	
	if (choice == 1) {
		lcd_clr();
		lcd_pos(0 ,3);
		lcd_puts2("Song 1:");
		lcd_pos(1 ,0);
		lcd_puts2("Happy Birthday");
	}
	if (choice == 2) {
		lcd_clr();
		lcd_pos(0 ,3);
		lcd_puts2("Song 2:");
		lcd_pos(1 ,0);
		lcd_puts2("Twinkle Twinkle");
	}
	if (choice == 3) {
		lcd_clr();
		lcd_pos(0 ,1);
		lcd_puts2("Song 3: Mary's");
		lcd_pos(1 ,2);
		lcd_puts2("Little Lamb");
	}

	for (i = 0; i < length; i++) {

		play_note(&song[i]);
		//avr_wait(50);
	}
	
	//set screen to default
	lcd_clr();
		
	lcd_pos(0,3);
	lcd_puts2("MUSIC BOX");
	lcd_pos(1,2);
	lcd_puts2("Songs: 1,2,3");
}

void play_note(PlayingNote* note) {
	int q = get_key();

	
	// change frequency lower PRESS 4
	if (q == 5) {
		pitch += 60;
	}
	//change frequency higher PRESS 5
	if (q == 6) {
		pitch -= 60;
	}
		// change tempo slower PRESS 6
	if (q == 7) {
		tempo_change *= 2;
	}
	//change tempo faster PRESS 7
	if (q == 9) {
		tempo_change *= 0.8;
	}
	
	// if k == A, pause it, B to PLAY
	if (q == 4) {
		while (1) {
			q = get_key();
			if (q == 8) {
				break;
			}
		}
	}
	
	double frequency = 0;
	Note n = note->note;
	Duration d = note->duration;
	
	double P = 0;
	
	switch (n) {
		case A:
			frequency = A_NOTE;
			break;
		case As:
			frequency = A_SHARP_NOTE;
			break;
		case B:
			frequency = B_NOTE;
			break;
		case C:
			frequency = C_NOTE;
			break;
		case Cs:
			frequency = C_SHARP_NOTE;
			break;
		case D:
			frequency = D_NOTE;
			break;
		case Ds:
			frequency = D_SHARP_NOTE;
			break;
		case Ee:
			frequency = E_NOTE;
			break;
		case F:
			frequency = F_NOTE;
			break;
		case Fs:
			frequency = F_SHARP_NOTE;
			break;
		case G:
			frequency = G_NOTE;
			break;
		case Gs:
			frequency = G_SHARP_NOTE;
			break;
		case Au:
			frequency = A_UP_NOTE;
			break;
		case Asu:
			frequency = A_SHARP_UP_NOTE;
			break;
		case Cu:
			frequency = C_UP_NOTE;
			break;
	}
	P = 1 / (frequency + pitch); 
	P *= 10000;
	
	double TH = P / 2.0, TL = P / 2.0;
	double duration = 0;

	
	switch (d) {
		case W:
			duration = W_DURATION;
			break;
		case H:
			duration = H_DURATION;
			break;
		case Q:
			duration = Q_DURATION;
			break;
		case Ei:
			duration = Ei_DURATION;
			break;
	}
	double final_dur = duration * tempo_change;
	
	int i, k = final_dur / (P / 10000);
	
	for (i = 0; i < k; i++) {
		SET_BIT(PORTB, 3);
		wait(round(TH));
		CLR_BIT(PORTB, 3);
		wait(round(TL));
	}
}
