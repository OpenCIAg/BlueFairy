# BlueFairy

Read this in [portuguese](README.pt_BR.md) or [english](README.md).

BlueFairy is a library to make arduino easier. At the beginning was builded to make simple work with intervals, but now, has some more utilities.

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

void setup() {
    pinMode(LED_PIN, OUTPUT);
    scheduler.every(0, 1000, [](){
        digitalWrite(LED_PIN, 0 & blinkerOn);
    });
    scheduler.every(500, 1000, [](){
        digitalWrite(LED_PIN, 1 & blinkerOn);
    });
    scheduler.every(50, [](){
        blinkerOn = !digitalRead(BUTTON_PIN);
    });
}

void loop() {
    scheduler.loop();
}
```

Now the `blinkerOn` will be updated every 50 milliseconds and the LED will blink with the same interval, just like the last example.

The [blink example](/examples/Blink/Blink.ino) use the scheduler to blink fast five times and after 3 seconds blink slower.

### Keyboard
[example](/examples/Keyboard/Keyboard.ino)
### StateMachine
[example](/examples/State/State.ino)
### Namespace

If you don't know what are c++ namespaces you really need to read that:

We use namespaces, so, you can always write thta as a type prefix or "import" the namespace with `using`.
The `bluefairy` namespace is just a alias for `ciag::bluefairy` namespace, you can choice what you prefer.

Using like a type prefix:

```c++
ciag::bluefairy::Scheduler scheduler;
bluefairy::Scheduler scheduler;
```

"Import" with `using`:

```c++
using bluefairy;
using ciag::bluefairy;

Scheduler scheduler;
```