# FM������

![ǰ��](<Doc/Pictures/main.jpg>) 

## ��ʲô��
����Ƶ����ͨ��FM���Ƴ�ȥ��Ƶ�ʷ�Χ70~108MHz��  
�ҹ���̨���и����������������������װ�ã����԰���������Ϊһ��������ȣ�������ʡ��һ���������졣
������ʵ�ĳ���ֻ���������Ļ������������װ�ý�MP3��ŵ��������ϡ�

## ����
1. Ƶ�ʵ��ڣ�70~108MHz����0.1MHz��
2. ���ù��ܣ�������������������ʾ���ȡ�
3. �߼����ܣ�ALC(Automatic Level Control)�ȵȡ�

## ���ϸ��
1. FM��ƵоƬʹ��KT0803L�����ܱȽϷḻ���������ڵ�Ƭ�������������
2. LED����ܿ���оƬʹ��TM1650����ֽ���˵�Ƭ��������ʵ���˰������ɡ�
3. ���ػ���ʹ��STC8G�������ڱ�������û��������ROM������ʹ���˵�Ƭ���Դ���EEROM��
4. ����LEDʹ����ɫ����ܣ���Ҫ5V��������FMоƬ��Ҫ3.3V������ѹ����Ҫһ��I2C�ĵ�ƽת���߼���
5. PCBע�����ֵ�ƽ���ģ���ƽ��ֿ������ҵ�����ͨ��

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