#include <Arduino.h>
#include <Servo.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

#define SERVO_PIN 9
#define IR_DATA_PIN 6
#define MANUAL_PIN 2
#define MODE_LED_PIN 7
#define DELAY_EACH_DEGREE 15
#define STICK_X A0

void setup();
void loop();

Servo servo_motor; // create servo object to control a servo
LiquidCrystal_I2C lcd(0x27, 16, 2);

int position_in_deg = 0; // variable to store the servo position
int IR_VAL = 0;
int ir_flag = 0;
int is_manual = 0;
float stick_x = 0;

int degrees_state[180];

void setup()
{

  Serial.begin(9600);

  lcd.init();

  lcd.setBacklight(HIGH);
  lcd.print("Scanning...");

  servo_motor.attach(SERVO_PIN); // attaches the servo on pin 9 to the servo object

  servo_motor.write(position_in_deg);
  delay(1000);

  pinMode(IR_DATA_PIN, INPUT);
  pinMode(MANUAL_PIN, INPUT);
  pinMode(STICK_X, INPUT);
  pinMode(MODE_LED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(MANUAL_PIN), interrupt_handler, FALLING);
}

void update_lcd()
{

  lcd.clear();

  int left_open = degrees_state[174] || degrees_state[177] || degrees_state[180];
  int front_open = degrees_state[93] || degrees_state[90] || degrees_state[87];
  int right_open = degrees_state[0] || degrees_state[3] || degrees_state[6];

  lcd.println("Open directions:");
  lcd.setCursor(0, 10);

  if (left_open && front_open && right_open)
  {
    Serial.println("All open !");
    lcd.print("All");
  }
  else if (left_open && front_open)
  {
    Serial.println("Left and front open !");
    lcd.print("Left and front");
  }
  else if (left_open && right_open)
  {
    Serial.println("Left and right open !");
    lcd.print("Left and right");
  }
  else if (front_open && right_open)
  {
    Serial.println("Front and right open !");
    lcd.print("Front and right");
  }
  else if (left_open)
  {
    Serial.println("Left open !");
    lcd.print("Left");
  }
  else if (front_open)
  {
    Serial.println("Front open !");
    lcd.print("Front");
  }
  else if (right_open)
  {
    Serial.println("Right open !");
    lcd.print("Right");
  }
  else
  {
    Serial.println("All closed !");
    lcd.print("All closed !");
  }
}

void interrupt_handler()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    is_manual = !is_manual;
    digitalWrite(MODE_LED_PIN, is_manual);
    Serial.println("switching mode !");
  }
  last_interrupt_time = interrupt_time;
}

void loop()
{

  if (is_manual == 0)
  {
    Serial.println("Automatic !");

    for (; position_in_deg <= 180; position_in_deg += 1)
    { // goes from 0 degrees to 180 degrees

      if (is_manual == 1)
      {
        ir_flag = 0;
        break;
      }

      // in steps of 1 degree

      servo_motor.write(position_in_deg); // tell servo to go to position in variable 'position_in_deg'

      delay(DELAY_EACH_DEGREE); // waits 15ms for the servo to reach the position

      IR_VAL = digitalRead(IR_DATA_PIN);

      degrees_state[position_in_deg] = IR_VAL == HIGH;

      if (IR_VAL == LOW && ir_flag == 0)
      {
        Serial.print("open from ");
        Serial.print(position_in_deg);
        Serial.print(" degrees");
        ir_flag = 1;
      }
      if (IR_VAL == HIGH && ir_flag == 1)
      {
        Serial.print(" until ");
        Serial.print(position_in_deg);
        Serial.println(" degrees");
        ir_flag = 0;
      }
    }

    if (is_manual == 0)
    {
      position_in_deg = 180;
      update_lcd();
    }

    for (; position_in_deg >= 0; position_in_deg -= 1)
    { // goes from 180 degrees to 0 degrees

      if (is_manual == 1)
      {
        ir_flag = 0;
        break;
      }

      servo_motor.write(position_in_deg); // tell servo to go to position in variable 'position_in_deg'

      delay(DELAY_EACH_DEGREE); // waits 15ms for the servo to reach the position

      IR_VAL = digitalRead(IR_DATA_PIN);

      degrees_state[position_in_deg] = IR_VAL == HIGH;

      if (IR_VAL == LOW && ir_flag == 0)
      {
        Serial.print("open from ");
        Serial.print(position_in_deg);
        Serial.print(" degrees");
        ir_flag = 1;
      }
      if (IR_VAL == HIGH && ir_flag == 1)
      {
        Serial.print(" until ");
        Serial.print(position_in_deg);
        Serial.println(" degrees");
        ir_flag = 0;
      }
    }
    if (is_manual == 0)
    {
      position_in_deg = 0;
      update_lcd();
    }
  }
  else
  {
    Serial.println("Manual !");
    lcd.clear();
    lcd.print("Manual !   ");

    while (is_manual == 1)
    {
      stick_x = (analogRead(STICK_X) - 511) / 1023.0;

      if (round(stick_x - 0.2) == -1)
      {

        if (position_in_deg < 180)
        {
          position_in_deg += 1;
          servo_motor.write(position_in_deg);
        }
      }
      else if (round(stick_x + 0.2) == 1)
      {
        if (position_in_deg > 0)
        {
          position_in_deg -= 1;
          servo_motor.write(position_in_deg);
        }
      }

      delay(DELAY_EACH_DEGREE); // waits 15ms for the servo to reach the position

      IR_VAL = digitalRead(IR_DATA_PIN);

      if (IR_VAL == LOW && ir_flag == 0)
      {
        Serial.print("open from ");
        Serial.print(position_in_deg);
        Serial.print(" degrees");
        ir_flag = 1;
      }

      if (IR_VAL == HIGH && ir_flag == 1)
      {
        Serial.print(" until ");
        Serial.print(position_in_deg);
        Serial.println(" degrees");
        ir_flag = 0;
      }
    }
  }
}
