
# Telemetry plugin for Euro Truck Simulator 2 and American Truck Simulator

This is a fork of RenCloud's [scs-sdk-plugin](https://github.com/RenCloud/scs-sdk-plugin/) project. It adds macOS and Linux support by translating Windows API calls for memory-mapped files to their POSIX equivalents:

| Windows            | macOS / Linux |
| ------------------ | ------------- |
| `OpenFileMapping`  | `shm_open`    |
| `MapViewOfFile`    | `mmap`        |
| `UnmapViewOfFile`  | `munmap`      |
| `CloseHandle`      | `shm_unlink`  |

- On Windows, the name of the file-mapping object is `Local\SCSTelemetry`
- On macOS and Linux, the name of the shared memory object is `/SCSTelemetry`

The layout of the memory-mapped files is the same across all three OSs[^1], so it's possible to write cross-platform code against this plugin with relatively little effort. See:
- [`scs-telemetry-common.hpp`](https://github.com/truckermudgeon/scs-sdk-plugin/blob/master/scs-telemetry/inc/scs-telemetry-common.hpp) for the layout of the memory-mapped files
- [TruckSim-Telemetry](https://github.com/kniffen/TruckSim-Telemetry) for an example of a cross-platform node module that parses the memory-mapped files into JSON

[^1]: assuming we're targeting x64 architectures

## Installation

Download the latest [release](https://github.com/truckermudgeon/scs-sdk-plugin/releases) for your OS, unzip the plugin, then copy the plugin to the `plugins` folder located somewhere inside of your ETS2/ATS game install folder (e.g., `/Users/truckermudgeon/Library/Application Support/Steam/steamapps/common/American Truck Simulator` on macOS, or `C:\Program Files (x86)\Steam\steamapps\common\American Truck Simulator` on Windows). If you don't have a `plugins` folder, you'll need to create one:

| Operating System | `plugins` path |
| ---------------- | -------------- |
| macOS            | `<game install folder>/American Truck Simulator.app/Contents/MacOS/plugins` <br><br>(yes, it's _inside_ of the app package) |
| Linux            | `<game install folder>/bin/linux_x64/plugins` |
| Windows          | `<game install folder>/bin/win_x64/plugins`   |

## Building the code

If you want to build the code yourself, you'll need:

- [CMake](https://cmake.org/)
- A C++ compiler. Tested with:
  - Visual Studio Build Tools on Windows
  - Clang on macOS and Ubuntu

To build the code:

```sh
# clone the repo into a local directory
mdkir scs-sdk-plugin
cd scs-sdk-plugin
git clone https://github.com/truckermudgeon/scs-sdk-plugin.git .

# set up the `build` directory
cmake -B build

# build the plugin for the current OS.
cmake --build build --config Release
```

Binaries will be in the `build/bin` directory.

## Credits

- @RenCloud, for the [scs-sdk-plugin](https://github.com/RenCloud/scs-sdk-plugin/) project on which this fork is based.
- @y1lichen, whose [fork](https://github.com/y1lichen/scs-sdk-plugin) inspired me to dig deeper into how shared memory objects work on macOS.
- @jackz314, whose [fork](https://github.com/jackz314/scs-sdk-plugin) taught me about the existence of [C++ Attributes](https://clang.llvm.org/docs/AttributeReference.html#constructor-destructor).