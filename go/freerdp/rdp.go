package main

// #cgo CFLAGS: -I/home/zsy/Workspace/code/ubuntu14/FreeRDP/include
// #cgo CFLAGS: -I/home/zsy/Workspace/code/ubuntu14/FreeRDP/winpr/include
// #cgo LDFLAGS: -L/home/zsy/Workspace/code/ubuntu14/FreeRDP/libfreerdp -lfreerdp
// #cgo LDFLAGS: -L/home/zsy/Workspace/code/ubuntu14/FreeRDP/client/common -lfreerdp-client
// #include "freerdp/freerdp.h"
// #include "freerdp/client.h"
//
// extern BOOL xfreerdp_client_global_init();
//
// static int RdpClientEntry(RDP_CLIENT_ENTRY_POINTS* pEntryPoints) {
// 	pEntryPoints->Version = 1;
// 	pEntryPoints->Size = sizeof(RDP_CLIENT_ENTRY_POINTS_V1);
// 	pEntryPoints->GlobalInit = xfreerdp_client_global_init;
// }
import "C"

import (
	"log"
	"unsafe"
)

func test() {
	instance := C.freerdp_new()
	log.Printf("instance address:%p", instance)
	log.Printf("instance:%v", instance)
	log.Println(instance.PreConnect)
}

//export xfreerdp_client_global_init
func xfreerdp_client_global_init() C.BOOL {
	log.Println("xfreerdp_client_global_init")
	return C.BOOL(C.TRUE)
}

// func RdpClientEntry(pEntryPoints *C.RDP_CLIENT_ENTRY_POINTS) int {
// 	pEntryPoints.Version = 1
// 	var t C.RDP_CLIENT_ENTRY_POINTS_V1
// 	pEntryPoints.Size = C.DWORD(unsafe.Sizeof(t))
// 	pEntryPoints.GlobalInit = C.pRdpGlobalInit(&Xfreerdp_client_global_init)
// 	return 0
// }

func Rdp_new() {
	var clientEntryPoints C.RDP_CLIENT_ENTRY_POINTS
	clientEntryPoints.Size = C.DWORD(unsafe.Sizeof(clientEntryPoints))
	clientEntryPoints.Version = C.RDP_CLIENT_INTERFACE_VERSION
	log.Printf("size:%d version:%d", clientEntryPoints.Size, clientEntryPoints.Version)
	C.RdpClientEntry(&clientEntryPoints)
	log.Printf("size:%d version:%d", clientEntryPoints.Size, clientEntryPoints.Version)
	C.freerdp_client_context_new(&clientEntryPoints)
}
