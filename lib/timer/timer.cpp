
#include <Arduino.h>

unsigned int readTCNT1();
void writeTCNT1(unsigned int value);

void writeOCR1A(unsigned int value);

// callback function
void (*callback)(void) = NULL;

/// Initializes the 16 bit timer-counter unit
void initTimer()
{
    TCCR1A = 0;
    TCCR1B = 0;              // stop timer
    TCCR1B |= (1 << WGM12);  // CTC timer mode
    TIMSK1 |= (1 << OCIE1A); // enable output compare interrupts
}

/// Starts a 1 second timer
void startTimer(int seconds)
{
    TCCR1B |= B00000101; // 1024 prescaler
    writeOCR1A(15624);
}

/// Stops and resets timer
void stopTimer()
{
    TCCR1B &= B11111000; // disable timer
    writeTCNT1(0);       // clear the counter
}

/// Register a callback method to respond to the timer running out
void registerCallback(void (*func)())
{
    callback = func;
}

/// Reads a 16 bit register
unsigned int readTCNT1()
{
    unsigned int i;

    cli(); // disable interrupts

    i = TCNT1;

    sei();

    return i;
}

void writeTCNT1(unsigned int value)
{
    cli(); // disable interrupts

    TCNT1 = value;

    sei();
}

/// Writes the 16 bit output compare register
void writeOCR1A(unsigned int value)
{
    cli(); // disable interrupts

    OCR1A = value;

    sei();
}

// output compare match interrupt service routine
ISR(TIMER1_COMPA_vect)
{
    if (callback != NULL)
    {
        callback(); // call the callback function
    }
}
