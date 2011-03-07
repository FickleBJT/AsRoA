#ifndef __EXTRA_MACROS_H
#define __EXTRA_MACROS_H

/* Register Settings */

// TIMSK bits

#define TIMOVF0   0x01
#define OUTCOMP0  0x02
#define TIMOVF1   0x04
#define OUTCOMP1B 0x08
#define OUTCOMP1A 0x10
#define INCAP1    0x20
#define TIMOVF2   0x40
#define OUTCOMP2  0x80


// TCCR0 bits

#define CLKSEL00    0x01
#define CLKSEL01    0x02
#define CLKSEL02    0x04
#define WAVGEN01    0x08
#define COMPMOD00   0x10
#define COMPMOD01   0x20
#define WAVGEN00    0x40
#define FORCECOMP0  0x80 // Should always be zero

// TCCR1A bits

#define WAVGEN10    0x01
#define WAVGEN11    0x02
#define FORCECOMP1B 0x04
#define FORCECOMP1A 0x08
#define COMPMOD1B0  0x10
#define COMPMOD1B1  0x20
#define COMPMOD1A0  0x40
#define COMPMOD1A1  0x80

// TCCR1B bits

#define CLKSEL10    0x01
#define CLKSEL11    0x02
#define CLKSEL12    0x04
#define WAVGEN12    0x08
#define WAVGEN13    0x10
// PIN 5 UNUSED     0x20
#define INCAPES     0x40
#define INCAPNC     0x80

// TCCR2 bits

#define CLKSEL20   0x01
#define CLKSEL21   0x02
#define CLKSEL22   0x04
#define WAVGEN21   0x08
#define COMPMOD20  0x10
#define COMPMOD21  0x20
#define WAVGEN20   0x40
#define FORCECOMP  0x80 // Should always be zero

#endif
