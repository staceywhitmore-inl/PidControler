#include <util/atomic.h> // For the ATOMIC_BLOCK macro

#define ENCODER_PIN 2 
#define ENABLE_PIN 9 
#define MC2 5         // Motor Control 2
#define MC1 4         // Motor Control 1

// Variables
volatile int posI = 0;
long prevT = 0;
float ePrev = 0;
float dxOfErr;
float eInteg = 0; // Integration of Error w/ respect to t
// set target Position
int targetPos = 1050; 
// .. X (sin(prevT / 1,000,000));
long currentTime;
float deltaT;
int err;
float cntrlSig;
float rate;
int dir; // 1, -1, 0 | forward, reverse, brake
int measPos = 0;

// Constants (P,I,D)  Fine tune these as needed
float kP = 0.98;      //PRESENT (Strong & Quick)   | Gain or Proportional Controller
float kI = 0.0;       //PAST    (Accounts for Error. Weak)  | Gain of Integrator
float kD = 0.027;    //FUTURE  (D-term Compensates for P-term) | Gain of Derivative Controller 



/************* SETUP *****************************************/
void setup()
{
    Serial.begin(9600);
    pinMode(ENCODER_PIN, INPUT);  
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), encodeTicks, RISING);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(MC1, OUTPUT);
    pinMode(MC2, OUTPUT);
    Serial.println("targetPos \tpos");
}
/************* Close SETUP *****************************************/


/************* LOOP *****************************************/
void loop()
{  
    currentTime = micros(); // Get time stamp from start @ e/ loop.
    deltaT = ((float)(currentTime - prevT)) / 1000000; // 1.0e6;
    prevT = currentTime;

    // Designate block of code to be run Atomically (i.e., W/O Interuption).
    // Upon exiting this block it restores vars to prior state before entering the atomic block.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        measPos = posI; // Set measured Position to current posI.
    }
   
    // Order terms according to wiring.    
    err = targetPos - measPos; // MeasuredPosition - TargetPosition
    // ... e.g. err = pos - targetPos; 

    // Derivative of Error 
    dxOfErr = (err - ePrev) / deltaT;

    // Integral (Integration of Err w/ respect to t) 
    eInteg = eInteg + err * deltaT;

    // Control signal
    //           P          I           D  
   cntrlSig = kP*err + kI*eInteg + kD*dxOfErr; 
   
    rate = fabs(cntrlSig); // fabs(): Floating Point ABS [Value]
    // Reset rate back to 255 (8-bit max)
    if (rate > 255)   
        rate = 255;    

    // Direction
    dir = 1; // 1 = FORWARD  >>> 
    if (cntrlSig < 0)   // Rev dir when cntrlSig drops below 0.
        dir = -1;

    // vector         
    drive_h_bridge(dir, rate);

    // Save previous error
    ePrev = err;

    Serial.print(targetPos); Serial.print("\t");  Serial.println(measPos);         
}
/***** CLOSE LOOP *** **********************************************************************/



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
    digitalWrite(ENABLE_PIN, LOW); // Always disable the current flow BEFORE changing switch states to ensure ensure a momentary short canNOT occur as the swiches flip 
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

// Called whenever Encoder triggers an Interrupt
void encodeTicks()
{
    int a = digitalRead(ENCODER_PIN);     
     /*int b = digRd(EN2); if(b > 0)  posI++; else posI--;*/ // If Encoder has two sensors                                
    if (a > 0)    
        posI++;    
}
