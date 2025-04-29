#include <Arduino.h>
#include "relay_control.h"

// Create relay module instance
RelayModule relayController;

void setup()
{
  // Initialize relay module
  relayController.begin();

  Serial.println(F("SerialTransfer Relay Control System"));
  Serial.println(F("-----------------------------------"));
  Serial.println(F("Low Duty (10A): 4 switches on pins 2-5"));
  Serial.println(F("Heavy Duty (30A): 2 switches on pins 6-7"));
  Serial.println(F("Waiting for commands..."));
}

void loop()
{
  // Handle any incoming serial commands
  relayController.handleSerialCommands();
}