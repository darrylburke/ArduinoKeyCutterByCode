#include <LiquidCrystal.h>
#include <Stepper.h>
/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>
//Eeprom Stuff
//--------------------------------------------------------------------------------------------------------------
// LCD Init Stuff
//-------------------------------------------------------------------------------------------------------------
// Button Board
int switch1port=6;
int switch2port=7;
int switch3port=9;
int switch4port=10;
int button1port=22;
int button2port=24;
int button3port=26;
int button4port=28; //down
int button5port=30;
int button6port=32;

int ActionSwitchButton;

int MasterKeySwitch = 0;
int LoadSwitch = 0;
int EjectSwitch = 0;
int WiggleSwitch = 0;
int PrevMasterKeySwitch = 0;
int PrevLoadSwitch = 0;
int PrevEjectSwitch = 0;
int PrevWiggleSwitch = 0;

int SelectButton = 0;
int LeftButton = 0;
int UpButton = 0;
int DownButton = 0;
int RightButton = 0;
int ReturnButton = 0;

int PrevSelectButton = 0;
int PrevLeftButton = 0;
int PrevUpButton = 0;
int PrevDownButton = 0;
int PrevRightButton = 0;
int PrevReturnButton = 0;


boolean ForceAction=false;
int ToggleDepths=1;

int EEPROMKeyIndex=0;
int EEPROMCodeIndex=3;
int PrevSec=0;
String InitString ="Keysaurus Rex v1";

int ActionSwitchPort =8;
String OldMessage1="";
String OldMessage2="";
// initialize the library with the numbers of the interface pins

int CurrentKeyVals=0;
int PrevKeyVals=0;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Menu Configs
bool buttonpressed = false;

int MenuID = 0;
boolean InMenu=false;
char* KeyTypes[] = {
  "SC1","SC4","KW1","M1 "};
int KeyTypesCount=4;
String CurrentKeyType="SC1";
int CurrentKeyCode[] ={
  0,1,2,3,4,5};
int CurrentKeyCodeCount=6;

//--------------------------------------------------------------------------------------------------------------
// End LCD Init Stuff
//-------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
// Key Cutter Init Stuff
//-------------------------------------------------------------------------------------------------------------
boolean eject = true;
boolean load = false;
boolean kw1cwiggle=false;
boolean bumpkey=false;
boolean KeyTweeks=true;

// KW1 Space and Depth
//String KeyType="KW1"; int KeyCode[] = {1,1,1,1,1,1 }; //KW1 1
//String KeyType="KW1"; int KeyCode[] = {2,2,2,2,2,2 }; //KW1 1
//String KeyType="KW1"; int KeyCode[] = {3,3,3,3,3,3 }; //KW1 1
//String KeyType="KW1"; int KeyCode[] = {4,4,4,4,4,4 }; //KW1 1
//String KeyType="KW1"; int KeyCode[] = {5,5,5,5,5,5 }; //KW1 1
//String KeyType="KW1"; int KeyCode[] = {6,6,6,6,6,6 }; //KW1 1
//String KeyType="KW1"; int KeyCode[] = {7,7,7,7,7,7 }; //KW1 1


//Bump Keys
//String KeyType="SC1"; int KeyCode[] = {9,9,9,9,9,0 };// home key
//String KeyType="SC4"; int KeyCode[] = {9,9,9,9,9,9 };// home key


int KeyCode[] = {5,6,3,8,3,4 };  // TEST KEY

// Setup Locations
//------------------------------------------------------------
// SC1  at first cut space, touching top of blank

//# Uno Connection Constants
int Xwire1=45;
int Xwire2=43;
int Xwire3=41;
int Xwire4=39;

int Ywire1=53;
int Ywire2=51;
int Ywire3=49;
int Ywire4=47;

//int XMinFlag = 1;
//int XMaxFlag = 2;
//int YMinFlag = 3;
//int YMaxFlag = 4;
//
//int XMinFlagSwitch;
//int XMaxFlagSwitch;
//int YMinFlagSwitch;
//int YMaxFlagSwitch;


//#Motor Constants
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
int fullturn= stepsPerRevolution;
int halfturn=stepsPerRevolution/2;
int quarterturn=stepsPerRevolution/4;
int eightturn=stepsPerRevolution/8;  // for your motor
Stepper myYStepper(stepsPerRevolution, Xwire1,Xwire2,Xwire3,Xwire4);           
Stepper myXStepper(stepsPerRevolution, Ywire1,Ywire2,Ywire3,Ywire4);

double yRotationDistance=0.059;
double xRotationDistance=0.079;

// # Cutter Variables

int CuttingDelay=60;

