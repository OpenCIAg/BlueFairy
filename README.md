# BlueFairy
## A tool kit to make arduino easier

### First Steps


```c++
#include <bluefairy.h>
```


### Scheduler

### Keyboard

### StateMachine

### Namespace

If you don't know what are c++ namespaces you really need to read that:

We use namespaces, so, you can always write as a prefix of the types or "import" the namespace.

```c++
ciag::bluefairy::Scheduler scheduler;
bluefairy::Scheduler scheduler;
```

The `bluefairy` namespace is just a alias for `ciag::bluefairy` namespace

```c++
using bluefairy;
using ciag::bluefairy;

Scheduler scheduler;
```