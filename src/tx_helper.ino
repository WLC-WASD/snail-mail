
// Writes binary data to provided pin
void b_write(int pin, bool data)
{
  log("Binary Write (PIN " + String(pin) + ")");
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
