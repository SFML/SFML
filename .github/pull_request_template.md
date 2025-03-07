<!--
Thanks a lot for making a contribution to SFML! 🙂

Please make sure you are targetting the correct branch. No more features are planned for the 2.x branches! (See [the readme](https://github.com/SFML/SFML#state-of-development))

Before creating the pull request, we ask you to check the following boxes: (For small changes not everything needs to ticked, but the more the better!)

-   [ ] Has this change been discussed on [the forum](https://en.sfml-dev.org/forums/index.php#c3) or in an issue before?
-   [ ] Does the code follow the SFML [Code Style Guide](https://www.sfml-dev.org/style.php)?
-   [ ] Have you provided some example/test code for your changes?
-   [ ] If you have additional steps which need to be performed, please list them as tasks!
-->

## Description

<!-- Please describe your pull request. -->

This PR is related to the issue #

## Tasks

-   [ ] Tested on Linux
-   [ ] Tested on Windows
-   [ ] Tested on macOS
-   [ ] Tested on iOS
-   [ ] Tested on Android

## How to test this PR?

<!-- Describe how to best test these changes. -->

<!-- Please provide a [minimal, complete and verifiable example](https://stackoverflow.com/help/mcve) if possible, you can use the following template as a start: -->

```cpp
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Minimal, complete and verifiable example");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }
}
```
