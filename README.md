# CREN

![License](https://img.shields.io/github/license/UberDever/CREN)

# Description
CREN should have been become lightweight framework, that must have been oriented for 2.5D raycaster-based games.
Instead, it has become one of the experiments, that helped me understand software development principles.
It also has become field of UI experiments.
Thus, CREN is pure experimental project and it here mostly for experience.

# Featuers
## Structure
Main feature is project structure - it mainly consists of namespaces and struct, that treated like classes. I.e:
```c++
//gut - contains all graphic related utilities
namespace gut
{
    using namespace util;
    using namespace res;
    using namespace math;
    using namespace cfg;
    using namespace UI;

    e_exitCodes gut_init();
    void gut_clean();

    void gut_raycast();
    void gut_UI();

    void gut_updWin(int w, int h, uint32_t flags);
}
```
```using``` here for convenience. Thus, that approach is program that purely consists of global variables.
It sound wierd by now, but it was interesting experience.

### Conclusion
This system has some interesting advantages, such as: total dependancy separation by header includes,
handy global state and etc. But, it just boils down to global state machine with no clear logic isolation.
Therefore, this architecture is mostly redundant: if you want make a game with global state - do it straightforward.

## UI

![UIScreenshot](https://github.com/UberDever/CREN/blob/master/ui_example.png)

CREN was mainly developed in UI field. UI features:
1. Loading from XML file
2. Relational positioning and parenting
3. Normalized size of elements
4. Generic UI elements, that assembled from:
    * Frame
    * Text
    * Picture
5. Elements can be static and dynamic, latter group is being iterated while events are being processed.
6. Additional types of elements are described as XML tags: button, list, slider. Although, slider didn't implemented.
7. Groups of elements are divided into UI Scenes, and there can be quite lot of them

### Conclusion
At some point, I realised, that there can be another way of UI styling - just like CSS with HTML do!
Even though it must not be that complicated, it is right choice for this kind of UI implementation.
So, this main conclusion about UI XML structure - simply divide it in .style and .ui modules.


This project uses event-driven paradigm for handling dynamic UI elements.
About UI type: even at that moment I cannot decide which UI type is more suitable - Realtime widget processing or
Event-driven paradigm. Former is great for videogames by their nature, but it simply might be overkill. Latter is 
great for event-driven kind of communication, where UI elements are separate entities and work in other layer (perhaps in other thread) and
it also much better for UI based games (Party RPG's, Simulators...)


Also, there are quite many microfeatures, that I don't remember, but they are so important, so UI of this kind is unique.
Therefore, many of actually good engineering solutions are faded out.

## Items

![ItemScreenshot](https://github.com/UberDever/CREN/blob/master/item_example.png)

There are also fully working Diablo-like item generation function, that generates an Item object for a given list of parameters (itemLevel and possible type)
Quite handy routine, but it never has been built into something reasonable.

### Conclusion

Of course, for now it just pile of code, that does something with some extent, but it nowhere close to the actual item generation system.
Therefore, this generator is exposed as little concept of procedural generation with it's specials.