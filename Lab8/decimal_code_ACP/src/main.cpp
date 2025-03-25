#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


char SEG[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};

volatile unsigned char current_indicator = 0;
volatile unsigned int display = 0;

int main(void)
{
    // Налаштування портів семисегментних індикаторів на виведення та їх обнулення
  DDRC = 0xFF;
  PORTC = 0x00;
  DDRD = 0xFF;
  PORTD = 0x00;

    // Налаштування таймера 2
  TIMSK |= (1 << TOIE2);    // Дозвіл переривання від таймера 2
  TCCR2 |= (1 << CS21);     // Коефіцієнт ділення попереднього дільника=8

    // Налаштування АЦП
  ADCSRA |= (1 << ADEN)                       // Дозвіл АЦП
          | (1 << ADSC)                       // Запуск перетворення
          | (1 << ADATE)                      // Безперервний режим роботи АЦП
          | (1 << ADPS2) | (1 << ADPS1)       // Коефіцієнт ділення переддільника АЦП = 64
          | (1 << ADIE);                      // Дозвіл переривань від АЦП

  ADMUX &= (~(1 << REFS1)) & (~(1 << REFS0)); // Зовнішнє ДОН
  
  sei();    // Глобальний дозвіл переривань
  
  while (1)
  {
    _delay_ms(50);
  }

  return 0;
}

  // Умова переривання від АЦП, обробник перериваннь від АЦП
ISR(ADC_vect)
{
  display = ADC; // Присвоюємо глобальній змінній поточне значення АЦП
}

  // Умова переривання від таймера Т2, Обробник переривань від таймера Т2
ISR(TIMER2_OVF_vect)
{
  PORTD = 0xFF;                       // Вимикаємо всі сегменти
  PORTC = (1 << current_indicator);   // Обираємо поточний
  
    // індикатор
  switch (current_indicator)
  {
  case 0:
    PORTD = ~(SEG[display % 10000 / 1000]);   // Вмикаємо цифру одиниць
    break;
  case 1:
    PORTD = ~((SEG[display % 1000 / 100]));   // Вмикаємо цифру десятків
    break;
  case 2:
    PORTD = ~(SEG[display % 100 / 10]);       // Вмикаємо цифру сотень
    break;
  case 3:
    PORTD = ~(SEG[display % 10 / 1]);         // Вмикаємо цифру тисяч
    break;
  }

  if ((current_indicator++) > 2) // Переходимо на наступний індикатор
    current_indicator = 0;       // Обнуляємо, якщо current_indicator за межами
}
