// GPRS credentials
const char apn[]  = "iot.1nce.net";
const char user[] = "";
const char pass[] = "";
String dat = "{\"msisdn\":\"393600291838\",\"time\": \"22/03/23,18:00:09+04\",\"batteryLevel\":\"70%\",\"temperature\": \"30C\"}";


enum RegStatus {
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
};

#define MODEM_PWR_ON_PIN    13
#define MODEM_ON_PIN        32

#define BATTERY_PIN         35
#define BATTERY_K           ((2) * (3300.0) / 4096.0)
HardwareSerial serialGSM(2);

bool is_modem_registered = false;

unsigned long previousMillis = 0;


void waitResponse(int timeout = 1000L) {
  unsigned long currentMillis = millis();
  while (!serialGSM.available() || millis() - currentMillis >= timeout);
}

String sendAT(String text, int t = 2000L) {
  serialGSM.println(text);
  serialGSM.flush();
  waitResponse(t);
  return serialGSM.readString();
}


String senddata(String text, int t = 1000L) {
  serialGSM.print(text);
  //serialGSM.flush();
  waitResponse(t);
  return serialGSM.readString();
}

int getRegistrationStatus() {
  return sendAT("AT+CEREG?").toInt();

}

bool isNetworkConnected() {
  RegStatus s = (RegStatus)getRegistrationStatus();
  return (s == REG_OK_HOME || s == REG_OK_ROAMING);

}

bool waitForNetwork(int timeout_ms = 60000L, bool check_signal = false) {
  for (int start = millis(); millis() - start < timeout_ms;) {
    if (check_signal) {
      continue;
    }
  }
}



String getModemInfo() {
  return  sendAT("AT+CGMI");
}

String getIMEI() {
  return sendAT("AT+GSN");
}

String getIMSI() {
  return sendAT("AT+CIMI");
}

String getSimCCID() {
  return sendAT("AT+CCID");
}


void sendData() {
  
  Serial.println(sendAT("AT+CPIN?"));
  //Serial.println(sendAT("AT+QCFG=\"roamservice\",2,1"));
  Serial.println(sendAT("AT+QCFG=\"nwscanmode\",03,1"));
  Serial.println(sendAT("AT+QCFG=\"iotopmode\",1,1"));
  Serial.println(sendAT("AT+COPS=0,0"));
  delay(60000);
  String response = sendAT("AT+CREG?");
  Serial.println(response);
  
  while (response.indexOf("+CREG: 0,2")>0){
    delay(60000);
    Serial.println(sendAT("AT+CPSMS=0"));
    response = sendAT("AT+CREG?");
    Serial.println(response);
 }  
 // Serial.println(sendAT("AT+CEREG?"));  

  Serial.println(sendAT("AT+CGDCONT=1,\"IP\",\"iot.1nce.net\"" ));
  //modem.waitResponse();
  Serial.println(sendAT("AT+CGATT=1"  ));
  
  //Serial.println(sendAT("AT+CGACT=1,1"  ));
  
  Serial.println(sendAT("AT+QICSGP=1" ));

  Serial.println(sendAT("AT+QIACT=1"));

  Serial.println(sendAT("AT+QIACT?"  ));

  Serial.println(sendAT("AT+QIOPEN=1,2,\"UDP\",\"31.199.29.67\",6000,0,2"));

   delay(1000);
  Serial.println(senddata(dat));
  delay(1000);
  Serial.println(senddata("+++"));

  Serial.println(sendAT("AT+QICLOSE=2"));

}

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


  Serial.println(getModemInfo());
  Serial.println(getIMEI());
  Serial.println(getIMSI());
  Serial.println(getSimCCID());
  
  delay(60000);
  sendData();
}


void loop() {
  //put your main code here, to run repeatedly:




}
