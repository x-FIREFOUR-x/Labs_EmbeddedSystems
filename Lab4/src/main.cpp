#include <avr/io.h>
#include <avr/interrupt.h>

#define TRUE 1
#define FALSE 0

  // Режими роботи годинника.
#define MODE_WORK 0
#define MODE_EDIT_SECS 1
#define MODE_EDIT_MINS 2
#define MODE_EDIT_HOURS 3

  // Ліміти для годин, хвилин і секунд у BCD-коді
#define LIMIT_HOURS 0x24
#define LIMIT_MINUTES 0x60
#define LIMIT_SECONDS 0x60

  // Позиції у масиві, де зберігаються відповідні компоненти поточного часу
#define I_SECONDS 0
#define I_MINUTES 1
#define I_HOURS 2

  // Кількість цифр на індикаторі
#define N_DIGITS 6

  // Частота процесора * UL - беззнаковий Long [0, +4 294 967 295]
#define F_CPU (4000000UL)

  // кількість зсувів праворуч для переміщення старшої тетради у молодшу
#define HI_NIBBLE_SHIFT 0x04
  // Маска для виділення молодшої тетради
#define LO_NIBBLE_MASK 0x0F // 0b00001111 == 0x0F
  // Константа для виконання корекції BCD-числа при досягенні межі 0xXA
#define BCD_CORRECTION 0x06
  // Маска для виділення стану вивода, до якого підключено кнопку +1
#define PLUS_BUTTON_MASK 0x40
  // Затримка для утримання кнопки +1 перед початком автоінкременту
#define AUTOINC_DELAY 2
  // Маска для ініціалізації порту E
#define PE_INIT_MASK 0x3F // 0b00111111 == 0x3F
  // Маска для гасіння індикатора, враховано два старших біта,
  // біти що використовуються для включения резисторів, що підтягують, на входах PE6 і PE7
#define INDICATOR_DESELECT_MASK 0xC0 // 0b11000000 == 0xC0
  // Частота оновлення індикатора у Гц.
#define REFRESH_RATE 100
  // Коефіціент передподільника Т/С1.
#define PRESCALER 64

  // Сегменти індикатора.
#define A 1    // 0b00000001 == 0x01
#define B 2    // 0b00000010 == 0x02
#define C 4    // 0b00000100 == 0x04
#define D 8    // 0b00001000 == 0x08
#define E 16   // 0b00010000 == 0x10
#define F 32   // 0b00100000 == 0x20
#define G 64   // 0b01000000 == 0x40
#define DP 128 // 0b10000000 == 0x80

  // Таблиця перекодування у "семисегментний" код.
const unsigned char LED_CODE[] = {
  A | B | C | D | E | F, // 0x3F -> '0'
  B | C,                 // 0x06 -> '1'
  A | B | D | E | G,     // 0x5B -> '2'
  A | B | C | D | G,     // 0x4F -> '3'
  B | C | F | G,         // 0x66 -> '4'
  A | C | D | F | G,     // 0x6D -> '5'

  A | C | D | E | F | G,     // 0x7D -> '6'
  A | B | C,                 // 0x07 -> '7'
  A | B | C | D | E | F | G, // 0x7F -> '8'
  A | B | C | D | F | G      // 0x6F -> '9'
};

  // Граничне значення T/C1, за якого індикатор (повністю) оновлюється:
  // REFRESH_RATE разів у секунду.
const unsigned int TIMER_MAX = F_CPU / PRESCALER / REFRESH_RATE / N_DIGITS;


  // Число оновлень індикатора, після якого відбувається оновлення маски миготіння, простіше – період миготіння.
const unsigned int BLINK_MAX = 150;
  // Лічильник оновлень.
unsigned char updateCounter = 0;
  // Маска миготіння.
unsigned char blinkMask = 0xFF;
  // Нова маска миготіння.
unsigned char newBlinkMask = 0xFF;

  // Масив для зберігання поточного часу у BCD-коді.
unsigned char time[3] = {
    0, // секунди
    0, // хвилини
    0  // години
};

  // Остання оновлена цифра.
unsigned char lastRefreshedDigit = 0;
  // Режим.
unsigned char mode = MODE_EDIT_HOURS;
  // Лічильник утримання кнопки. */
unsigned char holdCounter = 0;


  /*
  * Функція інкремента заданого значения у масиві.
  * index – індекс у масиві часу
  * limit – максимальне значення (у BCD-коді).
  * Повертає TRUE, якщо у поточній позиції відбулося переповнення, яке може
  * означати, що необхідно оновити також інші позиції.
  */
