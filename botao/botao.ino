#include <Servo.h>

#define Rservo 9
#define Lservo 10
#define Bservo 11 //pinos dos servos (devem tem habilitação pwm)
#define amplitude 170 //amplitude maxima em graus que o servo vai para a dobra (0-180)
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

String Read(){
  String msg;
  char data;
  while(Serial.available()){
    data = Serial.read();
    delay(50);
    msg+=data;
  }
  msg.toLowerCase();
  return msg;
}//lê uma string recebida pelo serial

void calibrar(){
  int pos;
  pos=right.read();
  while(!digitalRead(Rend)){
    pos--;
    right.write(pos--);
    delay(100);
  }
  zero[1]=pos;
  pos=left.read();
  while(!digitalRead(Lend)){
    pos--;
    left.write(pos--);
    delay(100);
  }
  zero[2]=pos;
  pos=bottom.read();
  while(!digitalRead(Bend)){
    pos--;
    bottom.write(pos--);
    delay(100);
  }
  zero[0]=pos;
} 

void movimento(Servo &serv, int fim, int tempo){
  const int timestep = 10;
  int partes = tempo/timestep;
  int inicio = serv.read();
  float angstep = (fim-inicio)/partes;
  for(int i=1;i<=partes;i++){
    serv.write(inicio+i*angstep);
    delay(timestep);
  }
}

void dobrar(int* seq){
  for (int i = 0; i < sizeof(seq); ++i){
    int dobra = seq[i];
    switch(dobra){
      case 0:
        movimento(bottom, zero[dobra]+amplitude, dtrise);
        movimento(bottom,zero[dobra],dtfall);
        break;
      case 1:
        movimento(right, zero[dobra]+amplitude, dtrise);
        movimento(right,zero[dobra],dtfall);
        break;
      case 2:
        movimento(left, zero[dobra]+amplitude, dtrise);
        movimento(left,zero[dobra],dtfall);
        break;
    }
  }
}//função que executa as dobras de acordo com a sequencia de numeros recebida como parâmetro

void setup() {
  Serial.begin(9600);
  
  pinMode(Rend,INPUT);
  pinMode(Lend,INPUT);
  pinMode(Bend,INPUT);
  
  right.attach(Rservo);
  left.attach(Lservo);
  bottom.attach(Bservo);

  calibrar();
  
  String init = "Programa de dobra de camisetas iniciado";//\nDigite uma das opções a seguir:\n'manga longa' 'manga curta' 'regata'";
  Serial.println(init);
  Serial.flush();
  delay(2000);
}

void loop() {
  if(Serial.available()){
    String msg = Read();
    String ret = "Dobra iniciada";
    Serial.println(ret);
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
  }
  delay(100);
}
