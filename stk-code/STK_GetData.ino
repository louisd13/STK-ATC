#define LED_PIN 13

#define VIB_PIN1 5 // Thumb  
#define VIB_PIN2 9 // Droite
#define VIB_PIN3 6 // Middle finger
#define VIB_PIN4 10// Gauche
#define VIB_PIN5 3 // Little finger

unsigned long previousMillis = 0;
const long interval_1 = 500; 
const long interval_2 = 200; 



const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];  

char messageFromPC[numChars] = {0};
int integerFromPC1 = 0;
//int integerFromPC2 = 0;
float floatFromPC = 0.0;

boolean newData = false;

void setup() {
    Serial.begin(115200);
    //Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    //Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    //Serial.println();
    pinMode(VIB_PIN1 ,OUTPUT); // thumb
    pinMode(VIB_PIN2 ,OUTPUT); // index
    pinMode(VIB_PIN3 ,OUTPUT); // middle finger
    pinMode(VIB_PIN4 ,OUTPUT); // ring finger
    pinMode(VIB_PIN5 ,OUTPUT); // little finger
}

void loop() {
    unsigned long currentMillis = millis();

    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        showParsedData(currentMillis);
        newData = false;
    }
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC1 = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData(unsigned long currentMillis) {
    //Serial.print("Direction ");
    //Serial.println(messageFromPC[0]);
    // Serial.print("Intensity ");
    //Serial.println(integerFromPC1);
    // Serial.print("On road ");
    // Serial.println(integerFromPC2);
    //rallye(floatFromPC);

    //if (currentMillis - previousMillis >= interval) {
    //     // Save the last time you ran the function
      //previousMillis = currentMillis;

      if(messageFromPC[0] == 'l'){
          processValueLeft(integerFromPC1);
          analogWrite(VIB_PIN4, LOW);
        //   //Serial.println("Left close");
      }
      else if(messageFromPC[0] == 'r'){
        processValueRight(integerFromPC1);
        analogWrite(VIB_PIN2, LOW);
        //   //Serial.println("Right close");
      }
    //}
    // else{
    //   analogWrite(VIB_PIN4, LOW);
    //   analogWrite(VIB_PIN2, LOW);
    // }
}


void rallye(float f){
  if(f>2.0){ // go right
    //Serial.println("Go Right");
    analogWrite(VIB_PIN2, 25);
    analogWrite(VIB_PIN3, 0);
    analogWrite(VIB_PIN4, 0);
  }
  else if(f<-2.0){ // go left 
    Serial.println("Go Left");
    analogWrite(VIB_PIN2, 0);
    analogWrite(VIB_PIN3, 0);
    analogWrite(VIB_PIN4, 25);
  }
  else{ // go straight
    //Serial.println("Go Straight");
    analogWrite(VIB_PIN2, 0);
    analogWrite(VIB_PIN3, 0);
    analogWrite(VIB_PIN4, 0);
  }
}


// void processValueLeft(int value) {
//     if (value == 1) {
//         // Do something for value 1
//         analogWrite(VIB_PIN2, 20);
//         delay(100);
//         analogWrite(VIB_PIN2, LOW);
//     } else if (value == 2) {
//         // Do something for value 2
//         analogWrite(VIB_PIN2, 23);
//         delay(100);
//         analogWrite(VIB_PIN2, LOW);
//     } else if (value == 0) {
//       analogWrite(VIB_PIN2,0);
//     }
// }

void processValueLeft(int value) {
    static unsigned long previousMillis_1 = 0;
    static unsigned long previousMillis_2 = 0;
    const unsigned long duration_1 = 500; 
    const unsigned long duration_2 = 200; 


    unsigned long currentMillis = millis();

    if (value == 1) {
        if (currentMillis - previousMillis_1 >= duration_1) {
            previousMillis_1 = currentMillis;
            analogWrite(VIB_PIN4, 50);
            delay(100);
            analogWrite(VIB_PIN4, LOW);
        }
    } else if (value == 2) {
        if (currentMillis - previousMillis_2 >= duration_2) {
            previousMillis_2 = currentMillis;
            //Serial.println("gauche 2");
            analogWrite(VIB_PIN4, 50);
            delay(100);
            analogWrite(VIB_PIN4, LOW);
        }
    } else if (value == 0) {
        analogWrite(VIB_PIN4, 0);
    }
}



void processValueRight(int value) {
    static unsigned long previousMillis_1 = 0;
    static unsigned long previousMillis_2 = 0;
    const unsigned long duration_1 = 500;
    const unsigned long duration_2 = 200; 

    unsigned long currentMillis = millis();

    if (value == 1) {
        if (currentMillis - previousMillis_1 >= duration_1) {
            previousMillis_1 = currentMillis;
            analogWrite(VIB_PIN2, 50);
            delay(100);
            analogWrite(VIB_PIN2, LOW);
        }
    } else if (value == 2) {
        if (currentMillis - previousMillis_2 >= duration_2) {
            previousMillis_2 = currentMillis;
            analogWrite(VIB_PIN2, 50);
            delay(100);
            analogWrite(VIB_PIN2, LOW);
        }
    } else if (value == 0) {
        analogWrite(VIB_PIN2, 0);
    }
}





// void processValueRight(int value) {
//     if (value == 1) {
//         // Do something for value 1
//         analogWrite(VIB_PIN4, 20);
//         // pulse duration
//         delay(100);
//         analogWrite(VIB_PIN4, LOW);
//     } else if (value == 2) {
//         // Do something for value 2
//         analogWrite(VIB_PIN4, 23);
//         delay(100);
//         analogWrite(VIB_PIN4, LOW);

//     } else if (value == 0) {
//       analogWrite(VIB_PIN4,0);
//     }
// }
