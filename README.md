# Minesweeper

A simple, native, and cross platform Minesweeper game made with wxWidgets.

## Installation

Grab the executable appropriate to your system in the [GitHub Releases page](https://github.com/DoodlesEpic/Minesweeper/releases/tag/) and start playing, no installation needed.

## Development

This is application is developed using C++, the [Meson Build system](https://mesonbuild.com/), and [wxWidgets UI library](https://wxwidgets.org/). You must set up each one of those dependencies in your system, installation will differ for each operation system so follow the guide on each website. The recommened compiler to use under Windows in Clang under [MSYS2](https://www.msys2.org/).

To start development clone the repository and run in the root folder:

```sh
meson setup build -Dbuildtype=debugoptimized
```

Then compile the application using:

```sh
meson compile -C build
```

The final executable will be available under the build folder with the name Minesweeper and the appropriate file extension depending on your system.

## License

This project is licensed under the GNU General Public License v3.0. Check the [LICENSE](LICENSE) file for more information.
