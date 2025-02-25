#include <avr/io.h>
#include <util/delay.h>

// Перейменування портів до яких підключені кнопки відповідно до режиму роботи
#define STEPUPAUTO (PINC & (1 << PINC0))
#define STEPDOWNAUTO (PINC & (1 << PINC1))
#define STEPUP (PINC & (1 << PINC2))
#define STEPDOWN (PINC & (1 << PINC3))

#define HALFSTEPMODE (PINC & (1 << PINC5))

// Ініціалізація змінних
int checkUp = 1;
int checkDown = 1;
int angle = 0;
int stepAngle = 90;
int timeDelay = 100;

// Функція ініціалізації портів С та Е
void init_ports()
{
  DDRC = 0x00; // порт С на введення
  DDRE = 0xFF; // порт Е на виведення
}

// Функція зміни значення порту Е, для крокового режиму,
// порт виводу керуючого впливу на двигун в залежності від кута
void stepper()
{
  if (angle == 90)
    PORTE = 0x08;
  if (angle == 180)
    PORTE = 0x02;
  if (angle == 270)
    PORTE = 0x04;
  if (angle == 360)
    PORTE = 0x01;
  if (angle == 0)
    PORTE = 0x01;
  if (angle == -90)
    PORTE = 0x04;
  if (angle == -180)
    PORTE = 0x02;
  if (angle == -270)
    PORTE = 0x08;
  if (angle == -360)
    PORTE = 0x01;
}

// Функція керування МК для роботи уніполярного двигуна в кроковому режимі
void step_mode()
{
  // Якщо натиснута кнопка STEPUPAUTO (PC0) двигун обертається за годинниковою стрілкою
  if (!STEPUPAUTO)
  {
    PORTE = 0x08;
    _delay_ms(timeDelay);
    PORTE = 0x02;
    _delay_ms(timeDelay);
    PORTE = 0x04;
    _delay_ms(timeDelay);
    PORTE = 0x01;
    _delay_ms(timeDelay);
  }

  // Якщо натиснута кнопка STEPDOWNAUTO (PC1) двигун обертається проти годинникової стрілки
  if (!STEPDOWNAUTO)
  {
    PORTE = 0x04;
    _delay_ms(timeDelay);
    PORTE = 0x02;
    _delay_ms(timeDelay);
    PORTE = 0x08;
    _delay_ms(timeDelay);
    PORTE = 0x01;
    _delay_ms(timeDelay);
  }

  // Виконати крок за годинниковою стрілкою, якщо натиснута кнопка STEPUP (PC2),
  // змінити значення змінної angle на +90 та викликати функцію stepper()
  if (!STEPUP && checkUp == 1)
  {
    if (angle == 360)
    {
      angle = 0;
    }
    angle += stepAngle;
    checkUp = 0;
  }

  // Якщо кнопка відпущена STEPUP (PC2)
  if (STEPUP)
  {
    checkUp = 1; // 18. Прапорець checkUp встановлюється в 1
  }

  // Виконати крок проти годинникової стрілки, якщо натиснута кнопка STEPDOWN (PC3),
  // змінити значення змінної angle на -90 та викликати функцію stepper()
  if (!STEPDOWN && checkDown == 1)
  {
    if (angle == -360)
    {
      angle = 0;
    }
    angle -= stepAngle;
    checkDown = 0;
  }

  // Якщо кнопка відпущена STEPDOWN (PC3)
  if (STEPDOWN)
  {
    checkDown = 1;
  }

  // Виклик функції зміни кута повороту двигуна в покроковому режимі
  stepper();
}

