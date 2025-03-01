# portable_std (BROKEN)
A portable C++ Standard Library implementation for C++17 and later standards that is designed to be compatible with any compiler on any platform. The library is implemented in a way that is OS and compiler agnostic, allowing it to be used on any system that supports C++17 or later standards. The library is designed to be used in place of the standard library provided by the 
compiler, and is intended to provide a common interface that can be used across different platforms and compilers.

## **WARNING**
this is alpha stage lib spec compliance my not be 100%

## Best Time to Use
1. stage 2 Kernel or an app the uses -ffreestanding
2. embedded when you can't use dynamic dlls
3. when you fell like it

## warnings
1. any c++ earlier that c++17 is not going to be supported  
2. this lib will not to be use with the normal c++ std as it will conflict
 
## Building

### Requirements

1. C++23 or later compiler
2. CMake 3.28 or later

### **NOTE:** this only applies to the testing as this it men't to be a header only lib

  ```bash
  git clone https://github.com/pheonixfirewingz/portable_std.git
  cd portable_std
  mkdir build && cd build
  cmake ..
  cmake --build .
  ```

## using in own project

you will need to define the os functions that can be seen in ```tests/os.cpp```

## Reporting Incompatibilities with C++17+

If you encounter any incompatibilities or issues with C++17 or later standards while using this library, please report them. You can do so by opening an issue on our [GitHub repository](https://github.com/pheonixfirewingz/portable_std/issues) with a description of the problem. Please include details such as:

- The specific C++ feature that is causing the issue.
- The compiler and version you are using.
- Any error messages or warnings that are generated.
- A minimal code example that reproduces the issue, if possible.

Your feedback is invaluable in helping us improve the compatibility and quality of this library. Thank you for your contributions!

## Contribute
if you would like to contribute feel free to make a pull request only together well we make the best code
a few rules:
1. **we are all equal** dose not matter for your skin tone, prefers or what you see yourself or country of origin as we are here to write code and thats it we don't need wars over stupid stuff.
2. **this is a c++ project** we will strive for computing safety but c++ is an old language that did not have this put in it's spec this is no reason to force an a gender like let's rewrite it all in rust so please don't ask (note i don't hate rust but is not a point or topic for this project)
3. **respect if your not willing to be professional you have no place here** means no petty tit for tat or you have beef with someone it dose not come into this project