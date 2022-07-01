#define Te 320
#include "settings.h"
#include "esphome.h"

void SendCameBit(byte b)
{
  delayMicroseconds(Te);
  if (!b) digitalWrite(TRANSMITTER_PIN, HIGH);
  delayMicroseconds(Te);
  digitalWrite(TRANSMITTER_PIN, HIGH);
  delayMicroseconds(Te);
  digitalWrite(TRANSMITTER_PIN, LOW);
}


void RfTransmitt(char *codeString, unsigned int numberOfShipments)
{
  int codeLength = strlen(codeString);
  if (codeLength != 12 && codeLength != 24)
  {
    Serial.println("incorrect code.");
    return;
  }

  byte code[codeLength];
  for (int i = 0; i < codeLength; i++) {
    code[i] = codeString[i] == '0' ? 0 : 1;
  }

  byte codeUpdate[codeLength];

  int number = 0;

  switch (codeLength) {
    case 12:
      //обратный порядок
      for (int i = 5; i >= 0; i--) {
        codeUpdate[number] = code[i];
        //Serial.print(codeUpdate[number]);
        number++;
      }
      //Serial.print(" ");
      //прямой порядок
      for (int i = 6; i < 12; i++) {
        codeUpdate[number] = code[i];
        //Serial.print(codeUpdate[number]);
        number++;
      }
      break;
    case 24:
      //обратный порядок для всех символов
      for (int i = 1; i <= 3; i++) {
        for (int j = i * 8 - 1; j >= 8 * (i - 1); j--) {
          codeUpdate[number] = code[j];
          //Serial.print(codeUpdate[number]);
          number++;
        }
        //Serial.print("-");
      }
      break;
  }


  for (int i = 0; i < numberOfShipments; i++) // посылку посылаем как и брелок - NS раза подряд.
  {
    digitalWrite(TRANSMITTER_PIN, HIGH);
    delayMicroseconds(Te);
    digitalWrite(TRANSMITTER_PIN, LOW); // посылаем стартовый импульс
    for (int j = 0; j < codeLength; j++) {
      SendCameBit(codeUpdate[j]); // побитово перебираем и посылаем код
    }
    delay(16);
  }
}


class GatesSwitch : public Component, public Switch {
 public:
  void setup() override {
    // This will be called by App.setup()
    // pinMode(5, INPUT);
    pinMode(TRANSMITTER_PIN, OUTPUT);
  }
  void write_state(bool state) override {
    // This will be called every time the user requests a state change.

    RfTransmitt(GATES_CODE, 6);

    // Acknowledge new state by publishing it
    publish_state(state);
  }
};
