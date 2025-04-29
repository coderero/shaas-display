#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <Arduino.h>
#include "SerialTransfer.h"

// Relay Types
enum RelayType
{
    LOW_DUTY = 1,  // 10A, 4 switches
    HEAVY_DUTY = 2 // 30A, 2 switches
};

// Commands
enum CommandType
{
    TOGGLE_RELAY = 1,
    GET_RELAY_STATE = 2
};

// Command structure (sent from controller to Arduino)
struct __attribute__((packed)) RelayCommand
{
    uint8_t command; // Command type (TOGGLE_RELAY, GET_RELAY_STATE)
    uint8_t type;    // Relay type (LOW_DUTY, HEAVY_DUTY)
    uint8_t port;    // Port number (starting from 1)
    uint8_t state;   // Desired state (0 = OFF, 1 = ON)
};

// Response structure (sent from Arduino to controller)
struct __attribute__((packed)) RelayResponse
{
    uint8_t status; // 0 = success, 1 = error
    uint8_t type;   // Relay type that was acted upon
    uint8_t port;   // Port that was acted upon
    uint8_t state;  // Current state of the relay (0 = OFF, 1 = ON)
};

class RelayModule
{
private:
    // Pin definitions for relays
    // LOW_DUTY relay pins (10A, 4 switches)
    static const uint8_t LOW_DUTY_PINS[4];

    // HEAVY_DUTY relay pins (30A, 2 switches)
    static const uint8_t HEAVY_DUTY_PINS[2];

    // SerialTransfer instance
    SerialTransfer transferHandler;

    // Setup relay pins
    void setupRelayPins();

    // Test relays on startup
    void testRelays();

    // Validate relay parameters
    bool isValidRelay(RelayType type, uint8_t port);

public:
    RelayModule();

    // Initialize the module
    void begin();

    // Process incoming serial commands
    void handleSerialCommands();

    // Toggle a relay
    bool toggleRelay(RelayType type, uint8_t port, bool state);

    // Get current relay state
    bool getRelayState(RelayType type, uint8_t port);
};

#endif // RELAY_CONTROL_H