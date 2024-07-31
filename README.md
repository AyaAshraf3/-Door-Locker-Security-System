# -Door-Locker-Security-System
This system is designed to enhance door security using a password-based access control mechanism.

# Introduction
The Door Locker Security System is a project that utilizes two ATmega32 microcontrollers to provide secure door access through a password. It offers features such as password creation, password verification, door locking/unlocking, and user-friendly interaction via an LCD and a keypad.

# System Specifications
Microcontroller 1 (Mc1):

HAL drivers: LCD and Keypad
MCAL drivers: GPIO, UART, Timer1
Microcontroller 2 (Mc2):

HAL drivers: Buzzer, EEPROM, and DC-motor
MCAL drivers: GPIO, I2C, UART, Timer0_PWM, Timer1
Architecture
The project follows a layered architecture model:

Mc1 (HMI_ECU) handles user interaction and interface components.
Mc2 (Control_ECU) manages the core system logic, including password verification and door control.

# Usage
1-Creating a System Password
2-Follow the on-screen instructions to set up a system password.
3-Main Options
4-Use the main menu to select options like opening the door or changing the password.
5-Open Door

Enter the password to unlock the door securely.
Change Password

Choose this option to set a new system password.
Password Mismatch Handling

The system handles mismatched passwords, locking the system after multiple failed attempts.