//# Generic Constants
double ShoulderToFirstCut=0.231;
double CenterToCenter=0.156;
double DepthIncrement=0.015;
double SpaceIncrement=0.156;
double Depths[] = {
  0.335,0.320,0.305,0.290,0.275,0.260,0.245,0.230,0.215,0.200};
double Spaces[] = {
  0,0.231,0.387,0.543,0.699,0.855,1.001};
double DepthCount = 10;
double SpaceCount = 6;
double MACS=7;
double StepProgression=2;
double AngleOfCut=100; //degrees
//double CutRootWidth=0.031;

double KeyTweekX=0;
double KeyTweekY=0;
double BlankHeight=0.339;
//#Key Constants

double SCHLAGEShoulderToFirstCut=0.231;
double SCHLAGECenterToCenter=0.156;
double SCHLAGEBlankHeight=0.339;
double SCHLAGEDepthIncrement=0.015;
double SCHLAGESpaceIncrement=0.156;

double SCHLAGEKeyTweekY=0.00;
double SCHLAGEKeyTweekX=0.00;


double SCHLAGEDepths[] = {
  0.335,0.320,0.305,0.290,0.275,0.260,0.245,0.230,0.215,0.200};
double SCHLAGEDepthCount = 10;
double SCHLAGESpaceCount = 6;
double SCHLAGESpaces[] = {
  0,0.231,0.387,0.543,0.699,0.855,1.001};
double SCHLAGEMACS=7;
double SCHLAGEStepProgression=2;
double SCHLAGEAngleOfCut=100; //degrees
double SCHLAGECutRootWidth=0.031;

double KWIKSETKeyTweekY=0.015;
double KWIKSETKeyTweekX=0.00;

double KWIKSETBlankHeight=0.333;
double KWIKSETShoulderToFirstCut=0.247;
double KWIKSETCenterToCenter=0.150;
double KWIKSETDepthIncrement=0.023;
double KWIKSETSpaceIncrement=0.150;
double KWIKSETDepths[] = {
  0.333,0.328,0.305,0.282,0.259,0.236,0.213,0.190,0.333,0.333};
double KWIKSETDepthCount = 8;
double KWIKSETSpaces[] = {
  0.0,0.247,0.397,0.547,0.697,0.847,0.997};
double KWIKSETSpaceCount = 6;
double KWIKSETMACS=4;
double KWIKSETStepProgression=1;
double KWIKSETAngleOfCut=100; //degrees
double KWIKSETCutRootWidth=0.031;


double M1KeyTweekY=0.00;
double M1KeyTweekX=0.026;
double M1ShoulderToFirstCut=0.187;
double M1CenterToCenter=0.125;
double M1BlankHeight=0.272;
double M1DepthIncrement=0.0155;
double M1SpaceIncrement=0.156;
//Light key 2,1,2,3,0,0
double M1Depths[] = {
  0.272,0.2565,0.2410,0.2255,0.2100,0.1945,0.1790,0.1635,0.272,0.272};
double M1DepthCount = 7;
double M1SpaceCount = 4;
double M1Spaces[] = {
  0.213,0.338,0.463,0.588,0.713,0.838,0.963};
double M1MACS=7;
double M1StepProgression=2;
double M1AngleOfCut=100; //degrees
double M1CutRootWidth=0.272;



//# Switch Constants
int DoAction = 52;

int Y1 = 2;
int Y2 = 3;
int X1 = 5;
int X2 = 4;
int StartButton = 14;

int DoActionSwitch;

int Y1Switch;
int Y2Switch;
int X1Switch;
int X2Switch;
int StartSwitch;

int counterswitch;  
int clockwiseswitch;  
int reading;  

double time = 0;         // the last time the output pin was toggled
double debounce = 50;   // the debounce time, increase if the output flickers



//--------------------------------------------------------------------------------------------------------------
// End Key Cutter Init Stuff
//-------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  // set the speed at 60 rpm:
  myYStepper.setSpeed(100);
  myXStepper.setSpeed(100);

  pinMode(switch1port, INPUT);
  pinMode(switch2port, INPUT);
  pinMode(switch3port, INPUT);
  pinMode(switch4port, INPUT);
  pinMode(button1port, INPUT);
  pinMode(button2port, INPUT);
  pinMode(button3port, INPUT);
  pinMode(button4port, INPUT);
  pinMode(button5port, INPUT);
  pinMode(button6port, INPUT);

  // set up the LCD's number of columns and rows: 
  InitLCD();
  DisplayMessage(InitString,"By: Darryl Burke");
  delay(1000);
  //WriteCurrentKeyType();
  GetCurrentKeyType();
  MenuID=0;
  DisplayMenu(); 
}

