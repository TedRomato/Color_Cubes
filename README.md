# Color_Cubes
  <h1>What are color cubes:</h1>
   <p>Color cubes is an rgb lamp, but unlike other rgb lamps, it has got 2 working modes and more intuitive controls. The lamp is composed of 4 individual nodes. You can set   color for each node, to get just the color combination, that suits your current mood. If you are tired of watching static colors, you could change lamp mode to dynamic, which makes your colors move smoothly throughout all nodes, while still keeping your color selection.</p>
  <h1>Code and resources to build potenciometer controlled color cubes.</h1>
  <h2>How to control:</h2>
  <p>There are two modes: static and dynamic. You can switch between those two using lever switch. In static mode you can change color of individual cubes, with pots. One sets   which cube is being set and second one is used to select color. When you switch to dynamic mode, colors start to change using your pattern made in static mode. Once in  static mode pot one changes brightness and second one changes speed.</p>
  <h2>Code:</h2>
  <p>You can find code with explenations in Code.ino file.</p>
  <h2>Printer:</h2>
  <p>I used my Ender 3 for this print. I used white pla for outer part, so led's can glow through it, and black pla for the inner part, so the colors don't mix between cubes.
Unfortunately, I lost files for the control box, so I can't provide STL's, but it should be fairly simple to make. Youcan even go fancy, and make it out of wood. You can
see the footage folder for some inspiration. <br>Note: I used white pla instead of black forr inner parts of lamp, because I ran out of black one.</p>
  <h2>What do I need:</h2>
  <ul>
    <li>3D Printer</li>
    <li>Black PLA</li>
    <li>White PLA</li>
    <li>Gluegun</li>
    <li>Power source for your board</li>
    <li>
      <h3>Components:</h3>
      <ul>
        <li>Arduino UNO/Nano or any other arduino compatible board</li>
        <li>2x 10k potenciometers</li>
        <li>(any amount divisible by 4) Neopixel RGB leds (you can grab some here: https://www.adafruit.com/product/1655)</li>
        <li>2x 10k resistors</li>
        <li>2x 1k resistors</li>
        <li>1x lever switch</li>
        <li>Cables</li>
      </ul>
    </li>
  </ul>
  <h2>Diagram:</h2>
  <img src="https://github.com/TedRomato/Color_Cubes/blob/main/Schematic_Cubes_2021-03-30.png" width="480px">
  <h2>Putting it all together:</h2>
  <p>First we print our parts -> inner lamp, outer lamp and your control box parts. Then, we decide how many led's do we want to have in each cube. The more led's the more it shines.
Alter Code.ino LED_COUNT variable so it maches with amount of diodes you use.
Place your led's into the inner lamp part as you can see in the picture. Solder it together according to diagram with some wires throug the holes, and stick them in place using glue gun. Assemble, and solder your controll box. Connect your led's with the control box according to diagram. Now your color cubes should be working. :]</p>
