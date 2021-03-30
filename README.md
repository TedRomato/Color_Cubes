# Color_Cubes
Code and resources to build potenciometer controlled color cubes.

How to control:
There are two modes: static, and dynamic. You can switch between those two using lever switch. In static mode you can change color of individual cubes, with pots. One sets which cube is being set and second one is used to select color. When you switch to dynamic mode, colors start to change using your pattern made in static mode. One pot in statick mode changes brightness and second one changes speed.

Code:
You can find code with explenations in Code.ino file.

Printer:
I used my Ender 3 for this print. I used white pla for outer part, so led's can glow through it, and black pla for the inner part, so the colors don't mix between cubes.
Unfortunately, I lost files for the control box, so I can't provide STL's, but it should be fairly simple to make. Youcan even go fancy, and make it out of wood. You can
see the footage folder for some inspiration.

What do I need:
3D Printer
Black PLA
White PLA
Gluegun
Power source for your board

Components:
Arduino UNO/Nano or any other arduino compatible board
2x 10k potenciometers
(any amount divisible by 4) Neopixel RGB leds (you can grab some here: https://www.adafruit.com/product/1655)
2x 10k resistors
2x 1k resistors
1x lever switch
Cables

Putting it all together:
First we print our parts -> inner lamp, outer lamp and your control box parts. Then, we decide how many led's do we want to have in each cube. The more led's the more it shines.
Alter Code.ino LED_COUNT variable so it maches with amount of diodes you use.
Place your led's into the inner lamp part as you can see in the picture. Solder it together according to diagram with some wires throug the holes, and stick them in place using glue gun. Assemble, and solder your controll box. Connect your led's with the control box according to diagram. Now your color cubes should be working. :]
