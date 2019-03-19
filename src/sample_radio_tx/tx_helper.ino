
//write the corresponding hex pattern to the pins
void h_write(int hex)
{
  int pin = 0;
  for(int i = 1; i <= 8; i *= 2){
    if (i & hex) {
      b_write(PINS[pin],LOW);
    }
    else{
      b_write(PINS[pin],HIGH);
    }
    pin++;
  }
}

// Writes binary data to provided pin
void b_write(int pin, bool data)
{
  //log("Binary Write (PIN " + String(pin) + ")");
  digitalWrite(pin, data);
}



// Batch Write data
void groupWrite(bool data)
{
  log("Group Write (" + String(data) + ")");
  
  for(int i = 0; i < PIN_C; i++)
  {
    digitalWrite(PINS[i], data);
  }
}
