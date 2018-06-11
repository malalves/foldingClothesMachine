#include <SoftwareSerial.h>
#include <Servo.h>


#define baudrate 9600 //baudrate da comunicação bluetooth
#define srx 6
#define stx 7 //pinos da comunicação serial do bluetooth
#define Rservo 9
#define Lservo 10
#define Bservo 11 //pinos dos servos (devem tem habilitação pwm)
#define amplitude 150 //amplitude maxima em graus que o servo vai para a dobra (0-180)
#define dtrise 1000 //tempo em milisegundo que espera para executar subida
#define dtfall 2000 //tempo em milisegundo que espera para executar descida
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
    if(data == '?'){
      break;  
    }
    delay(50);
    msg+=data;
  }
  msg.toLowerCase();
  return msg;
}//lê uma string recebida pelo módulo bluetooth

void calibrar(){
  int pos;
  bottom.attach(Bservo);
  delay(500);
  pos=bottom.read();
  while(!digitalRead(Bend)){
    pos++;
    bottom.write(pos);
    delay(100);
  }
  zero[0]=pos;
  right.attach(Rservo);
  delay(500);
  pos=right.read();
  while(!digitalRead(Rend)){
    pos--;
    right.write(pos);
    delay(100);
  }
  zero[1]=pos;
  left.attach(Lservo);
  delay(500);
  pos=left.read();
  while(!digitalRead(Lend)){
    pos++;
    left.write(pos);
    delay(100);
  }
  zero[2]=pos;
}  

void movimento(Servo &serv, int fim, int tempo){
  const int timestep = 10;
  int partes = tempo/timestep;
  int inicio = serv.read();
  float angstep = ((float)fim-(float)inicio)/(float)partes;
  for(int i=1;i<=partes;i++){
    int ang = inicio+i*angstep;
    serv.write(ang);
    delay(timestep);
  }
}//função de movimento controlado

void dobrar(int* seq){
  int i = -1;
 do{
    i++;  
    int dobra = seq[i];
    switch(dobra){
      case 0:
        movimento(bottom, zero[dobra]-amplitude, dtrise);
        delay(100);
        movimento(bottom,zero[dobra],dtfall);
        break;
      case 1:
        movimento(right, zero[dobra]+amplitude, dtrise);
        delay(100);
        movimento(right,zero[dobra],dtfall);
        break;
      case 2:
        movimento(left, zero[dobra]-amplitude, dtrise);
        delay(100);
        movimento(left,zero[dobra],dtfall);
        break;
    }
   }while(seq[i]!=0);
}//função que executa as dobras de acordo com a sequencia de numeros recebida como parâmetro

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
  
  String init = "Programa de dobra de camisetas iniciado";//\nDigite uma das opções a seguir:\n'manga longa' 'manga curta' 'regata'";
  Serial.println(init);
  Serial.flush();
  blue.println(init);
  delay(2000);
}

void loop() {
  if(blue.available()){
    String msg = Read();
    Serial.println(msg);
    String a = "Dobra iniciada";
    String ret = "";
    Serial.println(a);
    blue.println(a);
    if(msg == "manga longa"){
      dobrar(longa);
      ret = "1";
    }
    else if(msg == "manga curta"){
      dobrar(curta);
      ret = "2";
    }
    else if(msg == "regata"){
      dobrar(regata);
      ret = "3";
    }
    else{
      ret = "DOBRA INVÁLIDA";//\nDigite uma das opções a seguir:\n'manga longa' 'manga curta' 'regata'";
    }
    Serial.println(ret);
    Serial.flush();
    blue.println(ret);
  }
  delay(100);
}