void loop() {
  int Sec=millis()/1000;
  if (Sec!=PrevSec) {
    ForceAction=true;
    PrevSec=Sec;
  }
  GetSwitches();
  PrintSwitches();
  if (ForceAction)
    ForceAction =false;
  else 
  {  
    if (!CheckForNewKey())
      return; 
  }

  if (ActionSwitchButton==1){
    CutKey();
  } 
  else {
    CheckMenus() ; 
  }

}


void GetCurrentKeyType() {
  Serial.print("Reading: ");
  int _currentkeytype = EEPROM.read(EEPROMKeyIndex);
  Serial.print(_currentkeytype);
  Serial.print(":");
  CurrentKeyType == KeyTypes[_currentkeytype];
  for (int x=0; x<CurrentKeyCodeCount ;x++) { 
    byte _memval = EEPROM.read(EEPROMCodeIndex+x);
    Serial.print(_memval, DEC); 
    Serial.print("-");
    //  Serial.print(_kode);
    CurrentKeyCode[x]=_memval;
  }
  Serial.println("");
}
void WriteCurrentKeyType() {
  Serial.print("Writing: ");
  for (int x=0;x<KeyTypesCount;x++){
    if ( CurrentKeyType == KeyTypes[x])
    {
      EEPROM.write(EEPROMKeyIndex, x);
      Serial.print(x);
    }
  }
  Serial.print(":");

  for (int x=0; x<CurrentKeyCodeCount ;x++) {

    EEPROM.write(EEPROMCodeIndex+x, CurrentKeyCode[x]);
    Serial.print(CurrentKeyCode[x]);
    Serial.print("-");
  }
  Serial.println("");
}
void InitLCD()
{
  lcd.begin(16, 2); 
}

void DisplayMenu() {
  switch (MenuID) {
  case 0:
    DisplayMainMenu("");
    break;
  case 1:
    DisplayMoveMenu();
    break;
  case 2:
    DisplaySetKeyMenu();
    // statements
    break;
  default: 
    DisplayErrorMenu();
  }
}
String getKeyString() {
  String _dsp = "";
  _dsp="Key:["+CurrentKeyType+"]";

  for (int x=0; x<CurrentKeyCodeCount ;x++) {
    _dsp += CurrentKeyCode[x];
  }
  return _dsp;
}
void DisplayMainMenu(String Opt) 
{
  String _dsp1="";
  String _dsp2="";
  _dsp1=getKeyString();

  if (Opt != "")
    _dsp2=Opt;
  else{
    if (MenuID==0)
      _dsp2="Action: Cut";
    else if (MenuID==1)
      _dsp2="Action: Move";
    else if (MenuID==2)
      _dsp2="Action: Set Key";
  }
  DisplayMessage( _dsp1,_dsp2);
}

void DisplayMoveMenu(){
  String _dsp1="";
  String _dsp2="";
  _dsp1="Moving Carriage";
  _dsp2="StepSize:";
  if (WiggleSwitch==1)
    _dsp2 += "Key D&S";
  else 
    _dsp2+= "Fixed";
  DisplayMessage( _dsp1,_dsp2);

  if (UpButton==1) 
    moveY2();
  else if (DownButton==1)
    moveY1();
  else if (LeftButton==1)
    moveX2();
  else if (RightButton==1)
    moveX1();



}
int KeySetVar=0;
void SetKeyType(String Type){

  CurrentKeyType=Type;
  WriteCurrentKeyType();

}
int CurrentDepthPosition=0;
boolean InDepthEdit=false;

