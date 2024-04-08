long pin = 8;
long value = HIGH;
long thing_to_mess_with;
long read_pin_13 = 0;

uint16_t results;


void setup() {
  Serial.begin(9600);

}

void loop() {
  long read_pin_12 = 0;
  long read_pin_11 = 0;
  long read_pin_10 = 0;
  long read_pin_9 = 0;
  long read_pin_8 = 0;
  uint8_t data[1024];

  for (int i = 0; i < 1024; i++) {
    data[i] = PINB;
  };
  



  for (uint8_t x : data) {


    read_pin_8 += bitRead(x, 0);
    read_pin_9 += bitRead(x, 1);
    read_pin_10 += bitRead(x, 2);
    read_pin_11 += bitRead(x, 3);
    read_pin_12 += bitRead(x, 4);
  }

  Serial.print("Pin_8:");
  Serial.print((read_pin_8 > 800) * 1000);
  Serial.print(',');
  Serial.print("Pin_9:");
  Serial.print(read_pin_9);
  Serial.print(',');
  Serial.print("Pin_10:");
  Serial.print(read_pin_10);
  Serial.print(',');
  Serial.print("Pin_11:");
  Serial.print(read_pin_11);
  Serial.print(',');
  Serial.print("Pin_12:");
  Serial.println(read_pin_12);

}