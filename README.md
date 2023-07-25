# The-Way-Of-The-Galaxy
A space shooter game written in C++ with the SDL2 library

<img src="https://user-images.githubusercontent.com/93731405/219963991-8e1f9497-7bc9-432d-868e-6412d31585c8.png" width=100%/>

The Way Of The Galaxy is an open source game, inspired to the 16bit games of the 80s 90s, but especially to [Konami Gradius (1985)](https://it.wikipedia.org/wiki/Gradius). Such game promises to be a graphical adventure, (not an arcade as Gradius) and at the same time, a space shooter that exploit hard the ability of the player, beyond that to have a defined history and to be organized in levels dipendent of each order.

## Technical terms
The project promises to be multiplatform and to make the most of the potential of the [SDL2](https://www.libsdl.org/) graphics library, the principal library used in the project, the project is hybrid between a Cmake project and a Visual Studio project beyond that to exploit the libraries [nlohmann-json](https://github.com/nlohmann/json) and [pugixml](https://pugixml.org/) for the reading of the xml and json languages. The application exploit the architecture model, Entity Component System and benefits to other classes to manage too elements of the ECS. Inside the project is present a subfolder called simply "include", where are present all of various headers, and it it the "ECS" subfolder where are present all created components headers.

## Compile

### Windows

There are two ways to compile the project, if you are on Windows, you can choose to exploit the Visual Studio project file, or the CmakeLists.txt. **IMPORTANT! if you want to use Visual Studio, remember to install [vcpkg](https://vcpkg.io/en/index.html) and the dependencies of the project:**

```
vcpkg install sdl2 sdl2-image sdl2-ttf pugixml nlohmann-json boost-filesystem --triplet x64-windows
```

or if you want to compile for x86:

```
vcpkg install sdl2 sdl2-image sdl2-ttf pugixml nlohmann-json boost-filesystem --triplet x86-windows
```

If you want to use cmake, you can install the dependencies with vcpkg, or you can install the libraries locally and indicate the position of every package using the cmake variable that tou can specify adding the cmake args to indicate they, for exemple: `-DSDL2_DIR=`, `-DSDL2_image_DIR=`, `-DSDL2_ttf_DIR=`, `-Dnlohmann_json_DIR=`, `-Dboost_DIR` and `-Dpugixml_DIR=`.

### Linux

If you are on Linux, the only way to compile, is with cmake and unfortunately, **the CmakeLists.txt require a too recent version of SDL2 (2.26.0)** to find and use the necessary packages, and you need to install the development libraries with the package manager of the distro or compiling they from source:

**Ubuntu:** 
```
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev nlohmann-json3 pugixml libboost-filesystem-dev
```
**Arch Linux:**
```
sudo pacman -S sdl2 sdl2_image sdl2_ttf nlohmann-json pugixml boost
```
**Fedora:**
```
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel json-devel pugixml boost
```
If you want to compile for x86, you need to install the equivalent packages for x86 and you must add the cmake arg `-DCMAKE_CXX_FLAGS="-m32"`

## Run the builds

In the Windows builds (x86 and x64 Release Visual Studio) all the dynamics libraries (.dll) are in the same folder of the executable, there should be no runtime and dependency problems, if the windows version is recent enough. In the Linux builds (x86 and x64 Release Cmake), you need to install dynamic libraries as dependencies or their equivalent for x86:

**Ubuntu 22.04:**
```
sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0 libboost-filesystem1.74.0
```
**Arch Linux:**
```
sudo pacman -S sdl2 sdl2_image sdl2_ttf boost-libs
```
**Fedora:**
```
sudo dnf install SDL2 SDL2_image SDL2_ttf boost
```

## Screenshots

### Lastest Release:
![Screenshot_20230128_192929](https://user-images.githubusercontent.com/93731405/216776778-069b30cb-1283-47d1-8b04-67d20c08f8ff.png)
![Screenshot 2023-06-28 140143](https://github.com/it2000-the-original/The-Way-Of-The-Galaxy/assets/93731405/eb566dc6-03b7-4054-9934-d4ff50cfcac6)
