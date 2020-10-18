### ionir

ionir is a collection of in-memory intermediate representation constructs used by the Ion compiler. Type-checking,
borrow checking, optimization, and other important passes except for the macro expansion and name resolution passes
are present and occur in ionir.

The [ionlang project](https://github.com/ionlang/ionlang) (the Ion compiler) consumes
and emits ionir constructs which are then processed and emitted through the code generation pass to LLVM IR.

### Requirements

* [ionshared](https://github.com/ionlang/ionshared)
* [zlib](https://zlib.net/)
* [CMake](https://cmake.org/download/) `>=v3.13.X`
* [Python](https://www.python.org/download/releases/2.7/) `=v2.7`
* [MinGW-w64 (Windows)](https://ayera.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe)
    * with GNU `>=v8.1.0`
* [LLVM](https://releases.llvm.org/download.html)¹ `=v9.0.0`

---
1. _LLVM must be built from source on Windows. A different, close version of LLVM
might work, but you will need to modify `CMakeLists.txt`, specifically where
`find_package(LLVM X.0.0 REQUIRED CONFIG)` occurs._

### Building & installing

```shell
# Clone the repository.¹
$ git clone https://github.com/ionir
$ cd ionir

# Generate Makefiles and build.
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .

# Install the library. Might require additional permissions.²
$ cmake --install .
```

---
1. _Make sure you've selected the correct branch that you're intending to
build (`dev` for latest changes, `master` for stable), and initialized
git submodules after cloning the repository and prior to building._

2. _Installing the library might require administrator (or super-user)
permissions, as the installed files will likely be placed under restricted
paths. If you're using Windows, re-open the Command prompt (or PowerShell)
as Administrator. If you're on a Unix-like system, simple prepend `sudo` to
the install command._

### Usage examples

<details>
    <summary>Registering & running passes</summary>

```cpp
#include <iostream>
#include <ionir/passes/semantic/name_resolution_pass.h>
#include <ionir/passes/semantic/name_shadowing_pass.h>
#include <ionir/passes/optimization/dead_code_elimination_pass.h>
#include <ionir/passes/type_system/type_checker_pass.h>
#include <ionir/passes/codegen/llvm_codegen_pass.h>
#include <ionir/passes/construct_validation_pass.h>
#include <ionir/passes/pass_manager.h>
#include <ionir/generation/driver.h>
#include <ionir/lexical/lexer.h>
#include <ionir/lexical/token.h>
#include <ionir/misc/iterator.h>
#include <ionir/misc/helpers.h>
#include <ionir/misc/static_init.h>
#include <ionir/construct/construct.h>

using namespace ionir;

int main() {
    Ast ast = {
        // ...
    };

    /**
     * Create a PassManager instance and register various passes which
     * will traverse, analyze and modify the resulting, parsed AST. It is
     * important to note that the order in which passes are registered matters
     * since those registered and executed first may modify the AST.
     */
    PassManager passManager;

    /**
     * ConstructValidationPass: Ensures that all parsed
     * nodes are not missing (having null) its required
     * properties.
     */
    passManager.registerPass(std::make_shared<ConstructValidationPass>());

    /**
     * DeadCodeEliminationPass: An optimization pass which will remove
     * unreachable code from the resulting, parsed AST. For example, code
     * after a return statement is considered dead since it will never be
     * reached nor executed. Another example would be the removal of unused
     * variables, methods, classes, among other entities from the AST.
     */
    passManager.registerPass(std::make_shared<DeadCodeEliminationPass>());

    /**
     * TypeCheckPass: At this point, type checking will occur. This pass's
     * responsibility is enforcing various semantic type rules of the language.
     * For example, binary operations involving incompatible type operands will
     * result in a type error produced by this pass. Another example would be
     * verifying that return values' types are compatible with the defined return
     * type of its corresponding parent function.
     */
    passManager.registerPass(std::make_shared<TypeCheckPass>());

    /**
     * LlvmCodegenPass: Finally, the target code-generation pass will be registered
     * last after all optimizations and semantic analysis has taken place. This pass
     * will generate LLVM IR from the parsed AST. IonIR is designed in a way to support
     * multiple different targets, with the ability to create new ones simply by creating
     * and implementing a Pass class. This, in turn, makes the implementation of new targets
     * a breeze.
     */
    std::shared_ptr<LlvmCodegenPass> llvmCodegenPass = std::make_shared<LlvmCodegenPass>();

    passManager.registerPass(llvmCodegenPass);

    /**
     * After registering all the corresponding passes, instruct the pass manager to execute
     * all the passes while respecting the order in which they were registered.
     */
    passManager.run(ast);

    LlvmModule module = LlvmModule(llvmCodegenPass->getModule());

    // Since we're targeting LLVM IR, print out the resulting LLVM IR code.
    std::cout << module.makeIr() << std::endl;

    // Finish program.
    return 0;
}
```
</details>

### Running tests

Use the `ctest` command bundled with CMake to run tests.

Alternatively, build project & tests and run the tests executable generated.

---
### Developer notes

<details>
    <summary>Building & installing LLVM</summary>

#### (Recommended) Automatically building LLVM from source (Windows)

0. Ensure you have _all_ the requirements (excluding LLVM) properly installed, as the build process for LLVM on Windows will require Python, CMake and MinGW.

1. Download sources ([Visit LLVM's downloads page](http://releases.llvm.org/download.html)) for LLVM 9.0.0 (or [click here](http://releases.llvm.org/9.0.0/llvm-9.0.0.src.tar.xz) to download them directly).

2. Extract sources onto a directory whose absolute path contains no spaces. For example, use `C:/LLVM_BUILD` instead of `C:/LLVM BUILD`. This will prevent the CMake build from failing, since it seems that certain project(s) have a hard time with paths containing spaces in Windows.

3. Copy the `install_llvm.bat` Batch script located under the `tools/` directory to the folder where you extracted the sources.

4. Run the `install_llvm.bat` file you just copied **as administrator** by right-clicking it then selecting `Run as administrator`.

#### Manually building LLVM from source (Windows)

0. Ensure you have _all_ the requirements (excluding LLVM) properly installed, as the build process for LLVM on Windows will require Python, CMake and MinGW.

Additionally, install the following packages using the MinGW Installation Manager if not already installed:

* `mingw32-gcc-bin`
* `mingw32-gcc-g++-bin`
* `mingw32-make-bin`

1. Download sources ([Visit LLVM's downloads page](http://releases.llvm.org/download.html)) for LLVM 9.0.0 (or [click here](http://releases.llvm.org/9.0.0/llvm-9.0.0.src.tar.xz) to download them directly).

2. Extract sources onto a directory whose absolute path contains no spaces. For example, use `C:/LLVM_BUILD` instead of `C:/LLVM BUILD`. This will prevent the CMake build from failing, since it seems that a certain project has a hard time with paths with spaces.

3. Build sources using a Windows command prompt with **administrator privileges** by issuing the following commands:

```
> cd <sources directory>
> mkdir build
> cd build
> cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DLLVM_BUILD_LLVM_DYLIB=ON -DLLVM_INCLUDE_TESTS=OFF -DLLVM_ENABLE_ZLIB=OFF
> cmake --build . --config Release --target install -j8
> cd ..
```

**First command (cmake -G ...)**

It must be specified to CMake (through `-G "MinGW Makefiles`) to use MinGW on Windows (GCC) since LLVM cannot be built by MSVC (the Visual Studio C++ Compiler), thus making MinGW a requirement in Windows.

A build directory is required by MinGW, this is why the `build` directory is created and used.

The `-j8` flag instructs CMake to use 8 parallel builds (or concurrent jobs), so as to not take up 100% CPU usage. Beware that the more jobs specified, the more RAM that may be used. Note that specifying too many jobs may cause memory exhaustion. If you feel that you have complient hardware, you may adjust this flag accordingly.

The `LLVM_BUILD_LLVM_DYLIB=ON` option will instruct the compiler to add the shared library target `libLLVM` which is required since the project is built dynamically on Windows, not statically.

The `LLVM_INCLUDE_TESTS=OFF` option will instruct CMake to omit building tests, which would conflict with the existing Google Test project on IonIR.

**Second command (cmake --build ...)**

The `--target install` option will instruct CMake to copy the resulting LLVM installation to the `C:/Program Files (x86)/LLVM` directory on Windows. This is what we're aiming for. Additionally, please note that the target name is case-sensitive, and should always be `install` (rather than `INSTALL`).

The `CMAKE_BUILD_TYPE=Release` option will instruct CMake to build the project under the `Release` configuration, instead of `Debug`, since we're going to be installing this on our local machine.

**Additional notes**

* LLVM sources, which are exclusively compatible with CMake, should now be ready to be included in your `CMakeLists.txt` file. See example below:

```cmake
...
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Specify executable(s).
add_executable(${PROJECT_NAME} ${SOURCES})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries.
target_link_libraries(${PROJECT_NAME} ${llvm_libs})
...
```

For additional information on building LLVM with CMake, [view the LLVM docs on building with CMake](https://llvm.org/docs/CMake.html).

#### Installing LLVM (Linux)

Please refer to [this link](http://apt.llvm.org/) for instructions on installing LLVM on Linux systems. It is recommended to use the automatic installation script provided there.

#### Installing LLVM (macOS)

Simply use [Homebrew](https://brew.sh/) to install LLVM:

```
$ brew install llvm@9
```

#### Additional notes

If CMake is having a hard time finding your LLVM installation, try setting the `LLVM_DIR` environment variable to the root folder of the installation. Example: If you build LLVM from source and have it located on your Downloads folder.
</details>

#### Building project

* In Visual Studio Code, use `Clang X.0.0 for MSVC with Visual Studio Community 2019 (amd64)` as the build/compiler tool.
* When using CMake, first configure the project using `cmake --config Debug .` then build the executable using `cmake --build .`.
* Alternatively, configure the project using `cmake --config .` then build the executable with the default options using `cmake --build .`.

#### Visual Studio Code notes

If intelli-sense is failing in Visual Studio Code, there may be a few reasons behind it:

* Try re-installing plugin (usually fixes the problem).
* Compile project and its dependencies in your platform (ex. avoid using WSL on Windows).
* Ensure dependencies are properly installed and have correct versions.

1. Additional tools

* [libTooling](https://clang.llvm.org/docs/LibTooling.html) (not yet employed)
* [libClang](https://clang.llvm.org/doxygen/group__CINDEX.html) (not yet employed)

[Click here](https://clang.llvm.org/docs/Tooling.html) for more information.

2. Recommended use of Clang/Clang++

It is recommended to use the Clang-9 compiler to build this project. You can specify the compiler to CMake using the environment variables of `CC` and `CXX` like so:

```bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
```

Then running CMake.

Alternatively, you can also try running:

```bash
$ sudo update-alternatives --config c++
```

Then selecting `/usr/bin/clang++`.

<details>
    <summary>Resources</summary>
    
#### Type theory resources

1. [LEAN tutorial](https://leanprover.github.io/tutorial/tutorial.pdf)
2. [LEAN tutorial playground](https://leanprover.github.io/tutorial/)
3. [LEAN web editor](https://leanprover.github.io/live/latest/)
</details>

#### Common problems

* Linux: `/usr/bin/ld: cannot find -ledit`:

Install the `libedit-dev` package:

```bash
$ sudo apt-get install -y libedit-dev
```

* Linux: `Cannot open file 'optional.h'`:

Upgrade your `g++` package to `g++-8`, then replace the symbolic link with the executable of the updated package (using `ln`):

```bash
sudo apt-get install -y g++-8
sudo rm /usr/bin/g++
sudo ln /usr/bin/g++-8 /usr/bin/g++
```

#### Style guide

This project partially adopts [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
