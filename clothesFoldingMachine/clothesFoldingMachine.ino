#include <SoftwareSerial.h>

#define baudrate 9600
#define srx 2
#define stx 3

class BlueHandler{
  private:
    long int baud;
    int brx, btx;
    SoftwareSerial bserial;
    bool status;
  public:  
  BlueHandler(long int baud, int rx, int tx){
    this.baud = baud;
    this.brx = rx;
    this.btx = tx;
    this.status = FALSE;
    bserial = new SoftwareSerial (tx,rx);
  }//construtor instancia as variáveis e cria um software serial

  void Conect(){
    bserial.begin(baud);
    status = TRUE;;
  }//conecta com o software serial e muda o status para true

  bool Status(){
    return status;
  }//retorna o status da conexão

  void Send(String msg){
    serial.println(msg);
  }//envia uma mensagem definida pelo usuário

  String Read(){
    String msg;
    char data;
    while(serial.available()){
      data = serial.read();
      msg+=data;
    }
    msg.toLowerCase();
    return msg;
  }//lê uma string recebida pelo módulo bluetooth

  bool Available(){
    return serial.available();
  }//retorna true se tem algo a ser lido
};

class servo{
  public:
    servo();
};

class dobrador{
  public:
    dobrador();
};

#define ledPin 13
BlueHandler blue(baudrate,srx,stx);//baud,rx,tx 
int state = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  
  blue.Conect();
  if(blue.Status()){
    Serial.println("bluetooth conectado");
  }
  else{
    Serial.println("FALHA NA CONEXÃO BLUETOOTH")
  }
}

void loop() {
  if(blueSerial.available() > 0){ // Checks whether data is comming from the serial port
    state = blueSerial.read(); // Reads the data from the serial port
    Serial.print("leitura concluida:");
    Serial.println(state);
 }
 if (state == '0') {
  digitalWrite(ledPin, LOW); // Turn LED OFF
  blueSerial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
  state = 0;
  Serial.println("led: off");
 }
 else if (state == '1') {
  digitalWrite(ledPin, HIGH);
  blueSerial.println("LED: ON");;
  state = 0;
  Serial.println("led: on");
 } 
}
