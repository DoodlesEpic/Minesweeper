# Minesweeper

A simple, native, and cross-platform Minesweeper game made with wxWidgets.

![Screenshot from 2023-03-13 20-26-55](https://user-images.githubusercontent.com/37254797/224854238-36982506-32e7-4a5d-8499-177af2a096a7.png)

## Installation

Grab the executable appropriate for your system in the [GitHub Releases page](https://github.com/DoodlesEpic/Minesweeper/releases/) and start playing, no installation needed.

## Development

This is application is developed using C++, the [CMake build system](https://cmake.org/), and the [wxWidgets UI library](https://wxwidgets.org/). You must set up each one of those dependencies in your system, installation will differ for each operating system, so follow the guide on each website. The recommended compiler to use under Windows is [MSVC](https://learn.microsoft.com/en-us/cpp/), which can be [installed through Visual Studio](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation). Otherwise, you may also use Clang under [MSYS2](https://www.msys2.org/).

If you're using an Ubuntu-based distribution, you may install the needed dependencies using the following command:

```sh
sudo apt install build-essential git cmake libgtk-3-dev
```

To start development, clone the repository and run in the root folder:

```sh
git submodule update --init --recursive --progress
cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug
```

Compile wxWidgets and the application using:

```sh
cmake --build build -j8
```

You may omit -j8 if you are using Ninja or MSVC as a backend, since those will already use all available threads by default.

The final executable will be available under the build folder with the name Minesweeper and the appropriate file extension depending on your system.

## License

This project is licensed under the GNU General Public License v3.0. Check the [LICENSE](LICENSE) file for more information.
