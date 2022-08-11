# PID 

# *P*roportional 
# *I*ntegrator &
# *D*erivative 

# Controller

Materials Used:
* Arduino Uno
* SN754410 H-Bridge IC 
* Quadrature Encoder with 1 Optical Sensor
* 12V DC Motor (150 RPM)
* 12 VDC Power Supply 
* Breadboard and Jumper Wires
* Various peices from an Erector Set (to mount encoder over encoder wheel)

Implementation of PID Controller using Arduino to control a geared 12V DC motor position like a servo motor, 
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

