#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


#define F_CPU 16000000L


int main(void) // 0
{
    // Init port B
    DDRB = 0b00100011;      //встановлення типу виводів порта B, 1 - порт як вихід, 0 - порт як вхід 
    PORTB = 0b00000000;     //встановлення значення виводів порта B

    // Init 16–bit timer 1 values
    OCR1A = 20;
    ICR1 = 40;
    int delay = 1; // змінна delay, яка використовується при зміні // шпаруватості

    // Init Phase and frequency correct PWM
    TCCR1B = _BV(WGM13);
    TCCR1A = 0;
    TCCR1A |= _BV(COM1A0);
    TCCR1A |= _BV(COM1A1);
    TCCR1B |= _BV(CS11);

    // об’явлення змінних-прапорців
    char flag_start_stop = 0, flag_reverse = 0, flag_speedup = 0, flag_speeddown = 0;

    while (1)
    {
        // Start–Stop, button action
        if (!(PINB & 4))
        {
            flag_start_stop = 1;
            _delay_ms(10);
        }

        if ((flag_start_stop == 1) && (PINB & 4))
        {
            PORTB ^= 1;
            flag_start_stop = 0;
        }

        // Reverse, button action
        if (!(PINB & 8))
        {
            flag_reverse = 1;
            _delay_ms(10);
        }

        if ((flag_reverse == 1) && (PINB & 8))
        {
            PORTB ^= 3;
            flag_reverse = 0;
        }

        // Speed up, button action
        if (!(PINB & 16))
        {
            flag_speedup = 1;
            _delay_ms(10);
        }

        if ((flag_speedup == 1) && (PINB & 16))
        {
            if (OCR1A != 0)
                OCR1A -= delay;
            flag_speedup = 0;
        }

        //Speed down, button action
        if (!(PINB & 64))
        {
            flag_speeddown = 1;
            _delay_ms(10);
        }

        if ((flag_speeddown == 1) && (PINB & 64))
        {
            if (OCR1A != 40)
                OCR1A += delay;
            flag_speeddown = 0;
        }
    }
}
