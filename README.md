# BlueFairy ![bluefairy logo](/assets/logo.svg)

Read this in [portuguese](README.pt_BR.md) or [english](README.md).

BlueFairy is a library to make arduino easier. It started as a library to simplify work with intervals, but now it has some more utilities.

### First Steps

Include the **bluefairy** library:
```c++
#include <bluefairy.h>
```


### Scheduler

The main idea of `Scheduler` is to avoid the use of the  `delay` funcion in your code.
This way your code can run **almost** as a multitask program.


Imagine that you want to blink a LED with an interval of 1 second:

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

Now you want to add a button that when pressed turns off the blinker. The most trivial way to do this is:


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

When the `delay` function is called, the program stops and do nothing until the defined time over. So, during the delay, the `blinkerOn` will not be updated and your program could have a delay to respond the button press.

So let's rewrite that using `Scheduler`:

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

Now `blinkerOn` will be updated every 50 milliseconds and the LED will blink with the same interval, just like the last example.

The [blink example](/examples/Blink/Blink.ino) uses the scheduler to blink fast five times and after 3 seconds blink slower.

### Keyboard

The keyboard abstract things to treat inputs. They provide a way to define callbacks functions for events fired by the inputs.
Basically the keyboard verify each input periodically and when a change is detected your callback function is called.

The [keyboard example](/examples/Keyboard/Keyboard.ino) listen some inputs and just send info about each event through the serial port.

### StateMachine

The state machine is just a way to help with the organization of the code. They provide a pattern to define what to do when entering or leaving a state. In the [example](/examples/State/State.ino) there is a state where a LED blink fast, a state where the LED blink slowly and some buttons define when the state is changed.

### Namespace

The `bluefairy` namespace is just an alias for `ciag::bluefairy` namespace, you can choose what you prefer.

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
using ciag::bluefairy::Scheduler;

Scheduler scheduler;
```