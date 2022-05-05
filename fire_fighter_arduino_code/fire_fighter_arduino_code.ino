#include <Arduino_FreeRTOS.h>
#include <AFMotor.h>
#define threshold1 30
#define threshold2 35
#define delayMotor 500
#define delayBuzzer 500
#define delayRelay 3000
void TaskBuzzer(void *pvParameters);
void Task(void *pvParameters);

#define CF 52
#define ACF A8
#define CFg 51
#define CFd 50
#define relay 48
#define buzzer 49
int valeur; 
int etatduCF = 1;
//int threshold = value that we need to determine in ordre to stop the robot
int etatduCFg = 1;
int etatduCFd = 1;
int buzzerstate;

AF_DCMotor motorFR(2);
AF_DCMotor motorBR(1);
AF_DCMotor motorFL(3);
AF_DCMotor motorBL(4);

void setup() {
  Serial.begin(9600);
  pinMode(CF,INPUT);
  pinMode(ACF,INPUT);
  pinMode(CFg,INPUT);
  pinMode(CFd,INPUT);
  pinMode(relay,OUTPUT); 
  digitalWrite(relay, HIGH);
  pinMode(buzzer,OUTPUT);
  xTaskCreate(TaskBuzzer, "Task Buzzer", 128, NULL, 0, NULL);
  xTaskCreate(Task, "Task", 128, NULL, 0, NULL);
  vTaskStartScheduler();
}

void turnright() {
  motorFR.run(RELEASE);
  motorBR.run(RELEASE);
  motorFL.run(BACKWARD);
  motorFL.setSpeed(150);
  motorBL.run(BACKWARD);
  motorBL.setSpeed(150);
}

void turnleft(){
  motorFR.run(FORWARD);
  motorFR.setSpeed(150);
  motorBR.run(FORWARD);
  motorBR.setSpeed(150);
  motorFL.run(RELEASE); // needs a delay to release 
  motorBL.run(RELEASE);
}

void forward() {
  motorFR.run(FORWARD);
  motorFR.setSpeed(150);
  motorBR.run(FORWARD);
  motorBR.setSpeed(150);
  motorFL.run(BACKWARD);
  motorFL.setSpeed(150); 
  motorBL.run(BACKWARD);
  motorBL.setSpeed(150);
}
void Stop () { 
  motorFR.run(RELEASE);
  motorBR.run(RELEASE);
  motorFL.run(RELEASE); 
  motorBL.run(RELEASE);

}
void loop() {}

void TaskBuzzer(void *pvParameters){
 while (1){
  //while(digitalRead(CF)==0){
  while((digitalRead(CF)==0 || digitalRead(CFd)==0) || digitalRead(CFg)==0) {
      digitalWrite(buzzer, HIGH);
      vTaskDelay( delayBuzzer / portTICK_PERIOD_MS );
      digitalWrite(buzzer, LOW);
      vTaskDelay( delayBuzzer / portTICK_PERIOD_MS );
  }
  }
}

void Task(void *pvParameters) {
  while(1) {
  etatduCF = digitalRead(CF);
 //Buzzerstate=etatduCF;
   valeur = analogRead(ACF);
   Serial.print("Etat du CF :  ");
   Serial.println(etatduCF);
// vTaskDelay(1000/portTICK_PERIOD_MS);
    etatduCFg = digitalRead(CFg);
    Serial.print("\tEtat du CFg : ");
    Serial.print(etatduCFg);
    etatduCFd = digitalRead(CFd);
    Serial.print("\tEtat du CFd : ");
    Serial.print(etatduCFd);
   
    if(etatduCFg == 0) {
     turnleft();
     vTaskDelay( delayMotor / portTICK_PERIOD_MS );
  }
    if(etatduCFd == 0) {
     turnright();
     vTaskDelay( delayMotor / portTICK_PERIOD_MS ); 
  }
    if(etatduCF == 0) { // should t1is be a while etat du cf ==0 and valeur< threshold then forward ? etatduCF==0 && valeur<38
      forward();
// vTaskDelay( delayMotor / portTICK_PERIOD_MS ); 

      Serial.print("\tvalue :  "); 
      Serial.println(valeur); 
      //delay(1000);
//  vTaskDelay(1000/portTICK_PERIOD_MS); 
while(analogRead(ACF) <=threshold1 ){
    Stop ();
    digitalWrite(relay, LOW); // this shouldn't happen until the robot reaches the flame so i need an if condition here 
   // vTaskDelay( delayRelay / portTICK_PERIOD_MS);
    //vTaskDelay( delayMotor / portTICK_PERIOD_MS ); // i dont need a delay here because the robot should go forward until it reaches the threshold 
      
  
    }  digitalWrite(relay, HIGH);
  }
else {
Stop () ; }
} 
}
