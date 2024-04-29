# C && Rust Mix-coding programming start up
----

这是一个简单的模板，用于讲述最简单的使用 C 和 Rust 进行混合语言编程所需要的一切。

就本示例而言，它提供了一个这样的场景：对于一个维护多年的大型 C/C++ 项目而言，由于 C/C++ 本身的内存管理机制很容易出现内存安全问题(缓存区溢出，Use-After-Free，Double-Free等)。

Rust 使用所有权系统(Ownership)保障了内存安全，然而在短时间内使用 Rust 重写维护多年的项目不论是在时间上还是经济成本上都是不现实的。因此在实现新的功能模块时，可以使用 Rust 进行开发，并且在后续的迭代过程中逐步使用 Rust 完善，从而减少代码中的内存漏洞。

遵循以下步骤开始：

### 1. 安装 C 语言开发套件
```shell
sudo apt-get update
sudo apt-get install -y build-essential curl
gcc -v
```

### 2. 使用 rustup 安装 Rust 套件
```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
# Just Press "Enter"
source ${HOME}/.cargo/env
rustc -V
```

### 3. 示例程序

本示例功能如下：在 C 语言中创建一个 int[], 然后调用 Rust 中的一个函数，该函数的功能是将输入数组中的数字全部加一个 float, 并返回一个新的 array。

eg： 
```
input:  [1, 2, 3, 4], 3.14
output: [4, 5, 6, 7]
```

其中，C 语言部分代码如下(可以存储为 `main.c`)：
```C
#include <stdio.h>
#include <stdint.h>

extern int64_t* sum_elements(int[], int, float);
extern void     free_memory(int64_t*);

int main() {
    int array[] = {1, 2, 3, 4};
    float float_number = 3.14;
    int64_t* results = sum_elements(array, sizeof(array)/sizeof(int), float_number);
    for (int i = 0; i < sizeof(array)/sizeof(int); i++) {
        printf("--> idx: %d, %ld \n", i, results[i]);
    }
    free_memory(results);
    return 0;
}
```

Rust 部分代码如下(可以存储为 `test.rs`)：
```rust
use std::os::raw::{c_int, c_float, c_longlong};

#[no_mangle]
pub unsafe extern "C" fn sum_elements(array: *const c_int, len: c_int, float_number: c_float) -> *const c_longlong {
    let array = std::slice::from_raw_parts(array, len as usize);
    let mut results: Vec<c_longlong> = Vec::new();

    println!("Rust Recv Array Length: {} ", len);

    for &num in array {
        results.push(((num as f32) + float_number) as c_longlong);
    }
    
    let result_ptr = results.as_ptr();
    std::mem::forget(results);
    result_ptr
}

#[no_mangle]
pub extern "C" fn free_memory(ptr: *mut c_longlong) {
    unsafe {
        drop(Box::from_raw(ptr));  // 重新获得所有权并删除Box，这将自动释放内存
    }
    println!("Rust Free!");
}
```

使用如下命令进行编译：
```shell
# rustc --crate-type staticlib test.rs -o librh.a
rustc --crate-type cdylib test.rs -o librh.so
gcc -o main.elf main.c -g -ggdb -O0 -Wl,-rpath=./ -L./ -lrh
```

其中，将 Rust 代码编译成动态库 `.so` 文件时，需要加 `--crate-type cdylib` 选项；

同理，将 Rust 代码编译成静态库 `.a ` 文件时，需要加 `--crate-type staticlib` 选项；

gcc 在编译时，先传递**编译选项**， 再传递**链接选项**，否则会导致链接错误。

在使用 `-l` 链接时，需要忽略库的文件名前缀 `lib`。例如链接 `-ltest` 时，连接器会寻找 `-L` 目录下的 `libtest.so`(或`libtest.a`) 库。

预期的输出结果如下：
```text
Rust Recv Array Length: 4 
--> idx: 0, 4 
--> idx: 1, 5 
--> idx: 2, 6 
--> idx: 3, 7 
Rust Free!
```

