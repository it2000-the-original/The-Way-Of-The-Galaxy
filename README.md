
# The-Way-Of-The-Galaxy
A space shooter game written in C++ with the SDL2 library

The Way Of The Galaxy is an open source game inspired to the 16bit games of the 80s 90s, but especially to [Konami Gradius (1985)](https://it.wikipedia.org/wiki/Gradius). Such game promises to be a graphical adventure, (not an arcade as Gradius) and at the same time, an action game that exploit hard the ability of the player, beyond that to have a defined history and to be organized in levels indipendent of each order.

## Technical terms
The project promises to be multiplatform end to make the most of the potential of the [SDL2](https://www.libsdl.org/) graphics library, the principal library used in the project, the project is hybrid between a Cmake project and a Visual Studio project beyond that to exploit the libraries [nlohmann-json](https://github.com/nlohmann/json) and [pugixml](https://pugixml.org/) for the reading of the xml and json languages. The application exploit the architecture model Entity Component System and benefits to other classes to manage too elements of the ECS, inside the project is present a subfolder called simply include, where are present all of various headers, and a subfolder ECS where are present all created components.

## Compile

### Windows

There are two ways to compile the project, if you are on Windows, you can choose to exploit the project file Visual Studio or the CmakeLists.txt. **IMPORTANT! if you want to use Visual Studio remember to install [vcpkg](https://vcpkg.io/en/index.html) and the dependencies of the project:**

```
vcpkg install sdl2 sdl2-image sdl2-ttf pugixml nlohmann-json --triplet x64-windows
```

or if you want to compile for x86:

```
vcpkg install sdl2 sdl2-image sdl2-ttf pugixml nlohmann-json --triplet x86-windows
```

If you want to use cmake you can install the dependencies with vcpkg, or you can install the libraries locally and indicate the position of every package adding the cmake args `-DSDL2_DIR=`, `-DSDL2_image_DIR=`, `-DSDL2_ttf_DIR=`, `-Dnlohmann_json_DIR=` and `-Dpugixml_DIR=`.

### Linux

If you are and Linux, the only way to compile id with cmake and unfortunately, **the CmakeLists.txt require a too recent version of SDL2 (2.26.0)** to find and use the necessary packages and you need to install the development libraries with the package manager of the distro or compiling from source:

Ubuntu: 
```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev nlohmann-json3 pugixml
```

Arch Linux:
```
sudo pacman -S sdl2 sdl2_image sdl2_ttf nlohmann-json pugixml
```

Fedora:
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel json-devel pugixml
```

## Screenshots
![Screenshot_20221029_210831](https://user-images.githubusercontent.com/93731405/202857538-fb845781-a9b2-4435-b362-d54646596b17.png)
