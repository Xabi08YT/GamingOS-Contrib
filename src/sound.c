#include "../include/sound.h"


char MasterVolume;
void PlaySound(uint32_t nFrequence, char  Volume) {
 	uint32_t Div;
 	uint8_t tmp;
	if (Volume > 100){
		char invert2[99];
		for (char i = 100; i>0; i--) invert2[i] = i;
		Volume = invert2[Volume];
		float Volpercent = Volume / 100;
		int volpercent2 = Volpercent*100;
    	nFrequence=nFrequence/volpercent2;
	}
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
}

void noSound(){
 	uint8_t tmp = inb(0x61) & 0xFC;
 
 	outb(0x61, tmp);
}

void beep(){
 	 PlaySound(1000,100);
 	 noSound();
}