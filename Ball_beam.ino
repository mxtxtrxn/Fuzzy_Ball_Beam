/*                                       
                          CAMILO COVARRUBIAS HERNÁNDEZ
                          Proyecto: Barra esfera
                            Sistema de control para un mecanismo barra-esfera, tomando como base unos parámetros asignados de longitud de la barra, 
                            radio de la esfera, asi como también los angulos máximo y minimo. 
                          github: @mxtxtrxn

*/


#include <FuzzyIO.h>
#include <FuzzyRule.h>
#include <FuzzyOutput.h>
#include <FuzzyComposition.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyInput.h>
#include <Servo.h>
# define trigPin 13
# define echoPin 12
// NM - Negative Medium
// NS - Negative Small
// ZO - Zero
// PS - Positive Small
// PM - Positive Medium

// Longitud de la barra (cm)
int const L = 30;
// Radio de la pelota (cm)
int const R = 2;
// Angulo theta minimo
int const MIN = 45;
// Angulo theta máximo
int const MAX = 180;
// Velocidad de reacción en m/s
int const rspeed = 50;
// Posicion inicila del motor
int const initMposition = 0;
// ¿El motor se mueve ?
bool moveMotor = true;

// Set point
float set_point = 10;

Servo servo;

Fuzzy* fuzzyObj = new Fuzzy();

FuzzySet* position_nm = new FuzzySet(0, 0, 3, 6);
FuzzySet* position_ns = new FuzzySet(3, 6, 9, 12);
FuzzySet* position_zo = new FuzzySet(9, 12, 15, 18);
FuzzySet* position_ps = new FuzzySet(15, 18, 21, 24);
FuzzySet* position_pm = new FuzzySet(21, 24, 26, L);

FuzzySet* setpoint_nm = new FuzzySet(0, 0, 3, 6);
FuzzySet* setpoint_ns = new FuzzySet(3, 6, 9, 12);
FuzzySet* setpoint_zo = new FuzzySet(9, 12, 15, 18);
FuzzySet* setpoint_ps = new FuzzySet(15, 18, 21, 24);
FuzzySet* setpoint_pm = new FuzzySet(21, 24, 27, L);

FuzzySet* theta_nm = new FuzzySet(MIN, 50, 60, 80);
FuzzySet* theta_ns = new FuzzySet(60, 80, 90, 100);
FuzzySet* theta_zo = new FuzzySet(90, 100, 110, 120);
FuzzySet* theta_ps = new FuzzySet(110, 120, 120, 130);
FuzzySet* theta_pm = new FuzzySet(130, 140, 160, MAX);

