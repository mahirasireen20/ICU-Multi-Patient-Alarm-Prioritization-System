#define HEART_SENSOR 34

#define GREEN_LED 13
#define YELLOW_LED 12
#define RED_LED 14

#define BUZZER 18
#define PUSH_BUTTON 19

// Adjust this according to your sensor
int threshold = 2000;

bool pulseDetected = false;

unsigned long lastBeatTime = 0;
unsigned long lastDisplayTime = 0;

int bpm = 0;

bool alarmSilenced = false;

// FIX: BPM goes stale forever without this. If no valid beat arrives
// within this window, assume finger removed / signal lost, and reset
// bpm to 0 so the display falls back to WAITING instead of showing a
// stale (possibly "NORMAL") reading with no finger present.
const unsigned long BEAT_TIMEOUT = 2500; // ms

// FIX: non-blocking button debounce (was delay(200), which froze
// pulse sampling and display timing every press)
unsigned long lastButtonPress = 0;
const unsigned long BUTTON_DEBOUNCE = 200; // ms

void setup()
{
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(PUSH_BUTTON, INPUT_PULLUP);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  Serial.println("================================");
  Serial.println("   ICU MULTI-PATIENT MONITOR");
  Serial.println("          PATIENT 1");
  Serial.println("================================");
  Serial.println("Place finger on sensor");
}

void loop()
{
  // Read sensor
  int sensorValue = analogRead(HEART_SENSOR);
  unsigned long now = millis();

  // --------------------------------
  // HEARTBEAT DETECTION
  // --------------------------------

  if (sensorValue > threshold)
  {
    if (pulseDetected == false)
    {
      pulseDetected = true;

      if (lastBeatTime != 0)
      {
        unsigned long beatTime = now - lastBeatTime;

        // Valid heartbeat interval
        if (beatTime >= 400 && beatTime <= 1500)
        {
          bpm = 60000 / beatTime;

          // Display detected beat
          Serial.print("Beat detected! BPM = ");
          Serial.println(bpm);
        }
      }

      lastBeatTime = now;
    }
  }
  else
  {
    pulseDetected = false;
  }

  // FIX: reset bpm if no beat for too long (finger removed or
  // signal lost) instead of holding the last reading
  if (bpm > 0 && lastBeatTime != 0 && (now - lastBeatTime > BEAT_TIMEOUT))
  {
    bpm = 0;
    lastBeatTime = 0;
    digitalWrite(BUZZER, LOW);
    alarmSilenced = false;
  }

  // --------------------------------
  // PUSH BUTTON (debounced, non-blocking)
  // --------------------------------

  if (digitalRead(PUSH_BUTTON) == LOW && (now - lastButtonPress > BUTTON_DEBOUNCE))
  {
    lastButtonPress = now;
    alarmSilenced = true;
    digitalWrite(BUZZER, LOW);
  }

  // --------------------------------
  // DISPLAY EVERY 1 SECOND
  // --------------------------------

  if (now - lastDisplayTime >= 1000)
  {
    lastDisplayTime = now;

    Serial.println();
    Serial.println("============================");

    Serial.print("Sensor Value : ");
    Serial.println(sensorValue);

    Serial.print("Heart Rate   : ");

    if (bpm > 0)
    {
      Serial.print(bpm);
      Serial.println(" BPM");
    }
    else
    {
      Serial.println("Calculating...");
    }

    // --------------------------------
    // CRITICAL - LOW
    // --------------------------------

    if (bpm > 0 && bpm < 60)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, HIGH);

      Serial.println("STATUS       : CRITICAL");
      Serial.println("Reason       : LOW HEART RATE");

      if (alarmSilenced == false)
      {
        digitalWrite(BUZZER, HIGH);
      }
    }

    // --------------------------------
    // NORMAL
    // --------------------------------

    else if (bpm >= 60 && bpm <= 100)
    {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, LOW);

      digitalWrite(BUZZER, LOW);

      alarmSilenced = false;

      Serial.println("STATUS       : NORMAL");
    }

    // --------------------------------
    // WARNING
    // --------------------------------

    else if (bpm > 100 && bpm <= 120)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RED_LED, LOW);

      digitalWrite(BUZZER, LOW);

      Serial.println("STATUS       : WARNING");
      Serial.println("Reason       : HIGH HEART RATE");
    }

    // --------------------------------
    // CRITICAL - HIGH
    // --------------------------------

    else if (bpm > 120)
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, HIGH);

      Serial.println("STATUS       : CRITICAL");
      Serial.println("Reason       : VERY HIGH HEART RATE");

      if (alarmSilenced == false)
      {
        digitalWrite(BUZZER, HIGH);
      }
    }

    // --------------------------------
    // NO BPM
    // --------------------------------

    else
    {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER, LOW);

      Serial.println("STATUS       : WAITING");
      Serial.println("Place finger properly");
    }

    if (alarmSilenced)
    {
      Serial.println("Alarm        : SILENCED");
    }

    Serial.println("============================");
  }

  // Fast sensor sampling
  delay(10);
}