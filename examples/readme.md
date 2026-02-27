# SFML Examples

## Running the examples

When SFML is built from source with CMake, the example executables are generated in the `build/bin` directory.

Most examples rely on external resources (textures, fonts, sounds) that are loaded using relative paths.
Because of this, the programs must be run from their respective example directories; otherwise the resources cannot be located.

For example (on Linux/macOS):

```bash
cd examples/tennis
../../build/bin/tennis
```

For example (on Windows / Visual Studio):

```cmd
cd examples\tennis
..\..\build\bin\Debug\tennis.exe
```

Running an example directly from `build/bin` may result in errors such as:

```
Failed to open sound file
Failed to open sound buffer from file
```
