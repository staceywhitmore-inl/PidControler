# PID 

# *P*roportional 
# *I*ntegrator &
# *D*erivative 

# Controller

Materials Used:
* Arduino Uno
* SN754410 H-Bridge IC 
* Quadrature Encoder with 1 Optical Sensor
* Geared Motor
* 12 VDC Power Supply
* Breadboard and Jumper Wires
* Various peices from an Erector Set (to mount encoder over encoder wheel)

Implementation of PID Controller using Arduino to control a geared 12V DC motor position like a servo motor, 
fitted with a single optical encoder. Although, the motor can be driven forward or back ward using the *SN754410* H-Bridge IC, 
the program is unable to backtrack when the target is overshot due to there only being one optical sensor on the quaderature endocer
attached to the motor and it seems to work best with the i-term at left at 0.00 to avoid overshooting the target.   
