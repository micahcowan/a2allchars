// allchas.c
//
// Apple ][: display all character codes on screen
//           then prompt for keypresses and display those
//
// Copyright (C) 2022  Micah J Cowan
// MIT License

#include <stdlib.h>
#include <stdio.h>

unsigned char * const screen = (char *)(0x400);
unsigned char * const CH = (unsigned char *)0x24;
unsigned char * const CV = (unsigned char *)0x25;
unsigned char * * const BAS = (unsigned char **)0x28;
void (* const VTAB)(void) = (void (* const)(void))0xFC22;
void (** const KSW)(void) = (void (** const)(void))0x38;

unsigned char rdkey(void)
{
  unsigned char ret;
  asm("lda #$20");
  (*KSW)();
  asm("sta %v",ret);
  return ret;
}

int main (void)
{
  unsigned char c=0;	// char to draw
  unsigned char *p = screen;	// where to draw it
  unsigned char j=0;	// how far into a line we are
  
  do {
    *p = c;
    ++c; ++p; ++j;
    if (j == 32) {
      // line break;
      j = 0;
      p = p - 32 + 0x80;
      if (p - screen >= 0x400) {
        p = screen + 40;
      }
    }
  } while (c != 0);	// until every char has been drawn
  
  *CV = 12;
  VTAB();
  printf("NO KEY PRESSED");
  while (c = rdkey()) {
    *CH = 0;
    printf("KEY $%02x       \b\b\b\b\b\b", (int)c);
    //*CH = 9;
    *(*BAS + *CH) = c;
  }
  return EXIT_SUCCESS;
}

