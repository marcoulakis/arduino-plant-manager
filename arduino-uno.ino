#include <LiquidCrystal_I2C.h>

#define umidadeAnalogica A1 //Atribui o pino A1 a variável umidade - leitura analógica do sensor
#define umidadeDigital 7 //Atribui o pino 7 a variável umidadeDigital - leitura digital do sensor

LiquidCrystal_I2C lcd(0x27,16,2);
byte degree_symbol[8] = 
              {
                0b00111,
                0b00101,
                0b00111,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000
              };
int gate = A0; 
volatile unsigned long duration=0;
unsigned char i[5];
unsigned int j[40];
unsigned char value=0;
unsigned answer=0;
int z=0;
int b=1;

int ldr = A2; //Atribui aa\\a2 a variável ldr
int valorldr = 0; //Declara a variável valorldr como inteiro

int valorumidade; //Declaração da variável que armazenará o valor da umidade lida - saída analogica
int valorumidadeDigital; //Declaração da variável que armazenara a saída digital do sensor de umidade do solo


void setup() 
{
  lcd.init();
  lcd.init();
  lcd.backlight(); 
  lcd.print("Ar: ");
  lcd.setCursor(0,1);
  lcd.print("Solo: ");
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(6,0);
  lcd.write(1);
  lcd.print("C - ");
  lcd.setCursor(15,0);
  lcd.print("%");
  lcd.setCursor(15,1);
  lcd.print("%");

    lcd.setCursor(7,1);
  lcd.print("% - ");

 Serial.begin(9600); //INICIALIZA A SERIAL
 Serial.println("Lendo a umidade do solo..."); //IMPRIME O TEXTO NO MONITOR SERIAL
 delay(2000); //INTERVALO DE 2 SEGUNDOS
}

void loop() 
{

 while(1)
 {
  delay(1000);

  valorldr=analogRead(ldr);//Lê o valor do sensor ldr e armazena na variável valorldr

  valorumidade = analogRead(umidadeAnalogica); //Realiza a leitura analógica do sensor e armazena em valorumidade
  valorumidade = map(valorumidade, 1023, 315, 0, 100); //Transforma os valores analógicos em uma escala de 0 a 100

  valorumidadeDigital = digitalRead(umidadeDigital); //Realiza a leitura digital do sensor e armazena em valorumidadeDigital

  pinMode(ldr, INPUT);

  valorldr=analogRead(ldr); //Lê o valor do sensor ldr e armazena na variável valorldr
  pinMode(gate,OUTPUT);
  digitalWrite(gate,LOW);
  delay(20);
  digitalWrite(gate,HIGH);
  pinMode(gate,INPUT_PULLUP);
  duration=pulseIn(gate, LOW);

  if(duration <= 84 && duration >= 72)
  {
      while(1)
      {
        duration=pulseIn(gate, HIGH);
        
        if(duration <= 26 && duration >= 20){
        value=0;}
        
        else if(duration <= 74 && duration >= 65){
        value=1;}
        
        else if(z==40){
        break;}
        
        i[z/8]|=value<<(7- (z%8));
        j[z]=value;
        z++;
      }
    }
  
  float luminosidade = (((valorldr / 1023.0) * 100.0) - 100) * -1;

  char luminosidadeStr[4];  // Array para armazenar a string da porcentagem

  dtostrf(luminosidade, 4, 1, luminosidadeStr);  // Converte a porcentagem em uma string com 3 dígitos significativos

  answer=i[0]+i[1]+i[2]+i[3];

  if(answer==i[4] && answer!=0)
  {
  lcd.setCursor(4,0);
  lcd.print(i[2]);
  lcd.setCursor(13,0);
  lcd.print(i[0]);


  lcd.setCursor(5,1);
  lcd.print(valorumidade);
  lcd.setCursor(11,1);
  lcd.print(luminosidadeStr);
  }


  Serial.print((String)"Temperatura: "+ i[2] + "°C\n" + "Umidade do ar: " + i[0] + "%\n" + "Umidade do solo: " + valorumidade + "%\n" + "Luminosidade: " + luminosidadeStr + "%" );

  z=0;
  i[0]=i[1]=i[2]=i[3]=i[4]=0;
  }
}
