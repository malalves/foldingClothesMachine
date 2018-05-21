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

int longa[] = {1,2,1,2,0};
int curta[] = {2,1,2,0};
int regata[] = {2,1,0};
//sequências de dobras 0 1 2 = b r l

class BlueHandler{
  private:
    long int baud;
    int brx, btx;
    SoftwareSerial &bserial;
    bool status;
  public:  
  BlueHandler(long int baud, int rx, int tx){
    this->baud = baud;
    this->brx = rx;
    this->btx = tx;
    this->status = false;
    bserial = SoftwareSerial(rx,tx);
  }//construtor instancia as variáveis e cria um software serial

  void Conect(){
    bserial.begin(baud);
    status = true;;
  }//conecta com o software serial e muda o status para true

  bool Status(){
    return status;
  }//retorna o status da conexão

  void Send(String msg){
    bserial.println(msg);
    bserial.flush();
  }//envia uma mensagem definida pelo usuário

  String Read(){
    String msg;
    char data;
    while(bserial.available()){
      data = bserial.read();
      delay(50);
      msg+=data;
    }
    msg.toLowerCase();
    return msg;
  }//lê uma string recebida pelo módulo bluetooth

  bool Available(){
    return bserial.available();
  }//retorna true se tem algo a ser lido
};//classe que lida com o envio e recebimento de mensagens via modulo bluetooth

Servo right;
Servo left;
Servo bottom;
BlueHandler blue(baudrate,srx,stx);//baud,rx,tx 

void dobrar(int* seq){
  for (int i = 0; i < sizeof(seq); ++i){
    int dobra = seq[i];
    switch(dobra){
      case 0:
        bottom.write(amplitude);
        delay(dt);
        bottom.write(0);
        delay(dt);
        break;
      case 1:
        right.write(amplitude);
        delay(dt);
        right.write(0);
        delay(dt);
        break;
      case 2:
        left.write(amplitude);
        delay(dt);
        left.write(0);
        delay(dt);
        break;
    }
  }
}//função que executa as dobras de acordo com a sequencia de numeros recebida como parâmetro

int count=0;
void setup() {
  Serial.begin(9600);
  blue.Conect();
  delay(dt);
  if(blue.Status()){
    Serial.println("bluetooth conectado");
  }
  else{
    Serial.println("FALHA NA CONEXÃO BLUETOOTH");
  }

  right.attach(Rservo);
  left.attach(Lservo);
  bottom.attach(Bservo);
  
  String init = "Programa de dobra de camisetas iniciado";//\nDigite uma das opções a seguir:\n'manga longa' 'manga curta' 'regata'";
  Serial.println(count);
  count++;
  Serial.println(init);
  Serial.flush();
  blue.Send(init);
  delay(2000);
}

void loop() {
  Serial.println("estou vivo");
  if(blue.Available()){
    String msg = blue.Read();
    String ret = "Dobra iniciada";
    Serial.println(ret);
    Serial.flush();
    blue.Send(ret);
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
    blue.Send(ret);
  }
  delay(100);
}
