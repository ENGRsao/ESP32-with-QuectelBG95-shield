#define MODEM_PWR_ON_PIN    13
#define MODEM_ON_PIN        32

#define BATTERY_PIN         35
#define BATTERY_K           ((2) * (3300.0) / 4096.0)
HardwareSerial serialGSM(2);

void setup() {
  // put your setup code here, to run once:
  pinMode(MODEM_PWR_ON_PIN, OUTPUT);
  digitalWrite(MODEM_PWR_ON_PIN, LOW);

  pinMode(MODEM_ON_PIN, OUTPUT);
  digitalWrite(MODEM_ON_PIN, LOW);

#if ((defined IOT_BOARD_TYPE) && (IOT_BOARD_TYPE == IOT_BOARD_TYPE_QBOARDB))
  pinMode(BATTERY_EN_PIN, OUTPUT);
  digitalWrite(BATTERY_EN_PIN, LOW);
#endif

  delay(3000);

  // Modem On Begin
  digitalWrite(MODEM_PWR_ON_PIN, HIGH);

  delay(1000);

  digitalWrite(MODEM_ON_PIN, HIGH);

  delay(1000);

  digitalWrite(MODEM_ON_PIN, LOW);
  // Modem On End

  delay(1000);
  serialGSM.begin(115200);
  Serial.begin(115200);
  Serial.println("Started");

}

void loop() {
  // put your main code here, to run repeatedly:
while (serialGSM.available())
{
  Serial.println(serialGSM.readString());
}

 while(Serial.available())
{
  serialGSM.println(Serial.readString());
}
}
