#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 20000000
// Define control pins and data pins
#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define E PORTDbits.RD2
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7


// Configuration bits
__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_OFF & CP_OFF);

// Function to send a command to the LCD
void LCD_command(unsigned char command) {
    PORTD = (command & 0xF0);   // Send the high nibble first
    RD0 = 0; // RS (Register Select) = 0 for command
    RD1 = 0; //RW
    RD2 = 1; // EN (Enable) = 1 to latch data
    
    __delay_ms(1);
    RD2 = 0; // EN = 0 to complete the command
    __delay_ms(1);

    PORTD = ((command << 4) & 0xF0); // Send the low nibble
    RD2 = 1; // EN = 1 to latch data
    RD0 = 0; // RS (Register Select) = 0 for command
    RD1 = 0; //RW
    __delay_ms(1);
    RD2 = 0; // EN = 0 to complete the command
    
}

// Function to initialize the LCD
void LCD_initialize() {
    TRISD = 0x00; // Set PORTB as output
    __delay_ms(15); // LCD power-up delay
    LCD_command(0x02); // Initialize LCD in 4-bit mode
    LCD_command(0x28); // 4-bit, 2-line, 5x7 font
    LCD_command(0x0E); // Display ON, cursor ON
    LCD_command(0x0C);
    LCD_command(0x01); // Clear the display
    __delay_ms(2);
    LCD_command(0x06); // Entry mode: Increment cursor, no shift
    LCD_command(0x80);
}

// Function to display a character on the LCD
void LCD_char(unsigned char data) {
    PORTD = (data & 0xF0); // Send the high nibble first
    RD0 = 1; // RS (Register Select) = 1 for data
    RD1 = 0; //RW
    RD2 = 1; // EN (Enable) = 1 to latch data
    __delay_ms(1);
    RD2 = 0; // EN = 0 to complete the data
    __delay_ms(1);

    PORTD = ((data << 4) & 0xF0); // Send the low nibble
    RD2 = 1; // EN = 1 to latch data
    RD0 = 1;
    RD1 = 0; //RW
    __delay_ms(1);
    RD2 = 0; // EN = 0 to complete the data

}

// Function to display a string on the LCD
void LCD_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        LCD_char(str[i]);
    }
}

void main() {
    LCD_initialize(); // Initialize the LCD

    // Display "Hello, World!" on the LCD
    
    LCD_command(0x80); // Move the cursor to beginning of first line   
    LCD_string("Railway Crossing");
    LCD_command(0xc0);
    LCD_string("gate");
    TRISD=0x00;
    TRISC=0b11111111;
    TRISB=0b00000000;
    PORTB=0b00010100;
while(1){
if(PORTC==1){
LCD_command(0x80);
LCD_string("Attantion train");
LCD_command(0xc0);
LCD_string("Near to crossing");
__delay_ms(10);
PORTB=0b10101001;
__delay_ms(10);
PORTB=0b00001001;
__delay_ms(1000);
LCD_command(0x80);
LCD_string("Please Wait");
LCD_command(0xc0);
LCD_string("Gate is Closed");
}
if(PORTC==2){
LCD_command(0x80);
LCD_string("Star yor Car");
LCD_command(0xc0);
LCD_string("Gate is Opening");
__delay_ms(10);
PORTB=0b11000010;
__delay_ms(10);
PORTB=0b00010010;
__delay_ms(1000);
PORTB=0b00010100;
LCD_command(0x80);
LCD_string("Thank you");
LCD_command(0xc0);
LCD_string("Gate is Open");
}
}
    return;

}