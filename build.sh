
rm -rf *.elf *.so *.a

# rustc --crate-type staticlib test.rs -O -o librh.a
rustc --crate-type cdylib test.rs -O -o librh.so

gcc -o main.elf main.c -g -ggdb -O0 -Wl,-rpath=./ -L./ -lrh

./main.elf
