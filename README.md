# uv_platform

基于 libuv 的快速开发通用框架平台

- [uv\_platform](#uv_platform)
  - [libuv的使用(uvw)](#libuv的使用uvw)
  - [命令行解析](#命令行解析)

## libuv的使用(uvw)

- [uvw](https://github.com/skypjack/uvw) 为 libuv 的 C++ 封装, uvw 的使用请参考 [Demo](https://github.com/skypjack/uvw/tree/main/test/uvw)
- 需要了解 libuv 的原理可以参考官方文档, 但是我们通常不需要直接使用 libuv 的 C 库接口

## 命令行解析

- 内置 TCLAP, 详细使用参考[官方文档](https://tclap.sourceforge.net/manual.html)
- 引入```#include <tclap.hpp>```头文件即可开始使用
- 可以参考```example/args.hpp```的封装方法