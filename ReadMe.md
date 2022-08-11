# PID 

# ***P***roportional 
# ***I***ntegrator &
# ***D***erivative 

# Controller [of Error]

![equation](http://www.sciweavers.org/tex2img.php?eq=u%28t%29%20%3D%20%20K_%7Bp%7D%2Ae%28t%29%20%2B%20K_%7Bi%7D%20%5Cint_a%5Eb%20%20e%28t%29dt%20%2B%20%20K_%7Bp%7D%20%20%5Cfrac%7Bde%7D%7Bdt%7D%20%0A%0A%20&bc=White&fc=Black&im=png&fs=12&ff=arev&edit=0)


Materials Used:
* Arduino &#174; Uno
* SN754410 H-Bridge IC 
* Quadrature Encoder with 1 Optical Sensor
* 12V DC Motor (150 RPM)
* 12 VDC Power Supply 
* Breadboard and Jumper Wires
* Various peices from an Erector Set &trade; (to mount encoder over encoder wheel)

Implementation of PID Controller using Arduino to control a geared, 12V DC motor position so like a servo motor, 
fitted with a single optical encoder. Although, the motor can be driven forward or back ward using the *SN754410* H-Bridge IC, 
the program is unable to backtrack when the target is overshot due to there only being one optical sensor on the quaderature endocer
attached to the motor and it seems to work best with the i-term at left at 0.00 to avoid overshooting the target.   

I would also like to consturct a speed controlled PID loop; however, the motor I used for this project is geared down significantly 
(1:401 reduction ratio) for torque and better suited for lower speeds. 
I thought about using a small an large gear speed it up but due to the difficulty of mounting the encoder using a higher rpm motor seems less troublesome. (More to come on this.)
I will post this project once it's complete. 

Another idea I had was to use the speed controlled motor to generatea control voltage envelope that could be patched to 
a voltage controlled amplifier to generate a tremolo or pitch shifting effect. Operating on the same concept as the encoder––passing light 
through a paper disc attached to the motor shaft. By controlling the motor speed and experimenting with different shapes 
of holes in the paper, the photodiode on the other side will recieve varying voltages as the light changes in intensity. 


![Arduino Serial Plotter](https://github.com/staceywhitmore-inl/PidControler/blob/master/media/PidSerialPlotter.PNG?raw=true)
*Here is a screenshot of the Arduino IDE Serial Plotter Tool plotting the actual position as it approaches the targe position 
(blue line at top).*


![All Materials Used to build PID DC Motor Position Controller](https://github.com/staceywhitmore-inl/PidControler/blob/master/media/IMG-8604.jpg?raw=true)
*Materials used in project.*

 
![Geared DC Motor with Quaderature Encoder attached using Erector Set pieces](https://github.com/staceywhitmore-inl/PidControler/blob/master/media/IMG-8605.jpg?raw=true)
*Arduino pins and Quaderature Encoder mounted over wheel from Erector Set &trade; pieces.*


Breadboard and IC layout.
![Close up of breadboard layout with SN754410 IC connections](https://github.com/staceywhitmore-inl/PidControler/blob/master/media/IMG-8606.jpg?raw=true)
*(Notice the VCC2 and 3,4EN DIP pins (pins 8 & 9) broke off the H-bridge in transit and had to be reattached. Luckily, I was able to carefully solder on some replacement feet without further damaging the chip!)*

