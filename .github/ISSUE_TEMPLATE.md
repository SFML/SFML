Thanks for raising your issue here! 🙂

Before you submit the issue however, we'd like you to consider the follow points.

* We like to use the issue tracker for confirmed issues.
* If you're stuck with SFML, please use [the forum](https://en.sfml-dev.org/forums/index.php#c3) to get help.

----

## Subject of the issue

Describe your issue here.

## Your environment

* Your OS / distro / window manager used
* Your version of SFML (2.5.0, git master, etc)
* Your compiler and compiler version used
* Special compiler flags used

## Steps to reproduce

Tell us how to reproduce this issue. Please provide a [minimal, complete and verifiable example](https://stackoverflow.com/help/mcve), you can use the follow template as a start:

```cpp
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Minimal, complete and verifiable example");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        for (sf::Event event; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }
}
```

## Expected behavior

Tell us what should happen

## Actual behavior

Tell us what happens instead