unsigned char incTimePosition(unsigned char index, unsigned char limit)
{
  if (index < N_DIGITS >> 1)  // N_DIGITS >> 1 == 6 / 2
  {
    time[index]++;

      //Лічба відбувається в BCD-коді. Якщо досягли значення 0x0A, то необхідно додати 0x06, щоб отримати 0xY0, де Y == X + 1.
    if ((time[index] & LO_NIBBLE_MASK) == 0x0A) //(0x0A що в десятковій системі є 10)
    {
        // Наприклад, 0x39 + 0x01 = 0x3A, а потрібно 0x40, тому 0x3A + 0x06 = 0x40.
      time[index] += BCD_CORRECTION;
    }
      // У випадку досягнення граничного значення лічби -> почати з 0.
    if (time[index] == limit)
    {
      time[index] = 0;
      return TRUE;
    }
  }
  return FALSE;
}

  // Функція інкременту значення годин.
void incHours()
{
  incTimePosition(I_HOURS, LIMIT_HOURS);
}

  // Функція інкременту значення хвилин.
void incMinutes()
{
    // Переповнено значенння хвилин інкремент годин.
  if (incTimePosition(I_MINUTES, LIMIT_MINUTES))
  {
    incHours();
  }
}

  // Функція інкременту значення секунд.
void incSeconds()
{
    // Переповнено значенння секунд інкремент хвилин.
  if (incTimePosition(I_SECONDS, LIMIT_SECONDS))
  {
    incMinutes();
  }
}

  // Функція інкременту показань годин у режимі налаштування.
void setupIncTime()
{
  if (mode == MODE_EDIT_HOURS)
  {
    incTimePosition(I_HOURS, LIMIT_HOURS);
  }
  else if (mode == MODE_EDIT_MINS)
  { 
    incTimePosition(I_MINUTES, LIMIT_MINUTES);
  }
  else
  {
    incTimePosition(I_SECONDS, LIMIT_SECONDS);
  }
}


  // Оновлює цифри на індикаторі.
void refreshDigit()
{
    // lastRefreshedDigit / 2 -> індекс у масиві time[].
  unsigned char index = lastRefreshedDigit >> 1;

    /* Старша чи молодша тетрада?
    * Якщо старша тетрада, то виконати зсув на 4 біта праворуч,
    * щоб правильно виконати виведення у порт Е.
    * За допомогою конструкції (lastRefreshedDigit & 0x01) виконується перевірка на парність.
    */
  unsigned char shift = (lastRefreshedDigit & 0x01) ? HI_NIBBLE_SHIFT : 0x00;

    // Визначаємо, чи потрібна крапка (у крайній правій позиції – не потрібна).
  unsigned char point = (shift == 0 && index > 0) ? DP : 0x00;
    // Гасимо індикатор.
  PORTE &= INDICATOR_DESELECT_MASK;
    // Виведення чергової цифри в порт D.
  PORTD = LED_CODE[(time[index] >> shift) & LO_NIBBLE_MASK] | point;

    // Вибір позиції на індикаторі з урахуванням маски миготіння.
  PORTE |= (1 << lastRefreshedDigit) & blinkMask;

    // Після оновлення останньої цифри -> почати з початку.
  if (++lastRefreshedDigit == N_DIGITS)
  {
      /*
      * Оновлення маски миготіння на останній цифрі, інакше можливе
      * несинхронне миготіння цифр, коли одну із них
      * ще не видно, а інша використовує нову маску і її видно,
      * та навпаки.
      */
    blinkMask = newBlinkMask;
    lastRefreshedDigit = 0;
  }
}


  // Обробник переривань від асинхронного таймера T/C0. (таймер реального часу)
ISR(TIMER0_OVF_vect)
{
  if (!mode)  // Якщо не у режимі налаштування (!0, 0 робочий режим)
  {
    incSeconds();
  }
}

  // Обробник переривань від Т/C1. (таймер для оновлення годинника)
