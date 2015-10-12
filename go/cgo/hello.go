package main

// #cgo CFLAGS: -I./
// #cgo LDFLAGS: -L./ -lhello
// #include "hello.h"
import "C"
import "log"

func main() {
	d := C.func_add(1, 2)
	log.Printf("d:%d", d)
}