void DisplaySetKeyMenu(){
  // Serial.println("ABC");

  String _dsp1="";
  String _dsp2="";

  _dsp1 = getKeyString();

  if (KeySetVar ==0)
    _dsp2="Set: KeyType";
  else if (KeySetVar ==1 )
    _dsp2="Set: Depths";
  else if (KeySetVar ==2 )
    _dsp2="Set: Save";


  if (KeySetVar ==0) {
    if (UpButton==1){
      KeySetVar=2;
      _dsp2="Set: Save";
    }
    else if (DownButton==1) {
      KeySetVar=1;
      _dsp2="Set: Depths";
    }
    else {

      if (RightButton==1 && CurrentKeyType=="SC4")
        SetKeyType("KW1");
      else if (RightButton==1 && CurrentKeyType=="SC1")
        SetKeyType("SC4");
      else if (RightButton==1 && CurrentKeyType=="SC4")
        SetKeyType("KW1");
      else if (RightButton==1 && CurrentKeyType=="KW1")
        SetKeyType("M1 ");
      else if (RightButton==1 && CurrentKeyType=="M1 ")
        SetKeyType("SC1");
      else if (LeftButton==1 && CurrentKeyType=="KW1")
        SetKeyType("SC4");
      else if (LeftButton==1 && CurrentKeyType=="SC4")
        SetKeyType("SC1");
      else if (LeftButton==1 && CurrentKeyType=="SC1")
        SetKeyType("M1 ");
      else if (LeftButton==1 && CurrentKeyType=="M1 ")
        SetKeyType("KW1");        
    }
  }

  else if (KeySetVar ==1){

    if (UpButton==1&&!InDepthEdit){
      KeySetVar=0;
      _dsp2="Set: KeyType";
    }
    else if (DownButton==1&&!InDepthEdit) {
      KeySetVar=2;
      _dsp2="Set: Save";
    } 
    else if (ReturnButton==1&&!InDepthEdit){
      KeySetVar=2;
      _dsp2="Set: Save";
//      Serial.println("ASDASDADSADS");
      ReturnButton=0;
    }  
    else if ( SelectButton==1&&!InDepthEdit)
    {
      InDepthEdit=true;
    } 
    else if (ReturnButton==1 && InDepthEdit==true){
  //    Serial.println("WAWAWAWAW");
      InDepthEdit=false;
      ReturnButton=0;
    }
    else {

      if (InDepthEdit) 
      {
        if (LeftButton==1 && CurrentDepthPosition == 0) {
          CurrentDepthPosition=6; 
        }
        else if (LeftButton==1 && CurrentDepthPosition == 1) {
          CurrentDepthPosition=0; 
        }
        else if (LeftButton==1 && CurrentDepthPosition == 2) {
          CurrentDepthPosition=1; 
        }
        else if (LeftButton==1 && CurrentDepthPosition == 3) {
          CurrentDepthPosition=3; 
        }
        else if (LeftButton==1 && CurrentDepthPosition == 4) {
          CurrentDepthPosition=3; 
        }
        else if (LeftButton==1 && CurrentDepthPosition == 5) {
          CurrentDepthPosition=4; 
        }
        else if (LeftButton==1 && CurrentDepthPosition == 6) {
          CurrentDepthPosition=5; 
        }
        else if (RightButton==1 && CurrentDepthPosition == 0) {
          CurrentDepthPosition=1; 
        }
        else if (RightButton==1 && CurrentDepthPosition == 1) {
          CurrentDepthPosition=2; 
        }
        else if (RightButton==1 && CurrentDepthPosition == 2) {
          CurrentDepthPosition=3; 
        }
        else if (RightButton==1 && CurrentDepthPosition == 3) {
          CurrentDepthPosition=4; 
        }
        else if (RightButton==1 && CurrentDepthPosition == 4) {
          CurrentDepthPosition=5; 
        }
        else if (RightButton==1 && CurrentDepthPosition == 5) {
          CurrentDepthPosition=0; 
        }
        else if (UpButton==1) {
          int _tmp= CurrentKeyCode[CurrentDepthPosition];
          if (_tmp==9)
            CurrentKeyCode[CurrentDepthPosition]=0;
          else 
            CurrentKeyCode[CurrentDepthPosition]=_tmp+1;   
        }
        else if (DownButton==1) {
          int _tmp= CurrentKeyCode[CurrentDepthPosition];
          if (_tmp==0)
            CurrentKeyCode[CurrentDepthPosition]=9;
          else 
            CurrentKeyCode[CurrentDepthPosition]=_tmp-1;   
        }
        //up down set for depth

        if(ToggleDepths==0){
          _dsp1.setCharAt(9+CurrentDepthPosition,'_');
          ToggleDepths=1;
        } 
        else {
          ToggleDepths=0;  
        }

      } 
      else {

      }

    }

  }
  else if (KeySetVar ==2){
    if (UpButton==1){
      KeySetVar=1;
      _dsp2="Set: Depths";
    }
    else if (DownButton==1) {
      KeySetVar=0;
      _dsp2="Set: KeyType";
    }
    else if (SelectButton==1) {

      WriteCurrentKeyType();
      _dsp1 = getKeyString();
      _dsp2 = "Saved Key Type";
      DisplayMessage( _dsp1,_dsp2);
      delay (1000);
      KeySetVar=0;
      _dsp2="Set: KeyType";
    }
  }








  DisplayMessage( _dsp1,_dsp2);

}





void DisplayErrorMenu(){
  String _dsp1="";
  String _dsp2="";
  _dsp1="Error";
  _dsp2="";

  DisplayMessage( _dsp1,_dsp2);

}
void DisplayMessage(String Message1, String Message2){
  if (OldMessage1 != Message1 || OldMessage2 != Message2){
    ClearLCD();
    lcd.setCursor(0,0);
    // Print a message to the LCD.
    lcd.print(Message1);
    // set the cursor to column 0, line 2
    // (note: line 2 is the second row, since first  row is 1):
    lcd.setCursor(0, 2);
    // print the number of seconds since reset:
    lcd.print(Message2);
    OldMessage1=Message1;
    OldMessage2=Message2;
  }
}
void ClearLCD() {
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  lcd.print("                ");
  // set the cursor to column 0, line 2
  // (note: line 2 is the second row, since first  row is 1):
  lcd.setCursor(0, 2);
  // print the number of seconds since reset:
  lcd.print("                ");

}
int counter=0;

