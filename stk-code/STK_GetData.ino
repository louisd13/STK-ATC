#define LED_PIN 13

#define VIB_PIN1 6 // majeur
#define VIB_PIN2 10 // petit doigt
#define VIB_PIN3 9  // annulaire
#define VIB_PIN4 5 // index 

#define VIB_PIN5 3

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];  

// First message i get :  left or right / vibration threshold
char messageFromPC[numChars] = {0};
int integerFromPC1 = 0;
float floatFromPC = 0.0;

// The second message i get : direction to aim
int integerFromPC2 = 0;

boolean newData = false;
boolean expectFirstMessage = true;

void setup() {
    Serial.begin(115200);
    pinMode(VIB_PIN1 ,OUTPUT);
    pinMode(VIB_PIN2 ,OUTPUT);
    pinMode(VIB_PIN3 ,OUTPUT);
    pinMode(VIB_PIN4 ,OUTPUT); 
    pinMode(VIB_PIN5 ,OUTPUT);
}

void loop() {

    if (expectFirstMessage) {
        recvWithStartEndMarkers();
        if (newData == true) {
            strcpy(tempChars, receivedChars);
            parseData();
            showParsedData();
            newData = false;
            expectFirstMessage = false; 
        }
    } else {
        recvWithStartEndMarkers2();
        if (newData == true) {
            strcpy(tempChars, receivedChars);
            parseData2();
            showParsedData2();
            newData = false;
            expectFirstMessage = true; // Switch back to expect first message next
        }
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

void recvWithStartEndMarkers2() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '{';
    char endMarker = '}';
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

void parseData2() {
    if (newData) {
        strcpy(tempChars, receivedChars); // this temporary copy is necessary to protect the original data
                                          // because strtok() used in parseData() replaces the commas with \0

        // Parse the character into an integer using atoi
        integerFromPC2 = atoi(tempChars); // Convert char to integer
        newData = false;
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

void showParsedData() {
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
}

void showParsedData2() {
    if (integerFromPC2 == 0) {
        analogWrite(VIB_PIN3, 0);
        analogWrite(VIB_PIN1, 0);
    } else if (integerFromPC2 == 1) {
        processValueSlightRight(); 
    } else if (integerFromPC2 == 2) {
        processValueSlightLeft(); 
    } else if (integerFromPC2 == 3) {
        processValueTooMuchRight(); 
    } else if (integerFromPC2 == 4) {
        processValueTooMuchLeft();
    }
}

void processValueSlightRight() {
    static unsigned long previousMillis = 0;
    const unsigned long duration = 200; 

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= duration) {
        previousMillis = currentMillis;
        analogWrite(VIB_PIN3, 40);
        delay(100);
        analogWrite(VIB_PIN3, LOW);
    }
}

void processValueSlightLeft() {
    static unsigned long previousMillis = 0;
    const unsigned long duration = 200; 

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= duration) {
        previousMillis = currentMillis;
        analogWrite(VIB_PIN1, 40);
        delay(100);
        analogWrite(VIB_PIN1, LOW);
    }
}

void processValueTooMuchRight() {
    static unsigned long previousMillis = 0;
    const unsigned long duration = 500; 

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= duration) {
        previousMillis = currentMillis;
        analogWrite(VIB_PIN3, 40);
        delay(100);
        analogWrite(VIB_PIN3, LOW);
    }
}

void processValueTooMuchLeft() {
    static unsigned long previousMillis = 0;
    const unsigned long duration = 500; 

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= duration) {
        previousMillis = currentMillis;
        analogWrite(VIB_PIN1, 40);
        delay(100);
        analogWrite(VIB_PIN1, LOW);
    }
}


void processValueLeft(int value) {
    static unsigned long previousMillis_1 = 0;
    static unsigned long previousMillis_2 = 0;
    const unsigned long duration_1 = 500; 
    const unsigned long duration_2 = 200; 


    unsigned long currentMillis = millis();

    if (value == 1) {
        if (currentMillis - previousMillis_1 >= duration_1) {
            previousMillis_1 = currentMillis;
            analogWrite(VIB_PIN4, 40);
            delay(100);
            analogWrite(VIB_PIN4, LOW);
        }
    } else if (value == 2) {
        if (currentMillis - previousMillis_2 >= duration_2) {
            previousMillis_2 = currentMillis;
            analogWrite(VIB_PIN4, 40);
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
            analogWrite(VIB_PIN2, 40);
            delay(100);
            analogWrite(VIB_PIN2, LOW);
        }
    } else if (value == 2) {
        if (currentMillis - previousMillis_2 >= duration_2) {
            previousMillis_2 = currentMillis;
            analogWrite(VIB_PIN2, 40);
            delay(100);
            analogWrite(VIB_PIN2, LOW);
        }
    } else if (value == 0) {
        analogWrite(VIB_PIN2, 0);
    }
}




