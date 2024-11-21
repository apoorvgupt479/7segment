#include <dht.h>

#define DHT11_PIN 10


dht DHT;

// Define pin connections for each segment
const int segmentPins[] = {2, 3, 4, 5, 6, 7, 8}; // A, B, C, D, E, F, G pins

// Define segment patterns for numbers 0–9
const byte digitPatterns[] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
};

//coeffecients for heat index formula
const double coeff[] = {
  -8.78469475556,
  1.61139411,
  2.33854883889,
  -0.14611605,
  -0.012308094,
  -0.0164248277778,
  0.002211732,
  0.00072546,
  -0.000003582
};

// Function to display a digit
void displayDigit(int digit) {
  byte pattern = digitPatterns[digit];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], bitRead(pattern, i)); // Light up corresponding segments
  }
}

// Function to clear the display
void clearDisplay() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], LOW);
  }
}



void setup() {

  Serial.begin(9600);

  // Set all segment pins as output
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  
  // Turn off all segments initially
  clearDisplay();
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);

  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  //using heat index formula
  double heatIndex =   coeff[0] +
                       coeff[1] * temperature +
                       coeff[2] * humidity +
                       coeff[3] * temperature * humidity +
                       coeff[4] * pow(temperature, 2) +
                       coeff[5] * pow(humidity, 2) +
                       coeff[6] * pow(temperature, 2) * humidity +
                       coeff[7] * temperature * pow(humidity, 2) +
                       coeff[8] * pow(temperature, 2) * pow(humidity, 2);

  Serial.print("Heat: ");
  Serial.println(heatIndex);

  if(heatIndex < 20.00) displayDigit(0);
  else if (heatIndex < 23.75) displayDigit(1);
  else if (heatIndex < 27.50) displayDigit(2);
  else if (heatIndex < 31.25) displayDigit(3);
  else if (heatIndex < 35.00) displayDigit(4);
  else if(heatIndex >= 35.00) displayDigit(5);

  delay(1000);       // Delay needed for DHT11
  
}


