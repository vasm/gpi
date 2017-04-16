#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL // 1 MHz
#include <util/delay.h>

#define sbi(x,y) x |= _BV(y) //set bit - using bitwise OR operator 
#define cbi(x,y) x &= ~(_BV(y)) //clear bit - using bitwise AND operator
#define tbi(x,y) x ^= _BV(y) //toggle bit - using bitwise XOR operator
#define is_high(x,y) (x & _BV(y) == _BV(y)) //check if the y'th bit of register 'x' is high ... test if its AND with 1 is 1

/* _BV(a) is a macro which returns the value corresponding to 2 to the power 'a'. Thus _BV(PX3) would be 0x08 or 0b00001000 */

/*** config begin ***/
volatile uint8_t* digit_port = &PORTA;
uint8_t digit_port_offset = 1;
const int ngears = 7;
uint16_t gear_voltage[] = {983, 274, 361, 493, 643, 814, 899}; // N, 1, 2, 3, 4, 5, 6
uint8_t gears[] = {10, 1, 2, 3, 4, 5, 6};
#undef _hold_last_known_gear // if input voltage is unknown, a '-' sign will be displayed
/*** config end   ***/

void init_adc()
{
    ADCSRA |= _BV(ADEN);
    _delay_ms(1);

    ADCSRA = 0x8F;            // Enable the ADC and its interrupt feature
                              // and set the ACD clock pre-scalar to clk/128
    ADMUX |= _BV(REFS1) | 0b00000111; // select internal 1.1V as reference; select ADC7 as input
    
}

void display_digit(uint8_t d)
{
    #define _ndigits 12
    d = d % _ndigits;

    // Sequence: PB0 PB1 PA2 PA3 PA5 PB3 PA6

    //     PB0
    // PB3     PB1
    //     PA6
    // PA5     PA2
    //     PA3

    static uint8_t digits_pa[_ndigits] = {
        _BV(2) | _BV(3) | _BV(5),          _BV(2),                   _BV(6) | _BV(5) | _BV(3),          // 0, 1, 2
        _BV(2) | _BV(3) | _BV(6),          _BV(2) | _BV(6),          _BV(2) | _BV(3) | _BV(6),          // 3, 4, 5
        _BV(2) | _BV(3) | _BV(5) | _BV(6), _BV(2),                   _BV(2) | _BV(3) | _BV(5) | _BV(6), // 6, 7, 8
        _BV(2) | _BV(3) | _BV(6),          _BV(2) | _BV(6) | _BV(5), _BV(6)
    };

    static uint8_t digits_pb[_ndigits] = {
        _BV(0) | _BV(1) | _BV(3),  _BV(1),                      _BV(0) | _BV(1),          // 0, 1, 2
        _BV(0) | _BV(1),           _BV(1) | _BV(3),             _BV(0) | _BV(3),          // 3, 4, 5  
        _BV(0) | _BV(3),           _BV(0) | _BV(1) | _BV(3),    _BV(0) | _BV(1) | _BV(3), // 6, 7, 8
        _BV(0) | _BV(1) | _BV(3),  0, 0
    };
    
    PORTA = ~digits_pa[d];
    PORTB = ~digits_pb[d];
}

/*ADC Conversion Complete Interrupt Service Routine (ISR)*/
ISR(ADC_vect)
{
    uint8_t l = ADCL;
    uint8_t h = ADCH;
    uint16_t voltage = h << 8 | l;

    int gear_num = 0;
    for (; gear_num < ngears; ++gear_num) {
        if (voltage > gear_voltage[gear_num] - 40 &&
            voltage < gear_voltage[gear_num] + 40)
        {
            display_digit(gears[gear_num]);
            break;
        }
    }

    #ifndef _hold_last_known_gear
    if (gear_num >= ngears) 
        display_digit(11); // "-"
    #endif

    ADCSRA |= 1 << ADSC;      // Start Conversion
}

void main(void)
{
    init_adc();

    DDRA = 0b01111110;
    DDRB = 0xff;

    sei(); // Enable Global Interrupts
    ADCSRA |= 1 << ADSC; // Start Conversion
 
    while (1) {}
}
