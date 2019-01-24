# BlueFairy ![bluefairy logo](/assets/logo.svg)

Read this in [portuguese](README.pt_BR.md) or [english](README.md).

BlueFairy is a library to make arduino easier. At the beginning was built to make simple work with intervals, but now, has some more utilities.

### First Steps

Include the **bluefairy** library:
```c++
#include <bluefairy.h>
```


### Scheduler

The main idea of `Scheduler` is avoid the use of the funcion `delay` in your code.
At that way your code can run **almost** as a multitask program.


Imagine that you want to blink a LED with an interval of 1 second like that:

```c++
const unsigned char LED_PIN = 13;

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    delay(500);
    digitalWrite(LED_PIN, 0);
    delay(500);
    digitalWrite(LED_PIN, 1);
}
```

Now you want to add a button that when is pressed turn off the blinker, I think that most trivial way to do this is like that:


```c++
const unsigned char LED_PIN = 13;
const unsigned char BUTTON_PIN = 1;

unsigned char blinkerOn = 1;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
    blinkerOn = !digitalRead(BUTTON_PIN);
    delay(500);
    digitalWrite(LED_PIN, 0 & blinkerOn);
    delay(500);
    digitalWrite(LED_PIN, 1 & blinkerOn);
}
```
When the `delay` function is called the program stops, and do nothing until the defined time over. So, during the delay the `blinkerOn` will not be updated and your program could have a delay to response the button press.

So let's rewrite that using the scheduler.

```c++
#include <bluefairy.h>

bluefairy::Scheduler scheduler;

const unsigned char LED_PIN = 13;
const unsigned char BUTTON_PIN = 1;

unsigned char blinkerOn = 1;
unsigned char ledValue = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
    scheduler.every(0, 1000, [](){
        ledValue = 0;
        digitalWrite(LED_PIN, ledValue & blinkerOn);
    });
    scheduler.every(500, 1000, [](){
        ledValue = 1;
        digitalWrite(LED_PIN, ledValue & blinkerOn);
    });
    scheduler.every(50, [](){
        blinkerOn = !digitalRead(BUTTON_PIN);
        digitalWrite(LED_PIN, ledValue & blinkerOn);
    });
}

void loop() {
    scheduler.loop();
}
```

Now the `blinkerOn` will be updated every 50 milliseconds and the LED will blink with the same interval, just like the last example.

The [blink example](/examples/Blink/Blink.ino) use the scheduler to blink fast five times and after 3 seconds blink slower.

### Keyboard

The keyboard abstract things to treat inputs, they provide a way to define callbacks functions for events fired by the inputs.
Basically the keyboard verify each input periodically and when a change is detected your callback function is called.

The [keyboard example](/examples/Keyboard/Keyboard.ino) listen some inputs and just send, through the serial port, info about each event.

### StateMachine

The state machine is just a way to help at the organization of the code, they provide a pattern to define what to do when enter or leave a state. At the [example](/examples/State/State.ino) there a state that a LED blink fast and a state that the LED blink slowly and some buttons define when change the state.

### Namespace

The `bluefairy` namespace is just a alias for `ciag::bluefairy` namespace, you can choice what you prefer.

Some examples:

```c++
ciag::bluefairy::Scheduler scheduler;
bluefairy::Scheduler scheduler;
```

```c++
using namespace bluefairy;
using namespace ciag::bluefairy;

Scheduler scheduler;
```

```c++
using bluefairy::Scheduler;

Scheduler scheduler;
```