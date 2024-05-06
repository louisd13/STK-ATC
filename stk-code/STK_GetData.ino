#define LED_PIN 13
#define VIB_PIN1 9 
#define VIB_PIN2 10

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];  

char messageFromPC[numChars] = {0};
int integerFromPC1 = 0;
int integerFromPC2 = 0;
//float floatFromPC = 0.0;

boolean newData = false;

void setup() {
    Serial.begin(115200);
    //Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    //Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    //Serial.println();
    pinMode(VIB_PIN2, OUTPUT);
    pinMode(VIB_PIN1,OUTPUT);
}

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
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
    integerFromPC2 = atoi(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    //Serial.print("Direction ");
    //Serial.println(messageFromPC);
    // Serial.print("Intensity ");
    //Serial.println(integerFromPC1);
    // Serial.print("On road ");
    // Serial.println(integerFromPC2);
    if (messageFromPC[0] == 'r') {
      analogWrite(VIB_PIN1, 0);
      processValueRight(integerFromPC1);
      
    }
    if(messageFromPC[0] == 'l'){
      analogWrite(VIB_PIN2, 0);
      processValueLeft(integerFromPC1);
    }
}

void processValueLeft(int value) {
    if (value == 1) {
        // Do something for value 1
        analogWrite(VIB_PIN1, 15);
    } else if (value == 2) {
        // Do something for value 2
        analogWrite(VIB_PIN1, 20);
    } else if (value == 0) {
      analogWrite(VIB_PIN1,0);
    }
}


void processValueRight(int value) {
    if (value == 1) {
        // Do something for value 1
        analogWrite(VIB_PIN2, 15);
    } else if (value == 2) {
        // Do something for value 2
        analogWrite(VIB_PIN2, 20);
    } else if (value == 0) {
      analogWrite(VIB_PIN2,0);
    }
}