ISR(TIMER1_COMPA_vect)
{
  refreshDigit(); // Оновити цифру на індикаторі.

  if (mode == 0)  // Якщо в рабочому режимі більше нічого не робити.
  {
    return;
  }

  updateCounter++; // Інкремент лічильника оновлень

  if (updateCounter == BLINK_MAX) // Якщо лічильник оновлень досяг значенння BLINK_MAX -> оновити маску видимості.
  { 
      // Кнопку "+" натиснуто?
    if (PINE & PLUS_BUTTON_MASK)  
    { 
      /* Не натиснуто. */
      
        // Інвертувати 2 розряди у масці => 00110000b – години, 00001100b – хвилини, 00000011b – секунди.
      newBlinkMask ^= ((3 << ((mode - 1) << 1)));
        // Скинути лічильник утримання кнопки "+1".
      holdCounter = 0;
    }
    else
    {
      // Натиснуто.
      
        // Якщо кнопка "+1" утримується 4 періоди миготіня, почати інкремент часу.
      if (holdCounter == AUTOINC_DELAY)
      {
        setupIncTime();
      }
      else
      {
        holdCounter++;
      }
        // У режимі автоінкременту відключити миготіння.
      newBlinkMask = 0xFF;
    }
      // Скидання лічильника оновлень.
    updateCounter = 0;
  }
}


  // Ініціалізація асинхронного таймера
void initAsyncTimer()
{
  TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));  // Заборона переривань від T/C0.

  ASSR |= (1 << AS0);                       // Дозволити асинхронний режим.
  TCNT0 = 0x00;                             // Почати лічбу з нуля.
  TCCR0 |= (1 << CS00) | (1 << CS02);       // Встановити коефіціент дільника частоти 128.
  while (ASSR & 0x07);                      // Очікування оновлення регістрів.

  TIMSK |= (1 << TOIE0);                    // Дозволити переривання після переповнення T/C0.
} 

  // Налаштування портів введення/виведення
void initPorts() // 1.5.1.2.0
{
    // Налаштування порту D на виведення.
  DDRD = 0xFF;
  PORTD = 0xFF;
    // Налаштування порту Е: PE0...PE5 – на виведення, PE6, PE7 – на введення.
  DDRE = PE_INIT_MASK;
  PORTE = 0xFF;
}

  // Налаштування зовнішніх переривань для кнопок
void initExternalInt()
{
    // Кнопки пов'язано з перериваннями INT6 та INT7.
  EIMSK |= (1 << INT7) | (1 << INT6);
    // Переривання за фронтом.
  EICRB |= (1 << ISC71) | (1 << ISC70) | (1 << ISC61) | (1 << ISC60);
}

  // Налаштування T/C1
void initTimer1()
{
    // WGM11, WGM10 <- 0: режим скидання лічильника T/C1 при співпадінні
    // WGM13 <- 0, WGM12 <- 1, CS <- 3: джерело опорної частоти == CLKio/64.
  TCCR1B |= 0x0B;
  
  TCNT1 = 0x00;             // Почати лічбу з нуля.
  OCR1A = TIMER_MAX;        // Граничне значення лічби таймера.
  TIMSK |= (1 << OCIE1A);   // Дозволити переривання для каналу порівняння А T/C1.
}


  // Обробник переривань від кнопки "Setup".
ISR(INT7_vect)
{
  if (mode == MODE_WORK)
  {
    mode = MODE_EDIT_HOURS; //Перейти до налаштування значення годин. (режим 0 -> 4)
  }
  else
  {
    mode--; // Перейти до іншого режиму (4, 3, 2, 1)
  }

    //Скинути маску миготіння, щоб минулий налаштований розряд не залишився невидимим.
  newBlinkMask = 0xFF;
  updateCounter = 0x00;   // Обнулити лічильник миготіння.
  
    // Після завершення налаштування реініціалізуємо таймер,
    // щоб перша секунда була повною, інакше можливий варіант,
    // коли після виходу із режиму налаштування значення секунд
    // інкрементується одразу.
  if (mode == MODE_WORK)
  {
    initAsyncTimer();
  }
}

  //Обробник переривань від кнопки "+1".
ISR(INT6_vect)
{
    // Якщо у робочому режимі нічого не робити.
  if (mode == MODE_WORK)
  {
    return;
  }

  setupIncTime();         // Інкрементувати показання у поточній позиції.
  newBlinkMask = 0xFF;    // Не блимаємо.
  updateCounter = 0x00;   // Якщо кнопку натиснуто обнулити лічильник миготіння.
}


// Ініціалізація периферії
void init() // 1.5.1.0
{
  WDTCR = 0x10;       // Вимкнути Watchdog (можливо не знадобиться).
  initPorts();        // Налаштування портів ВВ/BИB.
  initAsyncTimer();   // Переведення T/C0 в асинхронний режим.
  initExternalInt();  // Налаштування зовнішніх переривань для кнопок.
  initTimer1();       // Налаштування T/C1.
}


int main()
{
    // Ініціалізація периферії
  init();

    // Дозволити глобальні переривання.
  sei();
  
  while (1);
  return 0;
}