/*  The intent of this program is for the student to explore how to do a round-robin cyclic executive task manager.
    To this end, this program queries the user across the serial port what LED and what 
    interval should that LED blink with. There are 2 LEDs, and each LED
    can blink at different rates. 
    Who: Theo Poulos
    When: 3/13/2026
*/

#define LEDPIN1 8
#define LEDPIN2 9

int LED1 = 0;                     // LED1 state
int LED2 = 0;                     // LED2 state

unsigned long interval1 = 0;      // LED1 blink interval
unsigned long interval2 = 0;      // LED2 blink interval

unsigned long delaystart1 = 0;    // last LED1 toggle time
unsigned long delaystart2 = 0;    // last LED2 toggle time

int selectedLED = 0;              // LED chosen by user
int waitingForInterval = 0;       // input state


void TaskSerial();
void TaskLED1();
void TaskLED2();

// setup stuff
void setup()
{
    Serial.begin(9600);

    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT);

    digitalWrite(LEDPIN1, LOW);
    digitalWrite(LEDPIN2, LOW);

    Serial.println("What LED?");
}

// loops through each task
void loop()
{
    TaskSerial();   // task 1
    TaskLED1();     // task 2
    TaskLED2();     // task 3
}


// select led and time
void TaskSerial()
{
    if (Serial.available() > 0)
    {
        if (waitingForInterval == 0)
        {
            selectedLED = Serial.parseInt();

            if (selectedLED == 1 || selectedLED == 2)
            {
                Serial.println("What interval?");
                waitingForInterval = 1;
            }else
            {
                Serial.println("Invalid LED. Enter 1 or 2.");
                Serial.println("What LED?");
            }
        }
        else
        // if theres a new interval that needs to be set
        {
            unsigned long newInterval = Serial.parseInt();
            if (newInterval < 100)
            {
                newInterval = 100;
            }
            if (selectedLED == 1)
            {
                interval1 = newInterval;
                delaystart1 = millis();
            }
            if (selectedLED == 2)
            {
                interval2 = newInterval;
                delaystart2 = millis();
            }
            waitingForInterval = 0;
            Serial.println("What LED?");
        }
    }
}


void TaskLED1()
{
    // start the new clock for LED 1
    if (interval1 > 0 && millis() - delaystart1 >= interval1)
    {
        if (LED1 == 0)
        {
            LED1 = 1;
            digitalWrite(LEDPIN1, HIGH);
        }else
        {
            LED1 = 0;
            digitalWrite(LEDPIN1, LOW);
        }
        delaystart1 = millis();
    }
}


void TaskLED2()
{   // start the new clock for LED 2
    if (interval2 > 0 && millis() - delaystart2 >= interval2)
    {
        if (LED2 == 0)
        {
            LED2 = 1;
            digitalWrite(LEDPIN2, HIGH);
        }else
        {
            LED2 = 0;
            digitalWrite(LEDPIN2, LOW);
        }
        delaystart2 = millis();
    }
}
