/* Buttons to USB MIDI Example

   You must select MIDI from the "Tools > USB Type" menu

   To view the raw MIDI data on Linux: aseqdump -p "Teensy MIDI"

   This example code is in the public domain.
*/

#include <Bounce.h>

// the MIDI channel number to send messages
const int channel = 1;

#define DEBOUNCE_MS    50
#define SW_IN_1        5
#define SW_IN_2        6
#define SW_LED_1       12
#define SW_LED_2       10
#define HEARTBEAT_LED  11
// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button1 = Bounce(SW_IN_1, DEBOUNCE_MS);  
Bounce button2 = Bounce(SW_IN_2, DEBOUNCE_MS);  

void setup() {
  pinMode(SW_IN_1, INPUT_PULLUP);
  pinMode(SW_IN_2, INPUT_PULLUP);
  pinMode(SW_LED_1, OUTPUT);
  pinMode(SW_LED_2, OUTPUT);
  pinMode(HEARTBEAT_LED, OUTPUT); 
}

int c = 0;
void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button1.update();
  button2.update();

  // Check each button for "falling" edge.
  // Send a MIDI Note On message when each button presses
  // Update the Joystick buttons only upon changes.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button1.fallingEdge()) {
    digitalWrite(SW_LED_1, HIGH);
    usbMIDI.sendNoteOn(60, 127, channel);
  }
  if (button2.fallingEdge()) {
    digitalWrite(SW_LED_2, HIGH);
    usbMIDI.sendNoteOn(61, 127, channel);  // 62 = D4
  }

  if (button1.risingEdge()) {
    digitalWrite(SW_LED_1, LOW);
    usbMIDI.sendNoteOff(60, 0, channel);  // 61 = C#4
  }
  if (button2.risingEdge()) {
    digitalWrite(SW_LED_2, LOW);
    usbMIDI.sendNoteOff(61, 0, channel);  // 62 = D4
  }

  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
  ++c;
  digitalWrite(HEARTBEAT_LED,!!(c&0x1000));
}

