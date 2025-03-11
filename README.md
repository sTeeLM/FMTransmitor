# FM发射器

![前脸](<Doc/Pictures/main.jpg>) 

## 有什么用
将音频输入通过FM调制出去，频率范围70~108MHz。  
我工作台上有个德生的收音机，有了这个装置，可以把收音机变为一个外放喇叭，这样就省了一个外置音响。
另外老实的车上只有收音机的话，可以用这个装置将MP3外放到收音机上。

## 功能
1. 频率调节：70~108MHz步长0.1MHz。
2. 常用功能：调节音量、静音、显示亮度。
3. 高级功能：ALC(Automatic Level Control)等等。

## 设计细节
1. FM射频芯片使用KT0803L，功能比较丰富，基本都在单片机上做了配置项。
2. LED数码管控制芯片使用TM1650，充分解放了单片机，而且实现了按键自由。
3. 主控还是使用STC8G，并且在本制作中没有用外置ROM，而是使用了单片机自带的EEROM。
4. 由于LED使用蓝色数码管，需要5V驱动，而FM芯片需要3.3V工作电压，需要一个I2C的电平转化逻辑。
5. PCB注意数字地平面和模拟地平面分开，并且单点连通。

## What is it used for?
It modulates audio input through FM transmission, with a frequency range of 70~108MHz.  
I have a Tecsun radio on my workbench, and with this device, the radio can be turned into an external speaker, saving the need for an external sound system.
Additionally, if an older car only has a radio, this device can be used to play MP3s through the car's radio.

## Features
1. **Frequency adjustment**: 70~108MHz with a step size of 0.1MHz.
2. **Common functions**: Volume adjustment, mute, display brightness.
3. **Advanced functions**: ALC (Automatic Level Control), etc.

## Design Details
1. The FM radio frequency chip uses **KT0803L**, which is quite feature-rich, with most configurations done on the microcontroller.
2. The LED digital tube control chip uses **TM1650**, which fully liberates the microcontroller and also achieves key freedom.
3. The main control still uses **STC8G**, and in this production, no external ROM is used; instead, the microcontroller's built-in EEROM is utilized.
4. Since the LED uses a blue digital tube that requires **5V** to drive, and the FM chip requires a **3.3V** operating voltage, an **I2C level conversion logic** is needed.
5. The PCB design ensures that the **digital ground plane** and **analog ground plane** are separated and connected at a single point.