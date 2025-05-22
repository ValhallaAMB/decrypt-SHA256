# decrypt-SHA256

## Prerequisites

1. [Visual Studio Code](https://code.visualstudio.com/)
2. [C/C++ compiler (MSYS2 Mingw-w64)](https://code.visualstudio.com/docs/languages/cpp#_install-a-compiler)

## Setup guide

1. Install `vcpkg`, through your terminal, for managing C/C++ libraries. Make sure you clone the git repository inside of your C drive.

```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

2. Install the library `OpenSSL` for SHA256 encryption.

```
.\vcpkg.exe install openssl:x64-windows
```

3. Integrates with Visual Studio (Windows-only), sets the user-wide vcpkg instance

```
vcpkg integrate install
```

4. Add the following to your `tasks.json` with your `.vscode` folder.

```
"tasks": [
    {
        "label": "Build & Run (vcpkg + OpenMP)", // No need to change this
        "args": ["-fopenmp", "-lcrypto", ...],
        ...
    }
]
```

5. Build and Run the program with `g++.exe` as your compiler.

6. You might need to reload your vscode. Press `ctrl+shift+p` then type and run `Developer: Reload Window`, or close and open your vscode. 