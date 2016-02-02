// go调用win32接口，参考http://stackoverflow.com/questions/22949444/using-go-golang-to-get-windows-idle-time-getlastinputinfo-or-similar
package main

import (
	"log"
	"syscall"
	"unsafe"
)

type PLASTINPUTINFO struct {
	cbSize uint32
	dwTime uint32
}

func main() {
	var lastInputInfo PLASTINPUTINFO
	lastInputInfo.cbSize = uint32(unsafe.Sizeof(lastInputInfo))
	lastInputInfo.dwTime = 0
	log.Printf("lastInputInof dwTime: %d", lastInputInfo.dwTime)

	user32 := syscall.MustLoadDLL("user32.dll")
	getLastInputInfo := user32.MustFindProc("GetLastInputInfo")
	r1, _, err := getLastInputInfo.Call(uintptr(unsafe.Pointer(&lastInputInfo)))
	if r1 == 0 {
		log.Println("error::" + err.Error())
	} else {
		log.Printf("lastInputInof dwTime: %d", lastInputInfo.dwTime)
	}
}
