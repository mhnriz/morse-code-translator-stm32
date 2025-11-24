
# **STM32 Morse Code Translator**
<div align="center">


![GitHub top language](https://img.shields.io/github/languages/top/mhnriz/morse-code-translator-stm32?style=for-the-badge&color=34D399)
![GitHub last commit](https://img.shields.io/github/last-commit/mhnriz/morse-code-translator-stm32?style=for-the-badge&color=6EE7B7)
![GitHub repo size](https://img.shields.io/github/repo-size/mhnriz/morse-code-translator-stm32?style=for-the-badge&color=34D399)
<br>

![Platform](https://img.shields.io/badge/platform-STM32-10B981?style=for-the-badge)
![Hardware](https://img.shields.io/badge/Hardware-STM32%20Nucleo-10B981?style=for-the-badge)

</div>

## 📖 **Overview**

The **STM32 Morse Code Translator** is an embedded system using an **STM32 Nucleo board**, **Mbed OS**, and a **16×2 LCD**.  
It captures dot and dash button inputs, decodes Morse sequences, and displays translated text on the LCD.

---

## ⚙️ **Features**

- 🔘 Dot & Dash Input with LED indicators  
- 🔄 Automatic detection of letter separation using timing  
- 🔤 Decodes Morse into characters in real time  
- 📟 Displays output on 16×2 LCD via TextLCD library  
- 🔌 Clean Mbed OS implementation  

---

## 🧩 **Project Structure**

```
morse-code-translator-stm32/
├── main.cpp
├── TextLCD.lib
├── mbed.bld
└── .gitignore
```

---

## 🔧 **Hardware Requirements**

| Component | Notes |
|----------|-------|
| STM32 Nucleo | e.g., Nucleo-F401RE |
| 16×2 LCD | HD44780-compatible |
| Push Buttons | Dot & Dash input |
| LEDs | Input indicators* |
| Jumper Wires | For connections |
| Breadboard | Optional |

*LEDs powered in series with button

---

## 🛠 **Pin Configuration**

Defined via main.cpp

---

## ❔ **How to Use**

- Press **dot button** → `.`  
- Press **dash button** → `-`  
- Pause briefly to auto-decode the character  
- LCD updates continuously with the decoded message  

---

## 🚀 **Getting Started**

### Clone Repository
```bash
git clone https://github.com/mhnriz/morse-code-translator-stm32.git
```

### Import to Mbed Studio or Keil Studio 
1. go to https://studio.keil.arm.com/
2. Install TextLCD library (refer below)
3. Open project
4. Select STM32 Nucleo target
5. Build & flash  

### TextLCD library
```bash
http://os.mbed.com/users/simon/code/TextLCD/
```
---

## 🖥️ **Technologies Used**

- C++ (Embedded)
- Mbed OS
- TextLCD Peripheral Library
- STM32 Nucleo

---