boolean CheckForNewKey() {

  if (PrevSelectButton != SelectButton){
    PrevSelectButton = SelectButton;
    return true;
  }
  else if ( PrevLeftButton != LeftButton){
    PrevLeftButton = LeftButton;
    return true;
  }
  else if (PrevUpButton != UpButton){
    PrevUpButton = UpButton;
    return true;
  }
  else if (PrevDownButton != DownButton){
    PrevDownButton = DownButton;
    return true;
  }
  else if (PrevRightButton != RightButton){
    PrevRightButton = RightButton;
    return true;
  }
  else if (PrevReturnButton != ReturnButton){
    PrevReturnButton = ReturnButton;
    return true;
  }
  if (PrevMasterKeySwitch != MasterKeySwitch){
    if (MasterKeySwitch ==1)
      DisplayMessage("Bump Key Switch","On");
    else 
      DisplayMessage("Bump Key Switch","Off");
    delay(1000);
    PrevMasterKeySwitch = MasterKeySwitch;
    return true;
  }
  else if (PrevEjectSwitch != EjectSwitch){
    PrevEjectSwitch = EjectSwitch;
     if (EjectSwitch ==1)
      DisplayMessage("Eject Switch","On");
    else 
      DisplayMessage("Eject Switch","Off");
    delay(1000);
    
    return true;
  }
  else if (PrevLoadSwitch != LoadSwitch){
    PrevLoadSwitch = LoadSwitch;
     if (LoadSwitch == 1)
      DisplayMessage("Load Switch","On");
    else 
      DisplayMessage("Load Switch","Off");
    delay(1000);
    
    return true;
  }
  else if (PrevWiggleSwitch != WiggleSwitch){
    PrevWiggleSwitch = WiggleSwitch;
     if (WiggleSwitch ==1)
      DisplayMessage("Wiggle Switch","On");
    else 
      DisplayMessage("Wiggle Switch","Off");
    delay(1000);
    return true;
  }


  return false;

}

void GetSwitches(){


  ActionSwitchButton= digitalRead(ActionSwitchPort);
  MasterKeySwitch = digitalRead(switch1port);
  LoadSwitch = digitalRead(switch2port);
  EjectSwitch = digitalRead(switch3port);
  WiggleSwitch = digitalRead(switch4port);
  SelectButton = digitalRead(button1port);
  LeftButton = digitalRead(button2port);
  UpButton = digitalRead(button3port);
  DownButton = digitalRead(button4port);
  RightButton = digitalRead(button5port);
  ReturnButton = digitalRead(button6port);
  delay(100);

}



void CheckMenus(){
  if (InMenu) {

    DisplayMenu();


    if (ReturnButton==1){
      Serial.println("Going Back");
      InMenu=false;
    }
  } 
  else 

  {


    if (MenuID==0 && (DownButton==1 || RightButton==1)) {
      MenuID=1; 
    }
    else if ( MenuID==0 && (UpButton==1 || LeftButton==1)) {
      MenuID=2;
    }
    else if ( MenuID==1 && (DownButton==1 || RightButton==1)) {
      MenuID=2;
    }
    else if ( MenuID==1 && (UpButton==1||LeftButton==1)) {
      MenuID=0;
    }
    else if ( MenuID==2 && (DownButton==1||RightButton==1)) {
      MenuID=0;
    }
    else if ( MenuID==2 && (UpButton==1||LeftButton==1)) {
      MenuID=1;
    }

    if (SelectButton==1 && MenuID==0)  {
      // Perform Cut 
      InMenu=true;

      //Start Cutting Command
      //delay(5000);
      CutKey();
      delay(1000);
      InMenu=false;

    }
    else if (SelectButton==1 && MenuID==1)  
    {
      DisplayMoveMenu();
      InMenu=true;
      return;
    } 
    else if (SelectButton==1 && MenuID==2)  
    {
      DisplaySetKeyMenu();
      InMenu=true;
      return;
    }
    DisplayMainMenu("");



    //  if (SelectButton==1 ||LeftButton==1||UpButton==1||RightButton==1||DownButton==1||ReturnButton==1)
    //    DisplayMessage("You Pressed",_str);
    //  else {
    //    if (ActionSwitchButton==0){
    //      if (buttonpressed)
    //        buttonpressed=false;
    //      if (MenuID==0) {
    //        DisplayMainMenu("");
    //      }
    //    }
    //    else 
    //    {
    //      if (!buttonpressed){
    //        for (int x=0; x<CurrentKeyCodeCount;x++) {
    //          CurrentKeyCode[x] = CurrentKeyCode[x]+1;
    //        }
    //        WriteCurrentKeyType();
    //        buttonpressed=true;  
    //      }
    //
    //
    //    }
    //
    //
  } 
}
void PrintSwitches(){

  Serial.print("Switches Mstr:");
  Serial.print(MasterKeySwitch);
  Serial.print(" Load:");
  Serial.print(LoadSwitch);
  Serial.print(" Eject:");
  Serial.print(EjectSwitch);
  Serial.print(" Wiggle:");
  Serial.print(WiggleSwitch);

  Serial.print("   Buttons Sel:");
  Serial.print(SelectButton);
  Serial.print(" Left:");
  Serial.print(LeftButton);
  Serial.print(" Up:");
  Serial.print(UpButton);
  Serial.print(" Down:");
  Serial.print(DownButton);
  Serial.print(" Right:");
  Serial.print(RightButton);
  Serial.print(" Return:");
  Serial.print(ReturnButton);
  Serial.println("");

  String _str="";
  _str+=String(MasterKeySwitch);
  _str+=String(LoadSwitch);
  _str+=String(EjectSwitch);
  _str+=String(WiggleSwitch);

  _str+=String(SelectButton);
  _str+=String(LeftButton);
  _str+=String(UpButton);
  _str+=String(DownButton);
  _str+=String(RightButton);
  _str+=String(ReturnButton);


}

