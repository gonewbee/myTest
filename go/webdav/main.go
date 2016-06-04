package main

import (
	"golang.org/x/net/webdav"
	"log"
	"net/http"
)

func main() {
	h := &webdav.Handler{
		// 在内存中的文件系统，重启应用后就消失
		// FileSystem: webdav.NewMemFS(),
		// 挂载本地文件系统
		FileSystem: webdav.Dir("."),
		LockSystem: webdav.NewMemLS(),
		// 设置log
		Logger: func(r *http.Request, err error) {
			log.Printf("[dav] %-10s%-30s%v", r.Method, r.URL.Path, err)
		},
	}
	// 挂载webdav服务：net use P: http://localhost:5555
	// 取消挂载：net use P: /del /y
	http.HandleFunc("/", h.ServeHTTP)
	http.ListenAndServe(":5555", nil)
}
