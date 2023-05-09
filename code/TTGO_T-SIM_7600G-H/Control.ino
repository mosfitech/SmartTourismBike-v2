
void Control_Set() {
  pinMode(SW_UP, INPUT_PULLUP);
  pinMode(Dinamo, OUTPUT);  
  pinMode(Buzzer, OUTPUT);
  
  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(300);
  digitalWrite(MODEM_PWRKEY, LOW);
  
  pinMode(MODEM_FLIGHT, OUTPUT);
  digitalWrite(MODEM_FLIGHT, HIGH);
  
  SW_UPState = digitalRead(SW_UP); 
  
  //void Control_lock()
  //{
  //    digitalWrite(Dinamo, HIGH);
  //    digitalWrite(Buzzer, HIGH);
  //    delay(2053);
  //    digitalWrite(Dinamo, LOW);
  //    digitalWrite(Buzzer, LOW);    
  //  }
}

void Buzzer_Blink(){
   for(int i = 0; i < 3; i++) { // Loop sebanyak blinkCount
    digitalWrite(Buzzer, HIGH); // LED menyala
    delay(500); // Tahan LED selama 500ms (0,5 detik)
    digitalWrite(Buzzer, LOW); // LED mati
    delay(500); // Tahan LED selama 500ms (0,5 detik)
   }
  }
  
