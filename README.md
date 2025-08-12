# learnopengl

This code is based on material from https://learnopengl.com/.

## Build dependencies

On Fedora, install these dependencies:

```sh
sudo dnf install wayland-devel libxkbcommon-devel libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel
```

## Build

You will need CMake to build this project.

Run

```sh
cmake -S . -B ./build
```

to generate build files.

Then run

```sh
cmake --build ./build
```

to build the project.

After that, the compiled binary will be in the `build` directory. Launch it:

```sh
cd build
./game
```