# Linux Programming

## 1. Lý thuyết

### 1.1. File và File System


### 1.2. Process & Thread

#### 1.2.1. Process


#### 1.2.2. Thread


### 1.3. Synchronization


### 1.4. Networking


## 2. Bài tập

### 2.1. Tools

#### 2.1.1. GDB on Linux
Debugger là một chương trình chạy các chương trình khác, cho phép người dùng kiểm soát các chương trình này và kiểm tra các biến khi có vấn đề phát sinh.

GNU Debugger (GDB) là debugger phổ biến nhất cho UNIX system để debug chương trình C/C++.

- Kiểm tra đã cài đặt GDB chưa? - `gdb -help`

- Cài đặt GDB - `sudo apt-get install libc6-dbg gdb valgrind`

Để GDB có thể đọc tất cả thông tin, ta cần compile chương trình, cách thông thường là: `gcc hello.cc -o hello`. Thay vì làm vậy, ta cần compile với `-g`: `gcc -g hello.cc -o hello`.

- [GDB Commands](https://www.tutorialspoint.com/gnu_debugger/gdb_commands.htm)

- 



#### 2.1.2. makefile


### 2.2. Linux Command


### 2.3. Trò chơi xếp bi

## 3. Nguồn tham khảo

- [GDB Debugging](https://www.youtube.com/watch?v=5yZIFmplXsw&list=PLMbKVy-fuVKF0iopxoNRvhE86xYWZz9-C)

- [CMake](https://viblo.asia/p/dao-dau-voi-cmake-thong-qua-vi-du-07LKXNbelV4)

