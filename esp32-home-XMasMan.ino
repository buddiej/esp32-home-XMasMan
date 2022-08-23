/*****************************************************************************************/
/* esp32-home-XMasMan.ino sketch v0.1                                                            */
/* Year 2022 by Jens Bundfuss                                                            */
/*                                                                                       */
/* - initial revision                                                                    */
/*                                                                                       */
/*****************************************************************************************/

/*****************************************************************************************/
/*                                    GENERAL DEFINE                                     */
/*****************************************************************************************/
#define PIN_GPIO_PIR_DATA   14
#define PIN_RELAY_OUT       15
#define PIN_ANI_POT         2

#define TRUE    1
#define FALSE   0

/*****************************************************************************************/
/*                                         VARIABLES                                     */
/*****************************************************************************************/
/* chip id */
uint64_t chipid; 

/* timer and detect */
uint32_t timer = 0;
uint8_t detect;

/* Potentiometer values */
uint16_t ValPotResult;
uint16_t ValPotMapped;

/*************************************************************************************************/
/**************************************************************************************************
Function: setup()
return: void
**************************************************************************************************/
/*************************************************************************************************/
void setup() {
  Serial.begin(115200);

  Serial.println("Init of esp32-home-XMasMan ....");

  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  pinMode(PIN_GPIO_PIR_DATA, INPUT);

  pinMode(PIN_RELAY_OUT, OUTPUT);
  digitalWrite(PIN_RELAY_OUT, 0);
  
  /* global delay 1s */
  delay(500);
}

/*************************************************************************************************/
/**************************************************************************************************
Function: loop()
return: void
**************************************************************************************************/
/*************************************************************************************************/
void loop() 
{
  /* Read value from potentiometer and map from raw 0 .... 4095 to result in range from 20s to 120s time */
  ValPotResult = analogRead(PIN_ANI_POT);
  int mappedVal = map(ValPotResult,0,4095,20,120);
  Serial.printf("Pot Value %d\n",mappedVal);  

  /* check for PIR detect */
  detect = digitalRead(PIN_GPIO_PIR_DATA);

  /* move detected? */
  if(detect == TRUE)
  {
    Serial.printf("PIR detect. Init time %d\n",mappedVal);
    timer = mappedVal;
    detect = FALSE; 
  }

  /* check for time greater zero? */
  if (timer > 0)
  {
    /* enable Relay */
    timer--;
    Serial.printf("Act timer %d\n",timer);
    
    digitalWrite(PIN_RELAY_OUT, TRUE);
    Serial.println("RELAY TRUE");
  }
  else
  {
    /* disable relay */
    digitalWrite(PIN_RELAY_OUT, FALSE);
    Serial.println("RELAY FALSE");
  }
  

  /* global delay 1s */
  delay(1000);
}