//==========================================================================================
//==========================================================================================
//==========================================================================================

void moveY1(){
  Serial.println("Moving Y1 Direction");
  myYStepper.step(stepsPerRevolution);
  //myYStepper.step(eightturn);
}
void moveY2(){
  Serial.println("Moving Y2 Direction");
  myYStepper.step(-stepsPerRevolution);
}
void moveX1(){
  Serial.println("Moving X1 Direction");
  myXStepper.step(stepsPerRevolution);
}
void moveX2(){
  Serial.println("Moving X2 Direction");
  myXStepper.step(-stepsPerRevolution);
}
void do45X1Y1() 
{
  Serial.println("Moving 45 X1 & Y1");
  for (int x = 0; x < stepsPerRevolution; x++)  {
    myYStepper.step(1);
    myXStepper.step(1);      
  }
}

void do45X2Y2() 
{
  Serial.println("Moving 45 X1 & Y1");
  for (int x = 0; x < stepsPerRevolution; x++)  {
    myYStepper.step(-1);
    myXStepper.step(-1);      
  }
}

void do45X1Y2() 
{
  Serial.println("Moving 45 X1 & Y1");
  for (int x = 0; x < stepsPerRevolution; x++)  {
    myYStepper.step(-1);
    myXStepper.step(1);      
  }
}

void do45X2Y1() 
{
  Serial.println("Moving 45 X1 & Y1");
  for (int x = 0; x < stepsPerRevolution; x++)  {
    myYStepper.step(1);
    myXStepper.step(-1);      
  }
}


void doActions(int time) 
{
  Serial.println("Do Action");

  for (int x = 0; x < time; x++){
    moveX2();
  }
  for (int x = 0; x < time; x++){
    do45X1Y1();
  }

  for (int x = 0; x < time; x++){
    moveX2();
  }

  for (int x = 0; x < time; x++){
    do45X1Y2();
  }
}


void ResetXY() 
{


}
void DoEject(){
  MoveKey("Up",1.5,0);     
  MoveKey("Right",Spaces[0],0);
}
void DoLoad(){
  MoveKey("Down",1.5,0);    
  MoveKey("Left",Spaces[0],0);
}
void CutKey() {
  // Set Key Variables
  Serial.println ("Starting Cutting");
  DisplayMainMenu("Starting Key Cutting");


eject = boolean(EjectSwitch);
load = boolean(LoadSwitch);
kw1cwiggle=boolean(WiggleSwitch);
bumpkey=boolean(MasterKeySwitch);


  SetKeyType(); 

  if (load) {
    DoLoad();
  }

  // Go to First 0 position on Key
  MoveToFirstCut();
  // Go through Key Depths and Spaces 
  StartCutting();  
  //
  FinishCutting();

  if (eject) {
    DoEject();
  }
  DisplayMainMenu("Key Cutting Complete");
}
void FinishCutting() {
  UpCut() ;  
  // move back one
  for (int x=0 ; x < SpaceCount ; x++) 
  {
    MoveKey("Left",SpaceIncrement,0);     
  }
  DownCut();




}

