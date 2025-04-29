#include "relay_control.h"

// Define the pin constants
const uint8_t RelayModule::LOW_DUTY_PINS[4] = {2, 3, 4, 5}; // Pins for 10A relays
const uint8_t RelayModule::HEAVY_DUTY_PINS[2] = {6, 7};     // Pins for 30A relays

RelayModule::RelayModule()
{
    // Constructor
}

void RelayModule::begin()
{
    Serial.begin(115200);
    transferHandler.begin(Serial);

    setupRelayPins();
    testRelays();

    // Clear any initial pending data
    while (Serial.available())
    {
        Serial.read();
    }

    Serial.println(F("Relay Module Initialized"));
}

void RelayModule::setupRelayPins()
{
    // Set all relay pins as outputs
    for (int i = 0; i < 4; i++)
    {
        pinMode(LOW_DUTY_PINS[i], OUTPUT);
        digitalWrite(LOW_DUTY_PINS[i], HIGH); // Relays are active LOW
    }

    for (int i = 0; i < 2; i++)
    {
        pinMode(HEAVY_DUTY_PINS[i], OUTPUT);
        digitalWrite(HEAVY_DUTY_PINS[i], HIGH); // Relays are active LOW
    }
}

void RelayModule::testRelays()
{
    // Test all relays by cycling through them
    Serial.println(F("Testing relays..."));

    // Test LOW_DUTY relays
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LOW_DUTY_PINS[i], LOW); // Turn ON
        delay(200);
        digitalWrite(LOW_DUTY_PINS[i], HIGH); // Turn OFF
        delay(100);
    }

    // Test HEAVY_DUTY relays
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(HEAVY_DUTY_PINS[i], LOW); // Turn ON
        delay(200);
        digitalWrite(HEAVY_DUTY_PINS[i], HIGH); // Turn OFF
        delay(100);
    }

    Serial.println(F("Relay test complete"));
}

bool RelayModule::isValidRelay(RelayType type, uint8_t port)
{
    // Validate that the relay type and port are valid
    // Port numbers start from 1 (user-friendly), but array indices from 0

    if (port < 1)
    {
        return false;
    }

    switch (type)
    {
    case LOW_DUTY:
        return port <= 4; // 4 LOW_DUTY relays

    case HEAVY_DUTY:
        return port <= 2; // 2 HEAVY_DUTY relays

    default:
        return false;
    }
}

bool RelayModule::toggleRelay(RelayType type, uint8_t port, bool state)
{
    // Toggle a relay to the desired state
    // Returns true if successful, false if invalid parameters

    if (!isValidRelay(type, port))
    {
        return false;
    }

    uint8_t pinIndex = port - 1;           // Convert to 0-based index
    uint8_t pinValue = state ? LOW : HIGH; // Relays are active LOW

    switch (type)
    {
    case LOW_DUTY:
        digitalWrite(LOW_DUTY_PINS[pinIndex], pinValue);
        Serial.print(F("LOW_DUTY relay "));
        break;

    case HEAVY_DUTY:
        digitalWrite(HEAVY_DUTY_PINS[pinIndex], pinValue);
        Serial.print(F("HEAVY_DUTY relay "));
        break;

    default:
        return false;
    }

    Serial.print(port);
    Serial.print(F(" set to "));
    Serial.println(state ? F("ON") : F("OFF"));

    return true;
}

bool RelayModule::getRelayState(RelayType type, uint8_t port)
{
    // Get the current state of a relay
    // Returns current state (true = ON, false = OFF)
    // Returns false if invalid parameters

    if (!isValidRelay(type, port))
    {
        return false;
    }

    uint8_t pinIndex = port - 1; // Convert to 0-based index
    bool currentState;

    switch (type)
    {
    case LOW_DUTY:
        currentState = (digitalRead(LOW_DUTY_PINS[pinIndex]) == LOW);
        break;

    case HEAVY_DUTY:
        currentState = (digitalRead(HEAVY_DUTY_PINS[pinIndex]) == LOW);
        break;

    default:
        return false;
    }

    return currentState;
}

void RelayModule::handleSerialCommands()
{
    // Check if data is available
    if (transferHandler.available())
    {
        // Create command and response structures
        RelayCommand cmd;
        RelayResponse resp;

        // Get the data
        uint16_t recSize = 0;
        recSize = transferHandler.rxObj(cmd, recSize);

        // Initialize response with error status by default
        resp.status = 1; // Error
        resp.type = cmd.type;
        resp.port = cmd.port;
        resp.state = 0;

        // Process valid commands
        if (recSize == sizeof(RelayCommand))
        {
            Serial.println(F("Valid command received"));

            // Handle the command based on type
            switch (cmd.command)
            {
            case TOGGLE_RELAY:
                if (isValidRelay((RelayType)cmd.type, cmd.port))
                {
                    // Toggle the relay
                    bool success = toggleRelay((RelayType)cmd.type, cmd.port, cmd.state);
                    resp.status = success ? 0 : 1;
                    resp.state = getRelayState((RelayType)cmd.type, cmd.port);
                }
                else
                {
                    Serial.println(F("Invalid relay parameters"));
                }
                break;

            case GET_RELAY_STATE:
                if (isValidRelay((RelayType)cmd.type, cmd.port))
                {
                    resp.status = 0; // Success
                    resp.state = getRelayState((RelayType)cmd.type, cmd.port);

                    Serial.print(F("Relay state: "));
                    Serial.print(F("Type "));
                    Serial.print(cmd.type);
                    Serial.print(F(", Port "));
                    Serial.print(cmd.port);
                    Serial.print(F(" = "));
                    Serial.println(resp.state ? F("ON") : F("OFF"));
                }
                else
                {
                    Serial.println(F("Invalid relay parameters"));
                }
                break;

            default:
                // Unknown command
                Serial.println(F("Unknown command"));
                resp.status = 1;
                break;
            }
        }
        else
        {
            Serial.println(F("Invalid packet size"));
        }

        // Send the response
        transferHandler.txObj(resp, sizeof(RelayResponse));
        transferHandler.sendData(sizeof(RelayResponse));

        // Add small delay to ensure packet completion
        delay(5);
    }
}