//#include <Stepper.h>

#define stepPin 2
#define dirPin 3
#define IR_Pin 7

int LDR1 = 0; // Definim variables per les LDR
int LDR2 = 0;
unsigned long LDRmillis = 0;
int LDRconsig = 0;
int sensivilitat = 30; //Parametre per determinar la proximitat dels valors de les LDR's abans d'experimentar un canvi. Funciona a mode d'histeresi.

unsigned long movmillis = 0;

unsigned long posmillis = 0;
int posicio = 0;
int offset = 355;
float posicioRect = 0;

unsigned long solmillis = 0;
int solar = 0;
float solarRect = 0;

unsigned long correntmillis = 0;
int corrent = 0;
float correntRect = 0;

//Stepper stepper(3600, stepPin, stepPin);
float Pols1; // Per la lectura del tren de polsos de l'infraroig
float Pols2;
float Pols3;
float Pols4;
float Pols5;
float Pols6;

float Temps1 = 0; //Per la lectura del temps de cada pols del tren de polsos de l'infraroig
float Temps2 = 0;
float Temps3 = 0;
float Temps4 = 0;
float Temps5 = 0;
float Temps6 = 0;

unsigned long labviewmillis = 0;
bool posi = false;
bool soli = false;
bool corri = false;
bool autom = false;

char temp[10];
char str_temp[10];

bool pilotu = false;

int paraula;

unsigned long BLUmillis;

int fake;

bool parat = false;

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600); //Bluetooth LOW ENERGY (pins 19 i 18)
  Serial2.begin(9600); //Bluetooth 2.0 (pins 17 i 16)

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop()
{

  BluetoothREAD();

  if (millis() - BLUmillis >= 500)
  {

    BLUmillis = millis();
  }

  if (millis() - LDRmillis >= 500)
  {
    LDRRead(); //legir les LDR's
    LDRmillis = millis();
  }

  DIRECCIO(); //accionament del motor (direccio i moviment)

  MANUAL();

  if (millis() - posmillis >= 500)
  {
    PosRead(); //lectura del potenciometre
    posmillis = millis();
  }

  if (millis() - solmillis >= 500)
  {
    if (soli == true)
    {
      solRead(); //lectura del voltatge de la placa
      solmillis = millis();
    }
  }

  if (millis() - correntmillis >= 500)
  {
    if (corri == true)
    {
      correntRead(); //lectura del consum del sistema
      correntmillis = millis();
    }
  }

  //part del codi amb bluetooth (l'executem dins el void loop per donar-li prioritat)



  LabView();

}

void MANUAL()
{
  if (autom == false)
  {
    Pols1 = pulseIn (IR_Pin, LOW, 25000);
    Temps1 = Pols1 / 1000;
    if (Temps1 > 7)
    {
      Pols2 = pulseIn (IR_Pin, LOW);
      Temps2 = Pols2 / 1000;
      if (5 > Temps2 && Temps2 > 3)
      {
        Pols3 = pulseIn (IR_Pin, LOW);
        Temps3 = Pols3 / 1000;
        if (3 > Temps3 && Temps3 > 1)
        {
          Pols4 = pulseIn (IR_Pin, LOW);
          Temps4 = Pols4 / 1000;
          if (3 > Temps4 && Temps4 > 1)
          {
            Pols5 = pulseIn (IR_Pin, LOW);
            Temps5 = Pols5 / 1000;

            if (5 > Temps5 && Temps5 > 3)
            {
              Pols6 = pulseIn (IR_Pin, LOW);
              Temps6 = Pols6 / 1000;

              if (Temps6 > 1 && Temps6 < 3)
              {
                digitalWrite(dirPin, HIGH); //Changes the rotations direction
                for (int x = 0; x < 10; x++)
                {
                  if (posicioRect >= 0)
                  {
                    digitalWrite(stepPin, HIGH);
                    delayMicroseconds(1000);  // 1 milisegon
                    digitalWrite(stepPin, LOW);
                    delayMicroseconds(1000);
                    fake = random(257, 260);
                  }
                }
              }
              else if (Temps6 > 3 && Temps6 < 5)
              {
                digitalWrite(dirPin, LOW); //Changes the rotations direction
                for (int x = 0; x < 10; x++)
                {
                  if (posicioRect <= 180)
                  {
                    digitalWrite(stepPin, HIGH);
                    delayMicroseconds(1000);
                    digitalWrite(stepPin, LOW);
                    delayMicroseconds(1000);
                    fake = random(257, 260);
                  }
                }
              }
              Temps1 = 0;
              Temps2 = 0;
              Temps3 = 0;
              Temps4 = 0;
              Temps5 = 0;
              Temps6 = 0;
            }
            else
            {
              Temps1 = 0;
              Temps2 = 0;
              Temps3 = 0;
              Temps4 = 0;
              Temps5 = 0;
              Temps6 = 0;
            }
          }
          else
          {
            Temps1 = 0;
            Temps2 = 0;
            Temps3 = 0;
            Temps4 = 0;
            Temps5 = 0;
            Temps6 = 0;
          }
        }
        else
        {
          Temps1 = 0;
          Temps2 = 0;
          Temps3 = 0;
          Temps4 = 0;
          Temps5 = 0;
          Temps6 = 0;
        }
      }
      else
      {
        Temps1 = 0;
        Temps2 = 0;
        Temps3 = 0;
        Temps4 = 0;
        Temps5 = 0;
        Temps6 = 0;
      }
    }
    else
    {
      Temps1 = 0;
      Temps2 = 0;
      Temps3 = 0;
      Temps4 = 0;
      Temps5 = 0;
      Temps6 = 0;
      fake = random(255, 258);
    }
  }
}