void SetKeyType() 
{
  Serial.println("Setting Key Type");
  if (  CurrentKeyType == "SC1" ||CurrentKeyType == "SC4"  ) {


    KeyTweekX=SCHLAGEKeyTweekX;
    KeyTweekY=SCHLAGEKeyTweekY;
    ShoulderToFirstCut=SCHLAGEShoulderToFirstCut;
    CenterToCenter=SCHLAGEShoulderToFirstCut;
    DepthIncrement=SCHLAGEDepthIncrement;
    SpaceIncrement=SCHLAGESpaceIncrement;
    DepthCount = SCHLAGEDepthCount;
    BlankHeight=SCHLAGEBlankHeight;
    MACS=SCHLAGEMACS;
    StepProgression=SCHLAGEStepProgression;
    AngleOfCut=SCHLAGEAngleOfCut; //degrees
    //CutRootWidth=SCHLAGECutRootWidth;
    if (CurrentKeyType == "SC1"){
      SpaceCount = 5;
    } 
    else if (CurrentKeyType == "SC4"){
      SpaceCount = 6;
    }
  }
  else if (  CurrentKeyType == "KW1") {

    KeyTweekX=KWIKSETKeyTweekY;
    ShoulderToFirstCut=KWIKSETShoulderToFirstCut;
    CenterToCenter=KWIKSETShoulderToFirstCut;
    DepthIncrement=KWIKSETDepthIncrement;
    SpaceIncrement=KWIKSETSpaceIncrement;
    DepthCount = KWIKSETDepthCount;
    SpaceCount = 5; 
    MACS=KWIKSETMACS;
    BlankHeight=KWIKSETBlankHeight;
    StepProgression=KWIKSETStepProgression;
    AngleOfCut=KWIKSETAngleOfCut; //degrees
    // CutRootWidth=KWIKSETCutRootWidth;


  } else if ( CurrentKeyType == "M1 "){
    
    KeyTweekX=M1KeyTweekY;
    ShoulderToFirstCut=M1ShoulderToFirstCut;
    CenterToCenter=M1ShoulderToFirstCut;
    DepthIncrement=M1DepthIncrement;
    SpaceIncrement=M1SpaceIncrement;
    DepthCount = M1DepthCount;
    SpaceCount = 4; 
    MACS=M1MACS;
    BlankHeight=M1BlankHeight;
    StepProgression=M1StepProgression;
    AngleOfCut=M1AngleOfCut; //degrees
  }


  for (int x=0 ; x < DepthCount ; x++) 
  {

    if (  CurrentKeyType == "SC1" ||CurrentKeyType == "SC4")
    {
      //      Serial.print("Setting Depth[");
      //      Serial.print(x);
      //      Serial.print("]=");
      //      Serial.print(SCHLAGEDepths[x],4);
      Depths[x] = SCHLAGEDepths[x];
    }
    else if (  CurrentKeyType == "KW1")
    {
      Depths[x] = KWIKSETDepths[x];
    }
    else if (  CurrentKeyType == "M1 ")
    {
      Depths[x] = M1Depths[x];
    }
  }

  //Spaces = SCHLAGESpaces;
  for (int x=0 ; x < SpaceCount ; x++) 
  {
    if (  CurrentKeyType == "SC1"||CurrentKeyType == "SC4")
    {
      Spaces[x] = SCHLAGESpaces[x];
    }
    else if (  CurrentKeyType == "KW1")
    {
      Spaces[x] = KWIKSETSpaces[x];
    }
    else if (  CurrentKeyType == "M1 ")
    {
      Spaces[x] = M1Spaces[x];
    }
  }


}
void  MoveToFirstCut(){
  Serial.println("Moving to First Cut");

  Serial.print("I'm at:");
  Serial.print(BlankHeight,4);
  Serial.print(" moving to:");
  Serial.print(Depths[0],4);
  // move from current location to the first 0 cut location
  double _ToZeroDist = BlankHeight - Depths[0];
  if (KeyTweeks){

    if ( KeyTweekX > 0)
      MoveKey("Up",KeyTweekX,CuttingDelay);
    if (KeyTweekX < 0)
      MoveKey("Down",KeyTweekX*-1,CuttingDelay);

    if ( KeyTweekY > 0)
      MoveKey("Right",KeyTweekY,CuttingDelay);
    if (KeyTweekY < 0)
      MoveKey("Left",KeyTweekY*-1,CuttingDelay);

  }

  Serial.print("Moving Down:");
  Serial.println(_ToZeroDist,4);
  MoveKey("Down",_ToZeroDist,CuttingDelay);
  delay(3000);
  //  HWiggle();
  //  VWiggle();
  //  
  //  for (int backout =0 ; backout < 2;backout ++ ) {
  //
  //      //move Forwards 1 step
  //      UpCut(); 
  //    }
  //    MoveKey("Left",ShoulderToFirstCut,0);
  //    for (int backout =0 ; backout < 2;backout ++ ) {
  //
  //      //move Forwards 1 step
  //      DownCut(); 
  //    }
  //    HWiggle();
  //    MoveKey("Down",0.05,0);
  //    HWiggle();
}
void HWiggle () 
{
  Serial.println("Wiggling");
  MoveKey("Left",0.030,0);
  MoveKey("Right",0.06,0);
  MoveKey("Left",0.03,0);

}
void CWiggle () 
{
  Serial.println("Wiggling");
  MoveKey("Left",0.015,100);
  MoveKey("Right",0.03,100);
  MoveKey("Left",0.015,0);

}

