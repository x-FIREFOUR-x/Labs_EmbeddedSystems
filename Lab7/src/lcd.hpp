#pragma once
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define eS_PORTA0 0
#define eS_PORTA1 1
#define eS_PORTA2 2
#define eS_PORTA3 3
#define eS_PORTA4 4
#define eS_PORTA5 5
#define eS_PORTA6 6
#define eS_PORTA7 7

#define eS_PORTB0 10
#define eS_PORTB1 11
#define eS_PORTB2 12
#define eS_PORTB3 13
#define eS_PORTB4 14
#define eS_PORTB5 15
#define eS_PORTB6 16
#define eS_PORTB7 17

#define eS_PORTC0 20
#define eS_PORTC1 21
#define eS_PORTC2 22
#define eS_PORTC3 23
#define eS_PORTC4 24
#define eS_PORTC5 25
#define eS_PORTC6 26
#define eS_PORTC7 27

#define eS_PORTD0 30
#define eS_PORTD1 31
#define eS_PORTD2 32
#define eS_PORTD3 33
#define eS_PORTD4 34
#define eS_PORTD5 35
#define eS_PORTD6 36
#define eS_PORTD7 37

#ifndef D0
#define D0 eS_PORTD0
#define D1 eS_PORTD1
#define D2 eS_PORTD2
#define D3 eS_PORTD3
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#endif


    //Функція зміни значення порта 
void pinChange(int port, int portValue)
{
    if (portValue == 0)
    {
        if (port == eS_PORTA0)
            PORTA &= ~(1 << PA0);
        else if (port == eS_PORTA1)
            PORTA &= ~(1 << PA1);
        else if (port == eS_PORTA2)
            PORTA &= ~(1 << PA2);
        else if (port == eS_PORTA3)
            PORTA &= ~(1 << PA3);
        else if (port == eS_PORTA4)
            PORTA &= ~(1 << PA4);
        else if (port == eS_PORTA5)
            PORTA &= ~(1 << PA5);
        else if (port == eS_PORTA6)
            PORTA &= ~(1 << PA6);
        else if (port == eS_PORTA7)
            PORTA &= ~(1 << PA7);
        else if (port == eS_PORTB0)
            PORTB &= ~(1 << PB0);
        else if (port == eS_PORTB1)
            PORTB &= ~(1 << PB1);
        else if (port == eS_PORTB2)
            PORTB &= ~(1 << PB2);
        else if (port == eS_PORTB3)
            PORTB &= ~(1 << PB3);
        else if (port == eS_PORTB4)
            PORTB &= ~(1 << PB4);
        else if (port == eS_PORTB5)
            PORTB &= ~(1 << PB5);
        else if (port == eS_PORTB6)
            PORTB &= ~(1 << PB6);
        else if (port == eS_PORTB7)
            PORTB &= ~(1 << PB7);
        else if (port == eS_PORTC0)
            PORTC &= ~(1 << PC0);
        else if (port == eS_PORTC1)
            PORTC &= ~(1 << PC1);
        else if (port == eS_PORTC2)
            PORTC &= ~(1 << PC2);
        else if (port == eS_PORTC3)
            PORTC &= ~(1 << PC3);
        else if (port == eS_PORTC4)
            PORTC &= ~(1 << PC4);
        else if (port == eS_PORTC5)
            PORTC &= ~(1 << PC5);
        else if (port == eS_PORTC6)
            PORTC &= ~(1 << PC6);
        else if (port == eS_PORTC7)
            PORTC &= ~(1 << PC7);
        else if (port == eS_PORTD0)
            PORTD &= ~(1 << PD0);
        else if (port == eS_PORTD1)
            PORTD &= ~(1 << PD1);
        else if (port == eS_PORTD2)
            PORTD &= ~(1 << PD2);
        else if (port == eS_PORTD3)
            PORTD &= ~(1 << PD3);
        else if (port == eS_PORTD4)
            PORTD &= ~(1 << PD4);
        else if (port == eS_PORTD5)
            PORTD &= ~(1 << PD5);
        else if (port == eS_PORTD6)
            PORTD &= ~(1 << PD6);
        else if (port == eS_PORTD7)
            PORTD &= ~(1 << PD7);
    }
    else
    {
        if (port == eS_PORTA0)
            PORTA |= (1 << PA0);
        else if (port == eS_PORTA1)
            PORTA |= (1 << PA1);
        else if (port == eS_PORTA2)
            PORTA |= (1 << PA2);
        else if (port == eS_PORTA3)
            PORTA |= (1 << PA3);
        else if (port == eS_PORTA4)
            PORTA |= (1 << PA4);
        else if (port == eS_PORTA5)
            PORTA |= (1 << PA5);
        else if (port == eS_PORTA6)
            PORTA |= (1 << PA6);
        else if (port == eS_PORTA7)
            PORTA |= (1 << PA7);
        else if (port == eS_PORTB0)
            PORTB |= (1 << PB0);
        else if (port == eS_PORTB1)
            PORTB |= (1 << PB1);
        else if (port == eS_PORTB2)
            PORTB |= (1 << PB2);
        else if (port == eS_PORTB3)
            PORTB |= (1 << PB3);
        else if (port == eS_PORTB4)
            PORTB |= (1 << PB4);
        else if (port == eS_PORTB5)
            PORTB |= (1 << PB5);
        else if (port == eS_PORTB6)
            PORTB |= (1 << PB6);
        else if (port == eS_PORTB7)
            PORTB |= (1 << PB7);
        else if (port == eS_PORTC0)
            PORTC |= (1 << PC0);
        else if (port == eS_PORTC1)
            PORTC |= (1 << PC1);
        else if (port == eS_PORTC2)
            PORTC |= (1 << PC2);
        else if (port == eS_PORTC3)
            PORTC |= (1 << PC3);
        else if (port == eS_PORTC4)
            PORTC |= (1 << PC4);
        else if (port == eS_PORTC5)
            PORTC |= (1 << PC5);
        else if (port == eS_PORTC6)
            PORTC |= (1 << PC6);
        else if (port == eS_PORTC7)
            PORTC |= (1 << PC7);
        else if (port == eS_PORTD0)
            PORTD |= (1 << PD0);
        else if (port == eS_PORTD1)
            PORTD |= (1 << PD1);
        else if (port == eS_PORTD2)
            PORTD |= (1 << PD2);
        else if (port == eS_PORTD3)
            PORTD |= (1 << PD3);
        else if (port == eS_PORTD4)
            PORTD |= (1 << PD4);
        else if (port == eS_PORTD5)
            PORTD |= (1 << PD5);
        else if (port == eS_PORTD6)
            PORTD |= (1 << PD6);
        else if (port == eS_PORTD7)
            PORTD |= (1 << PD7);
    }
}

    // 8-розрядні функції інтерфейсу LCD 
    // перевіряє кожен біт в bite і відповідно встановлює значення відповідного Dx на дісплеї)
