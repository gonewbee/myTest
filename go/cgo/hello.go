package main

// #cgo CFLAGS: -I./
// #cgo LDFLAGS: -L./ -lhello
// #include "hello.h"
/*

extern int myadd(int a, int b);
extern int mysubtract(int a, int b);
extern int mymultiply(int a, int b);
extern int mydivide(int a, int b);

static void math_new(mathOps *math) {
	math->add = myadd;
	math->subtract = mysubtract;
	math->multiply = mymultiply;
	math->divide = mydivide;
}
*/
import "C"
import (
	"log"
)

//export myadd
func myadd(a, b C.int) C.int {
	return a + b
}

//export mysubtract
func mysubtract(a, b C.int) C.int {
	return a - b
}

//export mymultiply
func mymultiply(a, b C.int) C.int {
	return a * b
}

//export mydivide
func mydivide(a, b C.int) C.int {
	return a / b
}

func main() {
	d := C.func_add(1, 2)
	log.Printf("d:%d", d)
	var math C.mathOps
	C.math_new(&math)
	C.mathOps_test(&math)
}
