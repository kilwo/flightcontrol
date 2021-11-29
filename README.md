# Flight control box

Flight control box is a button box, designed for flight simulators, that appears as a joystick. 

No drivers or software required to be running on the computer. Appears as a USB Joystick which can be managed in MS Flight Simulator 2020 as the second joystick.

Supports:
* Analogue sliders - Mapped to the Joystick analogue axes.
* Buttons - Mapped 1 to 1 to the Joystick buttons.
* Switches - Each switch is mapped to 2 Joystick buttons. When the switch is changed, one of the two buttons will be signaled for about .2 sec. This allows a switch to send "Gear Up" one one button and "Gear Down" on another button.
* Rotary encoders - Clicking the encoder is mapped to a Joystick button. Rotation of the encoder is mapped to 2 Joystick buttons, an "Up" and a "Down". For each detent on the encoder, either the up or down button is signaled for about .2 sec. These are then mapped to the "Up" and "Down" actions. EG, autopilot altitude up and down. 

Therefore, in my configuration, 10 physical buttons, 2 switches, and 4 rotary encoders, map through to 24 buttons on the USB Joystick.

# How to use it

The code can be compiled using PlatformIO and Visual Studio Code and will run on a Arduino Leonardo Pro Micro 5V Atmega32U4. 

# If you use it

If you have any updates or suggestions, or if you build a cool implementation, please share.

# Stuff I used

Shout out to:
* Best-Microcontroller-Projects - This site has a great article on decoding rotary encoders. https://www.best-microcontroller-projects.com/rotary-encoder.html
* Arduino Joystick Library by Matthew Heironimus - Great library that does all the joystick work. https://github.com/MHeironimus/ArduinoJoystickLibrary