void initSensor(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void initServo(){
servo.attach(9);
servo.write(initMposition);
}



void setup() {
  Serial.begin(9600);
  initSensor();
  initServo();
  initFuzzyLogic();
}

float calculateDistance(){
  float distance, duration;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  distance = (calculateDuration()/2) * 0.0344;
  distance = distance - R;
  if(distance > L){distance = L;}
  if(distance <= 0){distance = 0;}

  Serial.print("Adjusted Distance = ");
  Serial.println(distance);
  
  return distance;
}


float calculateDuration(){
  float duracion;
  duracion = pulseIn(echoPin, HIGH);
  
  return duracion;
}

float calculateVelocity(){
  float velocidad,x1,x2,t1,t2;
  x1 = calculateDistance();
  t1 = calculateDuration();

  delay(100);
  
  x2 = calculateDistance();
  t2 = calculateDuration();

  velocidad = (x2 - x1)/(t2 - t1);

  return velocidad;
}

void loop() {
  float ball_position = calculateDistance();
  
  fuzzyObj->setInput(1,ball_position);
  fuzzyObj->setInput(2,set_point);
  
  fuzzyObj->fuzzify();
  
  float thetaOutput = fuzzyObj -> defuzzify(1);
  
  logger(thetaOutput);
  
  if(moveMotor){
    moveServo(thetaOutput);
  }

  delay(rspeed);
}


void initFuzzyLogic(){
FuzzyInput* ballPosition = new FuzzyInput(1);
  
ballPosition->addFuzzySet(position_nm);
ballPosition->addFuzzySet(position_ns);
ballPosition->addFuzzySet(position_zo);
ballPosition->addFuzzySet(position_ps);
ballPosition->addFuzzySet(position_pm);
  
fuzzyObj->addFuzzyInput(ballPosition);

FuzzyInput*setPoint = new FuzzyInput(2);
  
setPoint->addFuzzySet(setpoint_nm);
setPoint->addFuzzySet(setpoint_ns);
setPoint->addFuzzySet(setpoint_zo);
setPoint->addFuzzySet(setpoint_ps);
setPoint->addFuzzySet(setpoint_pm);
  
fuzzyObj->addFuzzyInput(setPoint); 

FuzzyOutput* theta = new FuzzyOutput(1);
  
theta->addFuzzySet(theta_nm);
theta->addFuzzySet(theta_ns);
theta->addFuzzySet(theta_zo);
theta->addFuzzySet(theta_ps);
theta->addFuzzySet(theta_pm);
  
fuzzyObj->addFuzzyOutput(theta);

//                              REGLAS  I
FuzzyRuleConsequent* then_thetaPS = new FuzzyRuleConsequent();
then_thetaPS->addOutput(theta_ps);

FuzzyRuleConsequent* then_thetaPM = new FuzzyRuleConsequent();
then_thetaPM->addOutput(theta_pm);

FuzzyRuleConsequent* then_thetaNM = new FuzzyRuleConsequent();
then_thetaNM->addOutput(theta_nm);

FuzzyRuleConsequent* then_thetaNS = new FuzzyRuleConsequent();
then_thetaNS->addOutput(theta_ns);

FuzzyRuleConsequent* then_thetaZO = new FuzzyRuleConsequent();
then_thetaZO->addOutput(theta_zo);


//                              REGLAS II

//                              Posición NM
FuzzyRuleAntecedent* positionNM_setPointNM = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNM_setPointNS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNM_setPointZO = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNM_setPointPS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNM_setPointPM = new FuzzyRuleAntecedent();
  
positionNM_setPointNM->joinWithAND(position_nm, setpoint_nm);
positionNM_setPointNS->joinWithAND(position_nm, setpoint_ns);
positionNM_setPointZO->joinWithAND(position_nm, setpoint_zo);
positionNM_setPointPS->joinWithAND(position_nm, setpoint_ps);
positionNM_setPointPM->joinWithAND(position_nm, setpoint_pm);
  
FuzzyRule* Rule1 = new FuzzyRule(1, positionNM_setPointNM, then_thetaZO);
FuzzyRule* Rule2 = new FuzzyRule(2, positionNM_setPointNS, then_thetaNS);
FuzzyRule* Rule3 = new FuzzyRule(3, positionNM_setPointZO, then_thetaNS);
FuzzyRule* Rule4 = new FuzzyRule(4, positionNM_setPointPS, then_thetaNS);
FuzzyRule* Rule5 = new FuzzyRule(5, positionNM_setPointPM, then_thetaNM);

fuzzyObj->addFuzzyRule(Rule1);
fuzzyObj->addFuzzyRule(Rule2);
fuzzyObj->addFuzzyRule(Rule3);
fuzzyObj->addFuzzyRule(Rule4);
fuzzyObj->addFuzzyRule(Rule5);

//                           Posición NS
FuzzyRuleAntecedent* positionNS_setPointNS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNS_setPointNM = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNS_setPointZO = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNS_setPointPS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionNS_setPointPM = new FuzzyRuleAntecedent();
  
positionNS_setPointNS->joinWithAND(position_ns, setpoint_ns);
positionNS_setPointNM->joinWithAND(position_ns, setpoint_nm);
positionNS_setPointZO->joinWithAND(position_ns, setpoint_zo);
positionNS_setPointPS->joinWithAND(position_ns, setpoint_ps);
positionNS_setPointPM->joinWithAND(position_ns, setpoint_pm);
  
FuzzyRule* Rule6 = new FuzzyRule(6, positionNS_setPointNS, then_thetaZO);
FuzzyRule* Rule7 = new FuzzyRule(7, positionNS_setPointNM, then_thetaZO);
FuzzyRule* Rule8 = new FuzzyRule(8, positionNS_setPointZO, then_thetaNS);
FuzzyRule* Rule9 = new FuzzyRule(9, positionNS_setPointPS, then_thetaNS);
FuzzyRule* Rule10 = new FuzzyRule(10, positionNS_setPointPM, then_thetaNM);

fuzzyObj->addFuzzyRule(Rule6);
fuzzyObj->addFuzzyRule(Rule7);
fuzzyObj->addFuzzyRule(Rule8);
fuzzyObj->addFuzzyRule(Rule9);
fuzzyObj->addFuzzyRule(Rule10);

//                           Posición ZO
FuzzyRuleAntecedent* positionZO_setPointZO = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionZO_setPointNM = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionZO_setPointNS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionZO_setPointPS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionZO_setPointPM = new FuzzyRuleAntecedent();
  
positionZO_setPointZO->joinWithAND(position_zo, setpoint_zo);
positionZO_setPointNM->joinWithAND(position_zo, setpoint_nm);
positionZO_setPointNS->joinWithAND(position_zo, setpoint_ns);
positionZO_setPointPS->joinWithAND(position_zo, setpoint_ps);
positionZO_setPointPM->joinWithAND(position_zo, setpoint_pm);
  
FuzzyRule* Rule11 = new FuzzyRule(11, positionZO_setPointZO, then_thetaZO);
FuzzyRule* Rule12 = new FuzzyRule(12, positionZO_setPointNM, then_thetaPS);
FuzzyRule* Rule13 = new FuzzyRule(13, positionZO_setPointNS, then_thetaPS);
FuzzyRule* Rule14 = new FuzzyRule(14, positionZO_setPointPS, then_thetaZO);
FuzzyRule* Rule15 = new FuzzyRule(15, positionZO_setPointPM, then_thetaNS);

fuzzyObj->addFuzzyRule(Rule11);
fuzzyObj->addFuzzyRule(Rule12);
fuzzyObj->addFuzzyRule(Rule13);
fuzzyObj->addFuzzyRule(Rule14);
fuzzyObj->addFuzzyRule(Rule15);

//                          Position PS
FuzzyRuleAntecedent* positionPS_setPointPS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPS_setPointNM = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPS_setPointNS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPS_setPointZO = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPS_setPointPM = new FuzzyRuleAntecedent();

positionPS_setPointPS->joinWithAND(position_ps, setpoint_ps);
positionPS_setPointNM->joinWithAND(position_ps, setpoint_nm);
positionPS_setPointNS->joinWithAND(position_ps, setpoint_ns);
positionPS_setPointZO->joinWithAND(position_ps, setpoint_zo);
positionPS_setPointPM->joinWithAND(position_ps, setpoint_pm);

FuzzyRule* Rule16 = new FuzzyRule(16, positionPS_setPointPS, then_thetaZO);
FuzzyRule* Rule17 = new FuzzyRule(17, positionPS_setPointNM, then_thetaPM);
FuzzyRule* Rule18 = new FuzzyRule(18, positionPS_setPointNS, then_thetaPS);
FuzzyRule* Rule19 = new FuzzyRule(19, positionPS_setPointZO, then_thetaZO);
FuzzyRule* Rule20 = new FuzzyRule(20, positionPS_setPointPM, then_thetaZO);

fuzzyObj->addFuzzyRule(Rule16);
fuzzyObj->addFuzzyRule(Rule17);
fuzzyObj->addFuzzyRule(Rule18);
fuzzyObj->addFuzzyRule(Rule19);
fuzzyObj->addFuzzyRule(Rule20);

//                           Posición PM
FuzzyRuleAntecedent* positionPM_setPointPM = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPM_setPointNM = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPM_setPointNS = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPM_setPointZO = new FuzzyRuleAntecedent();
FuzzyRuleAntecedent* positionPM_setPointPS = new FuzzyRuleAntecedent();

positionPM_setPointPM->joinWithAND(position_pm, setpoint_pm);
positionPM_setPointNM->joinWithAND(position_pm, setpoint_nm);
positionPM_setPointNS->joinWithAND(position_pm, setpoint_ns);
positionPM_setPointZO->joinWithAND(position_pm, setpoint_zo);
positionPM_setPointPS->joinWithAND(position_pm, setpoint_ps);

FuzzyRule* Rule21 = new FuzzyRule(21, positionPM_setPointPM, then_thetaZO);
FuzzyRule* Rule22 = new FuzzyRule(22, positionPM_setPointNM, then_thetaPM);
FuzzyRule* Rule23 = new FuzzyRule(23, positionPM_setPointNS, then_thetaPM);
FuzzyRule* Rule24 = new FuzzyRule(24, positionPM_setPointZO, then_thetaPS);
FuzzyRule* Rule25 = new FuzzyRule(25, positionPM_setPointPS, then_thetaZO);

fuzzyObj->addFuzzyRule(Rule21);
fuzzyObj->addFuzzyRule(Rule22);
fuzzyObj->addFuzzyRule(Rule23);
fuzzyObj->addFuzzyRule(Rule24);
fuzzyObj->addFuzzyRule(Rule25);

}

void logger(float thetaOutput){
  log_position();
  log_setpoint();
  log_theta(thetaOutput);
}

void log_position(){
  Serial.println("");
  Serial.print("Posicion de la esfera: ");  
  Serial.print(position_nm->getPertinence());
  Serial.print(",");
  Serial.print(position_ns->getPertinence());
  Serial.print(",");
  Serial.print(position_zo->getPertinence());
  Serial.print(",");
  Serial.print(position_ps->getPertinence());
  Serial.print(",");
  Serial.println(position_pm->getPertinence());
}

void log_setpoint(){
  Serial.print("Set Point: ");
  Serial.print(setpoint_nm->getPertinence());
  Serial.print(",");
  Serial.print(setpoint_ns->getPertinence());
  Serial.print(",");
  Serial.print(setpoint_zo->getPertinence());
  Serial.print(",");
  Serial.print(setpoint_ps->getPertinence());
  Serial.print(",");
  Serial.println(setpoint_pm->getPertinence());
}

void log_theta(float thetaOutput){
  Serial.print("Theta: ");
  Serial.println(thetaOutput);

  Serial.print("Salida: ");
  Serial.print(theta_nm->getPertinence());
  Serial.print(",");
  Serial.print(theta_ns->getPertinence());
  Serial.print(",");
  Serial.print(theta_zo->getPertinence());
  Serial.print(",");
  Serial.print(theta_ps->getPertinence());
  Serial.print(",");
  Serial.println(theta_pm->getPertinence());
}


void moveServo(float theta){
if(theta <= 0){theta = MIN;}
if(theta >= 45){theta = MAX;}
  
servo.write(theta);
}

