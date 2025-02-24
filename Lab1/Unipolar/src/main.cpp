#include <avr/io.h>
#include <util/delay.h>


    // Перейменування портів до яких підключені кнопки відповідно до режиму роботи
#define STEPUPAUTO (PINC & (1 << PINC0))
#define STEPDOWNAUTO (PINC & (1 << PINC1))
#define STEPUP (PINC & (1 << PINC2))
#define STEPDOWN (PINC & (1 << PINC3))


    // Ініціалізація змінних
int checkUp = 1; 
int checkDown = 1;
int angle = 0;
int stepAngle = 90;
int timeDelay = 100;

    // Функція зміни значення порту Е, порт виводу керуючого впливу на двигун в залежності від кута
void stepper() 
{
    if (angle == 90)
        PORTE = 0x0C;
    if (angle == 180)
        PORTE = 0x06;
    if (angle == 270)
        PORTE = 0x03;
    if (angle == 360)
        PORTE = 0x09;
    if (angle == 0)
        PORTE = 0x09;
    if (angle == -90)
        PORTE = 0x03;
    if (angle == -180)
        PORTE = 0x06;
    if (angle == -270)
        PORTE = 0x0C;
    if (angle == -360)
        PORTE = 0x09;
}

    // Функція ініціалізації портів С та Е
void init_ports()
{
    DDRC = 0x00;
    DDRE = 0xFF; // порт С на введення; порт Е на виведення
}


int main(void)
{
    init_ports();
    
    while (1)
    {

            // Якщо натиснута кнопка STEPUPAUTO (PC0) двигун обертається за годинниковою стрілкою
        if (!STEPUPAUTO) 
        {
            PORTE = 0x0C;
            _delay_ms(timeDelay);
            PORTE = 0x06;           // 0x06 = 0b00000110  (PE1 і PE2 = 1)
            _delay_ms(timeDelay);
            PORTE = 0x03;
            _delay_ms(timeDelay);
            PORTE = 0x09;
            _delay_ms(timeDelay);
        }
        
            // Якщо натиснута кнопка STEPDOWNAUTO (PC1) двигун обертається проти годинникової стрілки
        if (!STEPDOWNAUTO) 
        {
            PORTE = 0x03;
            _delay_ms(timeDelay);
            PORTE = 0x06;
            _delay_ms(timeDelay);
            PORTE = 0x0C;
            _delay_ms(timeDelay);
            PORTE = 0x09;
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
}