volatile int rate[10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter[3] = {0, 0, 0};          // used to determine pulse timing
volatile unsigned long lastBeatTime[3] = {0, 0, 0};           // used to find IBI
volatile int P[3] = {512, 512, 512};                      // used to find peak in pulse wave, seeded
volatile int T[3] = {512, 512, 512};                     // used to find trough in pulse wave, seeded
volatile int thresh[3] = {525, 512, 512};                // used to find instant moment of heart beat, seeded
volatile int amp[3] = {100, 100, 100};                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat[3] = {true, true, true};        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat[3] = {false, false, false};      // used to seed rate array so we startup with reasonable BPM
int N[3];
word runningTotal[3] = {0, 0, 0};                 // WTF?!



// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE. 
// Timer 2 makes sure that we take a reading every 2 miliseconds
void Math(int s){                         // triggered when Timer2 counts to 124
  Signal[s] = analogRead(pulsePin[s]);              // read the Pulse Sensor 
  sampleCounter[s] += 2;                         // keep track of the time in mS with this variable
  N[s] = sampleCounter[s] - lastBeatTime[s];       // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
  if(Signal[s] < thresh[s] && N[s] > (IBI[s]/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal[s] < T[s]){                        // T is the trough
      T[s] = Signal[s];                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal[s] > thresh[s] && Signal[s] > P[s]){          // thresh condition helps avoid noise
    P[s] = Signal[s];                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (N[s] > 250){                                   // avoid high frequency noise
    if ( (Signal[s] > thresh[s]) && (Pulse == false) && (N[s] > (IBI[s]/5)*3) ){        
      Pulse[s] = true;                               // set the Pulse flag when we think there is a pulse
      IBI[s] = sampleCounter[s] - lastBeatTime[s];         // measure time between beats in mS
      lastBeatTime[s] = sampleCounter[s];               // keep track of time for next pulse

      if(secondBeat[s]){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat[s] = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI[s];                      
        }
      }

      if(firstBeat[s]){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat[s] = false;                   // clear firstBeat flag
        secondBeat[s] = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      runningTotal[s] = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i++];                  // and drop the oldest IBI value 
        runningTotal[s] += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI[s];                          // add the latest IBI to the rate array
      runningTotal[s] += rate[9];                // add the latest IBI to runningTotal
      runningTotal[s] /= 10;                     // average the last 10 IBI values 
      BPM[s] = 60000/runningTotal[s];               // how many beats can fit into a minute? that's BPM!
      QS[s] = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal[s] < thresh[s] && Pulse[s] == true){   // when the values are going down, the beat is over
    Pulse[s] = false;                         // reset the Pulse flag so we can do it again
    amp[s] = P[s] - T[s];                           // get amplitude of the pulse wave
    thresh[s] = amp[s]/2 + T[s];                    // set thresh at 50% of the amplitude
    P[s] = thresh[s];                            // reset these for next time
    T[s] = thresh[s];
  }

  if (N [s]> 2500){                           // if 2.5 seconds go by without a beat
    thresh[s] = 512;                          // set thresh default
    P[s] = 512;                               // set P default
    T[s] = 512;                               // set T default
    lastBeatTime[s] = sampleCounter[s];          // bring the lastBeatTime up to date        
    firstBeat[s] = true;                      // set these to avoid noise
    secondBeat[s] = false;                    // when we get the heartbeat back
  }

}