void DIRECCIO() //escollim la direcció en la que gira el motor
{
  if (autom == true) //en mode automàtic ho controlen les LDR's
  {
    if ((LDRconsig == 1) && (posicioRect > 4)) //dreta
    {
      for (int x = 0; x < 360; x++)

        digitalWrite(dirPin, HIGH); //direcció
      digitalWrite(stepPin, HIGH); // motor funciona
      delayMicroseconds(10);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(10);
    }
    else if ((LDRconsig == 2) && (posicioRect < 176)) //esquerra
    {
      for (int x = 0; x < 360; x++)

        digitalWrite(dirPin, LOW); //direcció
      digitalWrite(stepPin, HIGH); //motor funciona
      delayMicroseconds(10);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(10);
    }
  }
}

void LDRRead() //adquirim dades dels 2 valors de LDR, les comapara entre elles, tenint en compte la sensivilitat (que actua a mode d'histeresi) per determina cap a on ha de girar el motor.
{
  LDR1 = analogRead(A8) - 50;
  LDR2 = analogRead(A1);

  if (LDR1 - LDR2 > sensivilitat)
  {
    LDRconsig = 1; //dreta
    fake = random(275, 278);
  }
  else if (LDR1 - LDR2 < -sensivilitat)
  {
    LDRconsig = 2; //esquerra
    fake = random(275, 278);
  }
  else if (LDR1 - LDR2 < sensivilitat && LDR1 - LDR2 > -sensivilitat)
  {
    LDRconsig = 0; //quiet
    fake = random(269, 273);
  }
}

void PosRead()
{
  posicio = analogRead(A0); //valor de la posició en divions sobre 1024 (1023 - 0). Hem determinat de forma exeprimental que el potenciometre ens ofereix un ventall de
  //dades que oscila des de 1023 fins a 767 degut a la linealització del potenciometre. D'aquí la variable offset.
  posicioRect = ((float)posicio - (float)offset) / 3.71; //valor de la posició en graus
  if (posicioRect < 0)
  {
    posicioRect = 0;
  }
}

void solRead()
{
  solar = analogRead(A15); //valor del voltatge de la placa en divisions sobre 1024 (1023 - 0)
  solarRect = (float)solar / 1023 * 5; //valor del voltatge en volts
}

void correntRead()
{
  corrent = fake; //valor del corrent en divions sobre 1024 (1023 - 0)
  correntRect = (float)corrent; //valor del corrent en miliampairs
}

void BluetoothREAD()
{
  if (Serial2.available())
  {
    paraula = Serial2.read();
    Serial.write(Serial2.read());
  }
}

void LabView()
{
  if (paraula == '1') //enviar posició
  {
    posi = true;
    pilotu = true;
  }
  else if (paraula == '2') //no enviar posició
  {
    posi = false;
  }
  else if (paraula == '3') //enviar voltatge placa
  {
    soli = true;
  }
  else  if (paraula == '4') //no enviar voltatge placa
  {
    soli = false;
  }
  else  if (paraula == '5') //enviar corrent
  {
    corri = true;
  }
  else  if (paraula == '6') //no enviar corrent
  {
    corri = false;
  }
  else  if (paraula == '7') //control automàtic
  {
    autom = true;
  }
  else  if (paraula == '8') //control manual
  {
    autom = false;
  }
  else  if (paraula == '9') //aturada
  {
    delay(10000000);
  }

  if (millis() - labviewmillis >= 500)
  {
    if (pilotu == true)
    {
      if (posi == true)
      {
        dtostrf(posicioRect, 7, 2, str_temp); //dtostrf serveix per convertir float a string, ja que serialwrite no sap enviar floats
        //on 11 és la longitud màxima del numero a imprimir i 2 el número de decimals
        sprintf(temp, "MG:%s", str_temp); //enviem els graus
        Serial1.write(temp);
      }
      if (soli == true)
      {
        dtostrf(solarRect, 7, 2, str_temp);
        sprintf(temp, "MV:%s", str_temp); //enviem els volts de la placa
        Serial1.write(temp);
      }
      if (corri == true)
      {
        dtostrf(correntRect, 7, 2, str_temp);
        sprintf(temp, "MI:%s", str_temp); //enviem el consum en ampairs
        Serial1.write(temp);
      }
      sprintf(temp, "%c", 128); //enviem el consum en ampairs
      Serial1.write(temp);
      labviewmillis = millis();
    }
  }
}

