#include <lpc213x.h>   // LPC2138 specific header

#define LCD_DATA_MASK 0x00FF0000  // LCD data lines on P0.16–P0.23
#define RS (1 << 10)              // LCD RS pin on P0.10
#define RW (1 << 11)              // LCD RW pin on P0.11
#define EN (1 << 12)              // LCD EN pin on P0.12

#define ROWS_MASK (0x0F << 16)    // Keypad rows on P1.16–P1.19
#define COLS_MASK (0x07 << 20)    // Keypad columns on P1.20–P1.22

void delay(unsigned int ms);
void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_Init(void);
void LCD_String(char *str);
void LCD_Integer(int num);
char get_key(void);

int main(void) {
    char keys[2];
    int number, square;

    // Set LCD pins as output
    IO0DIR |= LCD_DATA_MASK | RS | RW | EN;

    // Set keypad rows as output, columns as input
    IO1DIR |= ROWS_MASK;     // P1.16–P1.19 output (rows)
    IO1DIR &= ~COLS_MASK;    // P1.20–P1.22 input (columns)

    LCD_Init();
    LCD_String("Enter No.:");

    keys[0] = get_key();
    LCD_Char(keys[0]);
    delay(100);

    keys[1] = get_key();
    LCD_Char(keys[1]);
    delay(100);

    number = (keys[0] - '0') * 10 + (keys[1] - '0');
    square = number * number;

    LCD_Command(0xC0);
    LCD_String("Square: ");
    LCD_Integer(square);

    while (1);
}

char get_key(void) {
    const char key_map[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };

    int row, col;

    while (1) {
        for (row = 0; row < 4; row++) {
            IO1DIR |= ROWS_MASK;           // Rows as output
            IO1SET = ROWS_MASK;            // Set all rows high
            IO1CLR = (1 << (16 + row));    // Pull one row low

            IO1DIR &= ~COLS_MASK;          // Columns as input

            delay(1);  // Settling delay

            if (!(IO1PIN & (1 << 20))) col = 0;
            else if (!(IO1PIN & (1 << 21))) col = 1;
            else if (!(IO1PIN & (1 << 22))) col = 2;
            else continue;  // No key in this row

            while (!(IO1PIN & (1 << (20 + col)))); // Wait for key release
            return key_map[row][col];
        }
    }
}

void LCD_Command(unsigned char cmd) {
    IO0CLR = LCD_DATA_MASK;
    IO0SET = (cmd << 16);     // Shift to P0.16–P0.23

    IO0CLR = RS | RW;         // RS = 0, RW = 0
    IO0SET = EN;
    delay(5);
    IO0CLR = EN;
}

void LCD_Char(unsigned char data) {
    IO0CLR = LCD_DATA_MASK;
    IO0SET = (data << 16);    // Shift to P0.16–P0.23

    IO0SET = RS;              // RS = 1
    IO0CLR = RW;              // RW = 0
    IO0SET = EN;
    delay(5);
    IO0CLR = EN;
}

void LCD_Init(void) {
    LCD_Command(0x38);  // 8-bit, 2-line
    LCD_Command(0x0C);  // Display ON, cursor OFF
    LCD_Command(0x06);  // Auto-increment
    LCD_Command(0x01);  // Clear display
    delay(10);
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_Integer(int num) {
    char buffer[6];
    int i = 0;

    if (num == 0) {
        LCD_Char('0');
        return;
    }

    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while (i > 0) {
        LCD_Char(buffer[--i]);
    }
}

void delay(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);  // ~1 ms delay at ~60 MHz
}
