// 调用gdi32.dll中接口获取屏幕图像
// 参考http://stackoverflow.com/questions/3291167/how-to-make-screen-screenshot-with-win32-in-c
// http://stackoverflow.com/questions/5069104/fastest-method-of-screen-capturing
// https://github.com/lxn/win/blob/master/gdi32.go

package main

import (
	"io/ioutil"
	"log"
	"syscall"
	"unsafe"
)

type BITMAPINFOHEADER struct {
	BiSize          uint32
	BiWidth         int32
	BiHeight        int32
	BiPlanes        uint16
	BiBitCount      uint16
	BiCompression   uint32
	BiSizeImage     uint32
	BiXPelsPerMeter int32
	BiYPelsPerMeter int32
	BiClrUsed       uint32
	BiClrImportant  uint32
}

type RGBQUAD struct {
	RgbBlue     byte
	RgbGreen    byte
	RgbRed      byte
	RgbReserved byte
}

type BITMAPINFO struct {
	BmiHeader BITMAPINFOHEADER
	BmiColors *RGBQUAD
}

const (
	HORZRES = 8
	VERTRES = 10

	SRCCOPY = 0x00CC0020

	BI_RGB = 0

	DIB_RGB_COLORS = 0
)

func main() {
	// 获取dll
	gdi32 := syscall.MustLoadDLL("Gdi32.dll")

	// 从dll中获取要调用的方法
	createDC := gdi32.MustFindProc("CreateDCW")
	createCompatibleDC := gdi32.MustFindProc("CreateCompatibleDC")
	getDeviceCaps := gdi32.MustFindProc("GetDeviceCaps")
	createCompatibleBitmap := gdi32.MustFindProc("CreateCompatibleBitmap")
	selectObject := gdi32.MustFindProc("SelectObject")
	bitBlt := gdi32.MustFindProc("BitBlt")
	getDIBits := gdi32.MustFindProc("GetDIBits")
	deleteDC := gdi32.MustFindProc("DeleteDC")
	deleteObject := gdi32.MustFindProc("DeleteObject")

	// NULL对应0
	hScreenDC, _, err := createDC.Call(uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr("DISPLAY"))), 0, 0, 0)
	if hScreenDC == 0 {
		panic("createDC error::" + err.Error())
	}
	hMemoryDC, _, _ := createCompatibleDC.Call(hScreenDC)
	defer deleteDC.Call(hMemoryDC)
	defer deleteDC.Call(hScreenDC)

	width, _, _ := getDeviceCaps.Call(hScreenDC, HORZRES)
	height, _, _ := getDeviceCaps.Call(hScreenDC, VERTRES)
	log.Printf("screen width:%d height:%d", width, height)
	hBitmap, _, _ := createCompatibleBitmap.Call(hScreenDC, width, height)
	defer deleteObject.Call(hBitmap)

	selectObject.Call(hMemoryDC, hBitmap)
	bitBlt.Call(hMemoryDC, 0, 0, width, height,
		hScreenDC, 0, 0, SRCCOPY)

	var bmi BITMAPINFOHEADER
	bmi.BiSize = uint32(unsafe.Sizeof(bmi))
	bmi.BiPlanes = 1
	bmi.BiWidth = int32(width)
	bmi.BiHeight = int32(height)
	bmi.BiCompression = BI_RGB
	bmi.BiSizeImage = 0
	bmi.BiXPelsPerMeter = 0
	bmi.BiYPelsPerMeter = 0
	bmi.BiClrUsed = 0
	bmi.BiClrImportant = 0
	log.Printf("bmi size:%d w:%d h:%d", bmi.BiSize, bmi.BiWidth, bmi.BiHeight)

	// var bi BITMAPINFO
	// bi.BmiHeader = bmi

	// len := ((width*32 + 31) / 32) * 4 * height
	len := 4 * width * height
	b := make([]byte, len)
	// var b [4196352]byte
	b[0] = 1

	// 未成功？？？还未发现原因
	ret, _, err := getDIBits.Call(hMemoryDC, hBitmap, 0, height,
		uintptr(unsafe.Pointer(&b[0])), uintptr(unsafe.Pointer(&bmi)), DIB_RGB_COLORS)
	if ret == 0 {
		log.Println("getDIBits err:" + err.Error())
	}
	log.Printf("data: %x %x %x %x %x %x %x %x", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7])
	ioutil.WriteFile("data.tmp", b, 0644)
}
