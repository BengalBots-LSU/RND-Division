/*
  Marc Hebert (Mr. Goop)

  Purpose:
  This code converts a PPM signal to raw time values from an 8 channel PPM reciver

  Backround:
  PPM (Pulse Position Modulation) is not to be confused with PWM (Pulse Width Modulation)
  PPM works by sending a pulse to signal to denote the start time of a channel. Time after this pulse is counted untill a new pulse arrives.
  At this new pulse, the first channel now has a start and stop time and the next channel has a start time. There is this small overlap
  In between channel 8 and 1 there is a larger gap in time. In this program, this is channel 0.
  You might already be able to tell that this could cause syncing issues, and this is addressed in the code

  Interupts are signals that start a subroutine that has priority over the main functions in code. ideally these interupts are fast and relatively spaced out in time.
  In our case we get an interupt every time there is a change in signal on pin 8, and we use a digital read to only use when the signal rises.

  Explantion:
  The code works by using interupts on pin 8 to calculate time between pulses. Using the timestamps, the channel values are calculated. Pulses are detected but an interupt
  and a subroutine tracks the time and syncing of the pulses. The ISR function is the interupt subroutine. The case statement is used to determine current channel and 
  each case has a syncing check. You may see that there is no call to the ISR function.
  It is accessed when the intereput happens becuase of the memory address placed in the parameters feild.

  WARNING:
  Do not run continuosly for over 70 minutes. After 70 minutes the arduino timer resets to 0 and could potentially break. I do plan to add something to fix this,
  however I do not plan on waiting over an hour to test it. Might be able to test by manipulating timer register.

  Channel assignment for Turningy controller:
  ch[0] = in between signal time
  ch[1] = x axis, right stick, right is 2000us, left is 1000us
  ch[2] = y axis, right stick, up is 2000us, down is 1000us
  ch[3] = y axis, left stick, up is 1900us, down is 1130us
  ch[4] = x axis, left stick, right is 2000us, left is 1000us
  ch[5] = swc three way switch, up is 2000us, middle is 1500us, down is 1000us
  ch[6] = VRB knob, full left is 1000us, full right is 2000us, linear gain

  ch[7] = unused
  ch[8] = unused
  
*/

int chanNum = 8;
unsigned long channel[9], time[10]; //channel[0] is the time in bettween PPM packets
                                    //to adapt to diffrent amount of channels, make channel[chanNum + 1] and time[chanNum + 2]
                                    //channel holds duration of each channels signal and time holds the most recent time stamp
int pulse = 0;                      //pulse is the counter for the current channel

void writeOut(){                    //basic for loop to display all channel values
  int i;                            //reminder that channel[0] is not a channel on the reciever, but the space between channel[1] and channel[8]
  Serial.println(" ");
  for(i = 0; i<=chanNum; i++){
    Serial.print(channel[i]);
    Serial.print(" - ");
  }
}

void setup() {
SREG |= B10000000;          //SREG bit 7 set to 1 (enables intterupts);
PCICR |= (1 << PCIE0);      //enable interupt on ports 0-7, PCINT stil needs to be enabled
PCMSK0 |= (1 << PCINT0);    //pin 8 on uno and nano, PCINT0 is enabled

Serial.begin(31250);
}

void loop() { //an other nessasry code may be placed in the loop and functions maybe accesed from it.
              //interupt still has priority
delay(500);   //to make output more readable
writeOut(); //uncomment to veiw the calculated times
}

ISR(PCINT0_vect){
  if (PINB & B00000001)   //fast version of digitalRead(8), only execute on rising edge
  {
    time[pulse] = micros(); //timestamp the rising edge interupt
    switch (pulse) {        //find which channel we are reading for.

      case 1:               //channel 1
      channel[1] = time[1] - time[0];   //find time value for channel
      pulse++;                          //increment pulse to prepare for next channel calculation
      if (channel[1] > 2500)            //if channel time is longer than 2500us, this is channel 0 and a desync has occured
      {
        time[0] = time[1];              //by resetting the pulse to 1 and assigning the current time to time[0], the program is re-synced
        pulse = 1;
      }
      break;

      case 2:               //channel 2
      channel[2] = time[2] - time[1];
      pulse++;
      if (channel[2] > 2500)
      {
        time[0] = time[2];
        pulse = 1;
      }
      break;

      case 3:               //channel 3
      channel[3] = time[3] - time[2];
      pulse++;
      if (channel[3] > 2500)
      {
        time[0] = time[3];
        pulse = 1;
      }
      break;

      case 4:               //channel 4
      channel[4] = time[4] - time[3];
      pulse++;
      if (channel[4] > 2500)
      {
        time[0] = time[4];
        pulse = 1;
      }
      break;

      case 5:               //channel 5
      channel[5] = time[5] - time[4];
      pulse++;
      if(channel[5] > 2500)
      {
        time[0] = time[5];
        pulse = 1;
      }
      break;

      case 6:               //channel 6
      channel[6] = time[6] - time[5];
      pulse++;
      if(channel[6] > 2500)
      {
        time[0] = time[6];
        pulse = 1;
      }
      break;

      case 7:               //channel 7
      channel[7] = time[7] - time[6];
      pulse++;
      if(channel[7] > 2500)
      {
        time[0] = time[7];
        pulse = 1;
      }
      break;

      case 8:               //channel 8
      channel[8] = time[8] - time[7];
      pulse++;
      if(channel[8] > 2500)
      {
        time[0] = time[8];
        pulse = 1;
      }
      break;

      case 9:               //channel 0
      channel[0] = time[9] - time[8];
      pulse = 1;
      //The code bellow should be unnessicary, however it has not been tested without using it
      pulse++;
      if(channel[0] > 2500)
      {
        time[0] = time[9];
        pulse = 1;
      }
      break;

      default:
      pulse++;
      break;
    }
  }
}