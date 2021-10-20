# README

## Overview

This code in `main.cpp` is copied from the Stack Overflow question [1].

The `CMakeLists.txt` is copied from [1] with minor changes to fix the linking error and I have written the result as the answer [2].

To fully understand why the static linking didn't work when only `${Boost_LIBRARIES}` was used, see the sibling directory `library-order-in-static-linking`.

## References

- [1] ["Compiling boost::log with static linking doesn't work when including init_from_settings.hpp"](https://stackoverflow.com/q/53551754/630364)
- [2] [My answer to [1]](https://stackoverflow.com/a/69647325/630364)
