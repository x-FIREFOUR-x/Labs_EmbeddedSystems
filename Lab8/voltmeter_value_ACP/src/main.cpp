#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


    // Масив значень для відображення цифр на семисегментних індикаторах
char SEG[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char SEG_with_dot[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

  // Ініціалізації глобальних змінних
volatile unsigned char current_indicator = 0;
volatile unsigned int display = 0;
volatile unsigned int ADC_value;

int main(void)
{
    // Налаштування та обнулення портів С та D керування семисегментними індикаторами
  DDRC = 0xFF;
  PORTC = 0x00;
  DDRD = 0xFF;
  PORTD = 0x00;

    // Налаштування таймера Т2
  TIMSK |= (1 << TOIE2);    // Дозвіл переривання від таймера Т2
  TCCR2 |= (1 << CS21);     // Переддільник на 8

    // Налаштування АЦП
  ADCSRA |= (1 << ADEN)                       // Дозвіл АЦП
          | (1 << ADSC)                       // Запуск перетворення
          | (1 << ADATE)                      // Безперервний режим роботи АЦП
          | (1 << ADPS2) | (1 << ADPS1)       // Переддільник на 64
          | (1 << ADIE);                      // Дозвіл переривань від АЦП

  ADMUX &= (~(1 << REFS1)) & (~(1 << REFS0)); // Зовнішнє ДОН

    // Глобальний дозвіл переривань
  sei();

  while (1)
  {
    display = ADC_value * (5.0 / 1023) * 100;
    _delay_ms(50);
  }

  return 0;
}

  // Умова переривання від АЦП, Обробник переривань від АЦП
ISR(ADC_vect)
{
  ADC_value = ADC;    // Присвоювання глобальній змінній поточного значення АЦП
}

  // Умова переривання від таймера Т2, Обробник переривань від таймера Т2
ISR(TIMER2_OVF_vect)
{
  PORTD = 0xFF;                     // Вимикання всіх сегментів
  PORTC = (1 << current_indicator); // Обирання поточного індикатора

  // починаючи зліва направо) Обрахування значення напруги, яка виводиться на дисплей у вольтах
  // ? помилка швидкості або неправильності
  //display = ADC_value * (5.0 / 1023) * 100;   

  switch (current_indicator)
  {
  case 0:
    PORTD = ~(SEG[display % 10000 / 1000]);         // Вмикання цифри десятків
    break;
  case 1:
    PORTD = ~(SEG_with_dot[display % 1000 / 100]);  // Вмикання цифри одиниць з десятковою крапкою
    break;
  case 2:
    PORTD = ~(SEG[display % 100 / 10]);             // Вмикання цифри десятих
    break;
  case 3:
    PORTD = ~(SEG[display % 10 / 1]);               // Вмикання цифри сотих
    break;
  }

  if ((current_indicator++) > 2)  // Перехід на наступний індикатор, якщо current_indicator не більше двох
    current_indicator = 0;        // Обнулення current_indicator, якщо він більше двох
  
}
