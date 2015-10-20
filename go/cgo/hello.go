package main

// #cgo CFLAGS: -I./
// #cgo LDFLAGS: -L./ -lhello
// #include "hello.h"
/*

typedef struct {
	int id;
	void *num;
} Test_c;

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
	"time"
	"unsafe"
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

// 在C中使用void*保存*chan int进行chan传递
func testWorker(o *C.Test_c) {
	t := (*chan int)(o.num)
	log.Printf("testWorker t:%p", t)
	for i := 0; i < 16; i++ {
		*t <- i * 10
		time.Sleep(1 * time.Second)
	}
}

func newTest(obj *C.Test_c) {
	num := make(chan int)
	obj.id = C.int(10)
	obj.num = unsafe.Pointer(&num)
	log.Printf("newTest num:%p", &num)
}

func main() {
	d := C.func_add(1, 2)
	log.Printf("d:%d", d)
	var math C.mathOps
	C.math_new(&math)
	C.mathOps_test(&math)
	var obj C.Test_c
	newTest(&obj)
	go testWorker(&obj)
	tchan := (*chan int)(obj.num)
	log.Printf("main tchan:%p", tchan)
	for i := 0; i < 20; i++ {
		select {
		case t := <-(*tchan):
			log.Println(t)
		case <-time.After(time.Second * 4):
			log.Println("time out")
			return
		}
	}
}
