#include <SoftwareSerial.h>
#include <Servo.h>


#define baudrate 9600 //baudrate da comunicação bluetooth
#define srx 6
#define stx 7 //pinos da comunicação serial do bluetooth
#define Rservo 9
#define Lservo 10
#define Bservo 11 //pinos dos servos (devem tem habilitação pwm)
#define amplitude 170 //amplitude maxima em graus que o servo vai para a dobra (0-180)
#define dt 2000 //tempo em milisegundo que espera para executar cada movimento do servo
#define Rend 2
#define Lend 3
#define Bend 4//pinos dos endstops (em pull down - lembrar de fazer essa conexão)

int longa[] = {1,2,1,2,0};
int curta[] = {2,1,2,0};
int regata[] = {2,1,0};
//sequências de dobras 0 1 2 = b r l
int zero[] = {0,0,0};//zeros das posições dos servos
Servo right;
Servo left;
Servo bottom;
SoftwareSerial blue(srx,stx);//rx,tx 

String Read(){
  String msg;
  char data;
  while(blue.available()){
    data = blue.read();
    delay(50);
    msg+=data;
  }
  msg.toLowerCase();
  return msg;
}//lê uma string recebida pelo módulo bluetooth

void calibrar(){
  int pos;
  pos=right.read();
  While(!digitalRead(Rend)){
    pos--;
    rigt.write(pos--);
    delay(10);
  }
  zero[1]=pos;
  pos=left.read();
  While(!digitalRead(Lend)){
    pos--;
    left.write(pos--);
  }
  zero[2]=pos;
  pos=bottom.read();
  While(!digitalRead(Bend)){
    pos--;
    bottom.write(pos--);
  }
  zero[0]=pos;
} 

void dobrar(int* seq){
  for (int i = 0; i < sizeof(seq); ++i){
    int dobra = seq[i];
    switch(dobra){
      case 0:
        bottom.write(zero[dobra]+amplitude);
        delay(dt);
        bottom.write(zero[dobra]);
        delay(dt);
        break;
      case 1:
        right.write(zero[dobra]+amplitude);
        delay(dt);
        right.write(zero[dobra]);
        delay(dt);
        break;
      case 2:
        left.write(zero[dobra]+amplitude);
        delay(dt);
        left.write(zero[dobra]);
        delay(dt);
        break;
    }
  }
}//função que executa as dobras de acordo com a sequencia de numeros recebida como parâmetro

int count=0;
void setup() {
  Serial.begin(9600);
  blue.begin(baudrate);
  if(blue){
    Serial.println("bluetooth conectado");
  }
  else{
    Serial.println("FALHA NA CONEXÃO BLUETOOTH");
  }
  pinMode(Rend,INPUT);
  pinMode(Lend,INPUT);
  pinMode(Bend,INPUT);
  
  calibrar();

  right.attach(Rservo);
  left.attach(Lservo);
  bottom.attach(Bservo);
  
  String init = "Programa de dobra de camisetas iniciado";//\nDigite uma das opções a seguir:\n'manga longa' 'manga curta' 'regata'";
  Serial.println(count);
  count++;
  Serial.println(init);
  Serial.flush();
  blue.println(init);
  delay(2000);
}

void loop() {
  Serial.println("estou vivo");
  if(blue.available()){
    String msg = Read();
    String ret = "Dobra iniciada";
    Serial.println(ret);
    blue.println(ret);
    if(msg == "manga longa"){
      dobrar(longa);
      ret = "Dobra de manga longa executada";
    }
    else if(msg == "manga curta"){
      dobrar(curta);
      ret = "Dobra de manga curta executada";
    }
    else if(msg == "regata"){
      dobrar(regata);
      ret = "Dobra de regata executada";
    }
    else{
      ret = "DOBRA INVÁLIDA\nDigite uma das opções a seguir:\n'manga longa' 'manga curta' 'regata'";
    }
    Serial.println(ret);
    Serial.flush();
    blue.println(ret);
  }
  delay(100);
}