void VWiggle () 
{
  Serial.println("Wiggling");
  MoveKey("Up",0.030,0);
  MoveKey("Down",0.06,0);
  MoveKey("Up",0.03,0);

}
void DownCut() {
  Serial.println("Cutting Down");
  MoveKey("Down",DepthIncrement,CuttingDelay);
}
void UpCut() {
  Serial.println("Cutting Up");
  MoveKey("Up",DepthIncrement,0); 
}
void MoveToNextCut() {
  Serial.println("Moving to Next Cut");
  MoveKey("Right",SpaceIncrement,0); 
}

void MoveKey(String Direction, double Distance, int mydelay) 
{
  Serial.print("Cutting Key - Direction [");
  Serial.print(Direction);
  Serial.print("] Distance [");
  Serial.print(Distance,4);
  Serial.print("] Delay [");
  Serial.print(mydelay);
  Serial.println("]");

  //get # of steps for distance
  int StepsRequired=0;
  int StepHolder = 1;


  if (Direction == "Left" || Direction == "Right")
    StepsRequired = Distance / xRotationDistance * stepsPerRevolution;
  else if (Direction == "Up" || Direction == "Down")
    StepsRequired = Distance / yRotationDistance * stepsPerRevolution;

  if (Direction == "Up" || Direction == "Left")
    StepHolder=-1;

  Serial.print("Moving ");
  Serial.print(StepsRequired );
  Serial.print(" Steps [");
  Serial.print(Direction);
  Serial.println("]");

  //  Serial.print("Step:");
  //  Serial.print(x);
  //  Serial.print(" of ");
  //  Serial.println(StepsRequired);


  if (mydelay==0)
  {
    if (Direction == "Up" || Direction == "Down")
      myYStepper.step(StepHolder*StepsRequired);
    else if (Direction == "Left" || Direction == "Right")
      myXStepper.step(StepHolder*StepsRequired);

  }
  else {

    for (int x=0 ; x< StepsRequired;x++){
      if (Direction == "Up" || Direction == "Down")
        myYStepper.step(StepHolder);
      else if (Direction == "Left" || Direction == "Right")
        myXStepper.step(StepHolder);
      delay(mydelay);
    }
  }


}
void  StartCutting(){
  //assume they are at the 0 cut

  Serial.print("Cutting Key - Type:[");
  Serial.print(CurrentKeyType);
  Serial.print("]  Code:[");
  for (int z=0;z< SpaceCount;z++) 
  {
    Serial.print(CurrentKeyCode[z]); 
  }

  Serial.print("]");
  int _tmpSpace =SpaceCount;

  for (int z=0; z<=SpaceCount ; z++ ) 
  {
    int thiscut;
    if (z==SpaceCount && !bumpkey){
      continue;
    }
    else {
      thiscut = CurrentKeyCode[z-1];
    }
    thiscut = CurrentKeyCode[z];
    //go forward and cut the key

    Serial.print("----------------[ #");
    Serial.print(SpaceCount);
    Serial.print(" ] Depth:[");
    Serial.print(thiscut);
    Serial.println(" ]-------------------------------------------");

    for(int depths = 0; depths <thiscut;depths ++ ) 
    {
      DownCut();  
    }

    if (CurrentKeyType == "KW1")
      if (kw1cwiggle)
        CWiggle();

    //backout
    // HWiggle();
    Serial.println("Finished cut, backing out");
    for(int depths = 0; depths <thiscut;depths ++ ) 
    {
      UpCut();  
    }
    // move to next cut
    Serial.println("Moving to next Start Position");
    //HWiggle();
    for (int backout =0 ; backout < 2;backout ++ ) {

      //move backwards 1 step
      UpCut();
    }
    // move to next cut
    MoveToNextCut();
    for (int backout =0 ; backout < 2;backout ++ ) {

      //move Forwards 1 step
      DownCut(); 
    }
    //  HWiggle();

  }

}  