void Lcd8_Port(char bite)
{
    if (bite & 1)
        pinChange(D0, 1);
    else
        pinChange(D0, 0);
    if (bite & 2)
        pinChange(D1, 1);
    else
        pinChange(D1, 0);
    if (bite & 4)
        pinChange(D2, 1);
    else
        pinChange(D2, 0);
    if (bite & 8)
        pinChange(D3, 1);
    else
        pinChange(D3, 0);
    if (bite & 16)
        pinChange(D4, 1);
    else
        pinChange(D4, 0);
    if (bite & 32)
        pinChange(D5, 1);
    else
        pinChange(D5, 0);
    if (bite & 64)
        pinChange(D6, 1);
    else
        pinChange(D6, 0);
    if (bite & 128)
        pinChange(D7, 1);
    else
        pinChange(D7, 0);
}

    //Функція надсилає команду до дісплея
void Lcd8_Cmd(char comand)
{
    pinChange(RS, 0);       // => RS = 0
    Lcd8_Port(comand);      // Data transfer
    pinChange(EN, 1);       // => E = 1
    _delay_ms(1);
    pinChange(EN, 0);       // => E = 0
    _delay_ms(1);
}

    // Функція очищення LСD
void Lcd8_Clear()
{
    Lcd8_Cmd(1);
}

    // Функція встановлення курсора (номер рядка, номер клітинки)
void Lcd8_Set_Cursor(char a, char b)
{
    if (a == 1)
        Lcd8_Cmd(0x80 + b);
    else if (a == 2)
        Lcd8_Cmd(0xC0 + b);
}

    // Функції ініціалізації LСD
void Lcd8_Init()
{
    pinChange(RS, 0);
    pinChange(EN, 0);
    _delay_ms(20);

    // Скидання процесу з таблиці даних
    Lcd8_Cmd(0x30);
    _delay_ms(5);
    Lcd8_Cmd(0x30);
    _delay_ms(1);
    Lcd8_Cmd(0x30);
    _delay_ms(10);

    Lcd8_Cmd(0xDF); // Додаткова команда ініціалізації (ймовірно, помилка)
    Lcd8_Cmd(0x38); // Вибір 8-розрядного режиму, 2 рядки, 5x8 символи
    Lcd8_Cmd(0x0C); // Включити дисплей, виключити курсор, виключити блимання
    Lcd8_Cmd(0x01); // Очистити дисплей
    Lcd8_Cmd(0x06); // Режим з автоматичним зсувом курсора
}

    // Функція виведення символа на LСD
void Lcd8_Write_Char(char a)
{
    pinChange(RS, 1);   // => RS = 1
    Lcd8_Port(a);       // Data transfer
    pinChange(EN, 1);   // => E = 1
    _delay_ms(1);
    pinChange(EN, 0);   // => E = 04
    _delay_ms(1);
}

    // Функція виведення рядка на LСD
void Lcd8_Write_String(char *a)
{
    int i;
    for (i = 0; a[i] != '\0'; i++) // Посимвольне виводення через цикл
        Lcd8_Write_Char(a[i]);
}

    // Функція, що зсуває текст LСD праворуч
void Lcd8_Shift_Right()
{
    Lcd8_Cmd(0x1C);
}

    // Функція, що зсуває текст LСD ліворуч
void Lcd8_Shift_Left()
{
    Lcd8_Cmd(0x18);
}