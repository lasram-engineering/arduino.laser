#ifndef CONFIG_H
#define CONFIG_H

#define INPUT_MODE INPUT_PULLUP

#define SERIAL_ENABLED

#pragma region DEFINITIONS

#if INPUT_MODE == INPUT_PULLUP
// if the pin mode is set to input pullup, the logical values are inverted
#define LOGICAL_HIGH LOW
#define LOGICAL_LOW HIGH
#elif INPUT_MODE == INPUT
#define LOGICAL_HIGH HIGH
#define LOGICAL_LOW LOW
#endif

#pragma endregion

#endif