// Функція зміни значення порту Е, для напівкрокового режиму,
// порт виводу керуючого впливу на двигун в залежності від кута
void half_stepper()
{
  if (angle == 0)
    PORTE = 0x01;
  if (angle == 45)
    PORTE = 0x09;
  if (angle == 90)
    PORTE = 0x08;
  if (angle == 135)
    PORTE = 0x0A;
  if (angle == 180)
    PORTE = 0x02;
  if (angle == 225)
    PORTE = 0x06;
  if (angle == 270)
    PORTE = 0x04;
  if (angle == 315)
    PORTE = 0x05;
  if (angle == 360)
    PORTE = 0x01;
  if (angle == -45)
    PORTE = 0x05;
  if (angle == -90)
    PORTE = 0x04;
  if (angle == -135)
    PORTE = 0x06;
  if (angle == -180)
    PORTE = 0x02;
  if (angle == -225)
    PORTE = 0x0A;
  if (angle == -270)
    PORTE = 0x08;
  if (angle == -315)
    PORTE = 0x09;
  if (angle == -360)
    PORTE = 0x01;
}

// Функція керування МК для роботи уніполярного двигуна в напівкроковому режимі
void half_step_mode()
{
  // Якщо натиснута кнопка STEPUPAUTO (PC0) двигун обертається за годинниковою стрілкою
  if (!STEPUPAUTO)
  {
    PORTE = 0x09;
    _delay_ms(timeDelay);
    PORTE = 0x08;
    _delay_ms(timeDelay);
    PORTE = 0x0A;
    _delay_ms(timeDelay);
    PORTE = 0x02;
    _delay_ms(timeDelay);
    PORTE = 0x06;
    _delay_ms(timeDelay);
    PORTE = 0x04;
    _delay_ms(timeDelay);
    PORTE = 0x05;
    _delay_ms(timeDelay);
    PORTE = 0x01;
    //_delay_ms(timeDelay);
  }

  // Якщо натиснута кнопка STEPDOWNAUTO (PC1) двигун обертається проти годинникової стрілки
  if (!STEPDOWNAUTO)
  {
    PORTE = 0x05; // 12. Поступове змінювання значення порту Е
    _delay_ms(timeDelay);
    PORTE = 0x04;
    _delay_ms(timeDelay);
    PORTE = 0x06;
    _delay_ms(timeDelay);
    PORTE = 0x02;
    _delay_ms(timeDelay);
    PORTE = 0x0A;
    _delay_ms(timeDelay);
    PORTE = 0x08;
    _delay_ms(timeDelay);
    PORTE = 0x09;
    _delay_ms(timeDelay);
    PORTE = 0x01;
    //_delay_ms(timeDelay);
  }

  // Виконати крок за годинниковою стрілкою, якщо натиснута кнопка STEPUP (PC2),
  // змінити значення змінної angle на +90 та викликати функцію stepper()
  if (!STEPUP && checkUp == 1)
  {
    if (angle == 360)
    {
      angle = 0;
    }
    angle += stepAngle;
    checkUp = 0;
  }

  // Якщо кнопка відпущена STEPUP (PC2)
  if (STEPUP)
  {
    checkUp = 1; // 18. Прапорець checkUp встановлюється в 1
  }

  // Виконати крок проти годинникової стрілки, якщо натиснута кнопка STEPDOWN (PC3),
  // змінити значення змінної angle на -90 та викликати функцію stepper()
  if (!STEPDOWN && checkDown == 1)
  {
    if (angle == -360)
    {
      angle = 0;
    }
    angle -= stepAngle;
    checkDown = 0;
  }

  // Якщо кнопка відпущена STEPDOWN (PC3)
  if (STEPDOWN)
  {
    checkDown = 1;
  }

  // Виклик функції зміни кута повороту двигуна в покроковому режимі
  half_stepper();
}

int main(void)
{
  init_ports();

  while (1)
  {
    // Якщо відпущена кнопка HALFSTEPMODE (PC5) двигун працює в кроковому режимі,
    // інакше в напівкроковому режимі
    if (HALFSTEPMODE)
    {
      stepAngle = 90;
      step_mode();
    }
    else
    {
      stepAngle = 45;
      half_step_mode();
    }
  }
}