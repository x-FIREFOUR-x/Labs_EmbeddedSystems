#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.hpp"


#ifndef F_CPU
#define F_CPU 1600000UL       // 16 MHz clock speed
#endif


volatile unsigned char current_indicator = 0;
volatile double display = 0.0;
volatile unsigned int ADC_value;

double mapVoltage()
{
  return ADC_value * 5.0 / 1023;
}

int main(void)
{
    // Ініціалізація портів
  DDRC = 0xF0;
  PORTC = 0x0F;
  DDRD = 0xFF;
  PORTD = 0x00;
  DDRF = 0x00;

  Lcd8_Init();                              // Виклик функції ініціалізації LСD
  Lcd8_Set_Cursor(1, 0);                    // Виклик функції встановлення курсора LСD в рядок 1 і положення 0
  Lcd8_Write_String("The voltage is: ");    // Виклик функції виведення рядка на LСD

    // Налаштування таймера 2
  TIMSK |= (1 << TOIE2);    // Дозвіл переривання за переповненням таймера 2
  TCCR2 |= (1 << CS21);     // Програмування коефіціента ділення системного тактового сигналу на 8
  
    // Налаштування АЦП
  ADCSRA |= (1 << ADEN)                     // Дозвіл АЦП
          | (1 << ADPS2)                    // Програмування попереднього дільника (64), що формує тактовий сигнал
          | (1 << ADPS1) | (1 << ADIE)      // Дозвіл переривання від завершення АЦП
          | (1 << ADFR)                     // Вибір режиму роботи АЦП: 1 – безперервне перетворення
          | (1 << ADSC);                    // Запуск перетворення (1 – почати перетворення)

  sei();    //Глобальний дозвіл переривань

  while (1)
  {
    _delay_ms(50);
  }

  return 0;
}

  // Умова переривання від АЦП, Обробник перериваннь від АЦП
ISR(ADC_vect)
{
  ADC_value = ADC;    //Присвоюємо глобальній змінній поточне значення АЦП
}

  // Умова переривання від таймера Т2, Обробник переривань від таймера Т2
ISR(TIMER2_OVF_vect)
{
  char voltageString[15];
  char doubleString[10];

    // Обраховуємо значення напруги за формулою: ADC_value*5.0/1023;
  display = mapVoltage();

    // Перетворюємо дані з плаваючою точкою в масив символів
  dtostrf(display, 1, 5, doubleString);

    // Функція повертає рядок, створений за допомогою doubleString
  sprintf(voltageString, "%7s V", doubleString);

    // Виклик функції встановлення курсору в задане положення LСD
  Lcd8_Set_Cursor(2, 0);

    // Виклик функції виведення рядка на LСD
  Lcd8_Write_String(voltageString);
}