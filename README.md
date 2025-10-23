# 📟 LPC2148 Keypad and LCD Interface — Square Calculator

## 🧾 Project Overview  
This project interfaces a 3x3 matrix keypad and a 16x2 LCD with the LPC2148 ARM7 microcontroller.  
The program reads a two-digit number entered via the keypad, calculates its square, and displays both the input number and its square on the LCD.  

This is a reimplementation of the classic 8051 keypad-LCD square calculator, adapted for the LPC2148 ARM7 microcontroller.

---

## ⚙️ Features
- 3x3 matrix keypad input (digits 0–9, *, #)
- 16x2 LCD output display
- Displays prompt → user input → calculated square  
- Efficient keypad scanning algorithm  
- Works on 8-bit LCD mode using port P0.16–P0.23  

---

## 🧰 Components Required
| Component | Quantity | Description |
|------------|-----------|-------------|
| LPC2148 ARM7 Development Board | 1 | Main controller |
| 16x2 LCD Display | 1 | For text output |
| 3x3 Matrix Keypad | 1 | For numeric input |
| Jumper Wires | As required | For connections |
| Power Supply (5V) | 1 | For powering board and peripherals |

---

## 🔌 Pin Connections

### LCD Connections (16x2)
| LCD Pin | Function | LPC2148 Pin |
|----------|-----------|-------------|
| D0–D7 | Data lines | P0.16–P0.23 |
| RS | Register Select | P0.10 |
| RW | Read/Write | P0.11 |
| EN | Enable | P0.12 |

### Keypad Connections (3x3 Matrix)
| Keypad Pin | Function | LPC2148 Pin |
|-------------|-----------|-------------|
| Row 1–4 | Output | P1.16–P1.19 |
| Column 1–3 | Input | P1.20–P1.22 |

---

## 🧩 Working Principle
1. The LCD is initialized in **8-bit mode** and displays `"Enter No.:"`.  
2. The keypad scanning routine activates one row at a time while reading the column pins.  
3. Each pressed key is mapped using a predefined `key_map` array.  
4. Two digits are read sequentially to form a **two-digit number**.  
5. The program computes the **square** of the number.  
6. The result is displayed on the **second line of the LCD**.  

---

## 🧠 Core Logic Summary
- **Keypad scanning** → Detect key press using row-column short detection.  
- **LCD interfacing** → Control via RS, RW, EN pins and data bus on P0.16–P0.23.  
- **Data conversion** → Convert ASCII input to integer, calculate square, and convert back to ASCII for display.  

---

## 🧮 Example
| Key Inputs | LCD Output |
|-------------|-------------|
| Press 1 → 2 | `Enter No.:12` |
|              | `Square: 144` |

---

## 🧑‍💻 Code Structure
```c
main()            → Initializes LCD & keypad, reads two digits, calculates square  
get_key()         → Scans 3x3 keypad and returns the pressed key  
LCD_Command()     → Sends command byte to LCD  
LCD_Char()        → Sends data byte to LCD  
LCD_String()      → Prints strings to LCD  
LCD_Integer()     → Converts and displays integers  
delay()           → Software delay loop  
