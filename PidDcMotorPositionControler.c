/* PID 
Proportional 
Integrator 
Derivative

Controller (... of ERRor).
*/

#include <util/atomic.h> // For interrupt code used in ATOMIC_BLOCK

//#define Pins and vars 
const int ENCODER_PIN = 2; 
const int ENABLE_PIN = 9; 
const int MC2 = 5;          // Motor Control 2
const int  MC1 = 4;         // Motor Control 1
const int D_PIN_TO_INTERRUPT = digitalPinToInterrupt(ENCODER_PIN); // Translate digital pin 2 to Interupt ID # ( D2 --> 0 on Arduino UNO).

// Variables
volatile int fwdTicks = 0; // 20 ticks/rotation
long prevT = 0;
float ePrev = 0;
const int TARGET_POS = 1023; //1050;  // set target Position // .. X (sin(prevT / 1,000,000)); // <---- SET TARGET HERE (User lower number if counting by rotations).
long currentTime;
float deltaT;
float rate;
int dir; // 1, -1, 0 | forward, reverse, brake
volatile int rotations = 0;
int measPos = 0;
int err;
float eInteg = 0; // Integration of Error w/ respect to t
float prev_eInteg = 0; 
float dxOfErr;
float cntrlSig;

// Constants (P,I,D)  Fine tune and adjust for gain as needed. 
float kP = 0.98;      //PRESENT (Strong & Quick)   | Gain  Proportional Controller
float kI = 0.0;       //PAST    (Accounts for Error. Weak)  | Gain of Integrator
float kD = 0.027;     //FUTURE  (D-term Compensates for P-term) | Gain of Derivative Controller (i.e., How quickly approaching target.) (Controls overshoot). (Can make motor sound like it's constantly on).



/************* SETUP *****************************************/
void setup()
{
    Serial.begin(9600);
    pinMode(ENCODER_PIN, INPUT);  
    attachInterrupt(D_PIN_TO_INTERRUPT , encodeTicks, RISING );
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(MC1, OUTPUT);
    pinMode(MC2, OUTPUT);
    Serial.println("TARGET_POS \tmPos \tRotations");
}
/************* Close SETUP *****************************************/


/************* LOOP *****************************************/
void loop()
{  
    currentTime = micros(); // Gets # of uSecs since starting curr program. (Will overflow and go back to 0 after 70 min). 1 sec = 1,000,000 (1.0e6) microSec| 1 ms = 1,000 uSec. 
    deltaT = ((float)(currentTime - prevT)) / 1000000; // Current delta/chnge in time (relative to start of prog) by 1/1,000,000 Sec (i.e., a microsecond)
    prevT = currentTime; // Save previousTime to configure delta again on next loop iteration. 

    // Designate block of code to be run atomically (i.e., W/O interuption).
    // Upon exiting this block it restores vars to prior state Before entering the atomic block.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        measPos = fwdTicks; // Store curr fwd tick pos in  measured position. I
        //measPos = rotations;  
    }
   
    // P-term : Order terms according to wiring. (E.g., p(x)ly MeasuredPosition - TargetPosition)   
    err = TARGET_POS - measPos; //  (If err +pos ^ V duty cycle else lower duty ...)
    // ... e.g. err = pos - TARGET_POS; 
    
    // I-term : Integral of Err (Integration of Err w/ respect to t) 
    prev_eInteg = eInteg; // 1st Save previous integral of Err if trying Trapazoidal Method
    eInteg += (err * deltaT); // 1st order finte diff est. 
    // OR ... Take 2 pts to get Avg. (/2)
    eInteg = prev_eInteg + ((eInteg + prev_eInteg)/2) * deltaT;  // ..try Trapazoidal Method : Avg. M of line under curve * deltaT to get the area under the curve. (forms a trap w/ rounded edge gone --> easier to find area)

    // D-term : Derivative of Error (Controls overshoot/Reinforc noise (Osillation)). (Can be omitted when building PID Speed Controller.)
    dxOfErr = (err - ePrev) / deltaT; // Backward Difference Method
     // Issue: Motor const sounds like it is on whilst stopped p(x)ly due to Derivative Gain.   

    // Control signal : Used to derive rate and det. mag. (and polarity/dir) of PWM Voltage applied to H-Bridge Enable pin.
    //           P          I           D  
   cntrlSig = kP*err + kI*eInteg + kD*dxOfErr; 
   // ProportionalTerm + IntegralTerm + DerivativeTerm
   
   // rate should always be positive.
    rate = fabs(cntrlSig); // fabs(): F-loating Point ABS [Value]
    // Reset rate back to 255 (8-bit max)
    if (rate > 255)   
        rate = 255;    

    // Direction
    dir = 1; // 1 = FORWARD  >>> 
    if (cntrlSig < 0)  //  dir = -1;  // To rev. dir when cntrlSig drops below 0.
     
    // Pass vector info cntrl plant
    drive_h_bridge(dir, rate);

    // Save previous error
    ePrev = err;

    Serial.print(TARGET_POS); Serial.print("\t\t");  Serial.print(measPos); Serial.print("\t\t"); Serial.println(rotations);         
}
/***** CLOSE LOOP *** **********************************************************************/


// Drive Plant (i.e., Motor & Encoder setup).
void drive_h_bridge(int dir, int rate)
{      
    if (dir == -1)    
        reverse(rate);    
    else if (dir == 1)    
        forward(rate);     
    else    
        brake();    
}

void reverse(int rate)
{
    digitalWrite(ENABLE_PIN, LOW); // ALWAYS disable the current flow BEFORE changing switch states to ensure ensure a momentary short canNOT occur as the swiches flip in H-bridge
    digitalWrite(MC1, LOW);
    digitalWrite(MC2, HIGH);
    analogWrite(ENABLE_PIN, rate);
}

void forward(int rate)
{
    digitalWrite(ENABLE_PIN, LOW); // Always start LOW
    digitalWrite(MC1, HIGH);
    digitalWrite(MC2, LOW);
    analogWrite(ENABLE_PIN, rate);
}

void brake()
{
    digitalWrite(ENABLE_PIN, LOW); // " Start LOW 
    digitalWrite(MC1, LOW);
    digitalWrite(MC2, LOW);
    digitalWrite(ENABLE_PIN, HIGH);
}

// Called whenever encoder triggers an interrupt.
void encodeTicks()
{
    int a = digitalRead(ENCODER_PIN);     
     /*int b = digRd(EN2); if(b > 0)  fwdTick++; else fwdTick--;*/ // If Encoder has two sensors for bi-directional tracking. 
     // Count ticks (on rising edge). Alternatively, could count measure time elapsed between triggers (although more coarse could count pulses/timeInterval [but would work better @ ^er speed]);
    if (a > 0)    
        fwdTicks++;    
    if (fwdTicks % 20 == 0)
       rotations++;
}
