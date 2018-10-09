# EOWU

eowu is a performant, flexible state-machine. 

State logic is written in lua, with utilities provided to simplify matters of timing and stimulus presentation. See the `examples` subfolder for a couple of basic demos.

### build

eowu currently supports macOS and Windows. A compiler with c++17 support is required, along with `cmake`. Graphics hardware with support for OpenGL 3.3+ is also required.

### build instructions - common to all platforms
```bash
git clone --recursive https://github.com/nfagan/eowu.git
cd ./eowu
mkdir build
cd ./build
```

#### build instructions - mac
```bash
cmake -G Xcode ..
cmake --build . --target ALL_BUILD --config Release
```

#### build instructions - windows
```bash
cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build . --target ALL_BUILD --config Release
```

### run

The main executable is `eowu-run`, created by default in `/path/to/eowu-repository/build/eowu-runtime/Release`. The executable takes a single argument: the path to a `setup.lua` or similarly named entrypoint. For an idea of what this file should contain, see the `examples` folder.