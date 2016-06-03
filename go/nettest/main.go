package main

import (
	"io"
	"log"
	"net/http"
	"os/exec"
	"runtime"
	"time"
)

func HandleIndex(w http.ResponseWriter, r *http.Request) {
	io.WriteString(w, index_html)
}

func HandleReq(w http.ResponseWriter, r *http.Request) {
	log.Println("Method:" + r.Method)
	// r.Method请求类型
	switch r.Method {
	case "GET":
		// r.URL.Query()获取请求参数
		log.Println(r.URL.Query())
	case "POST":
		// 获取post方法的表单
		r.ParseForm()
		log.Println(r.Form)
		// 解析MultipartForm类型表单
		r.ParseMultipartForm(2048)
		log.Println(r.MultipartForm)
		// 使用range遍历map
		for k, v := range r.MultipartForm.Value {
			log.Printf("%s -> %s\n", k, v)
		}
	}
}

func main() {
	port := "13389"
	http.HandleFunc("/", HandleIndex)
	http.HandleFunc("/req", HandleReq)
	// 使用goroutine，在2秒后启动URL
	go func() {
		// 判断系统类型
		if runtime.GOOS == "windows" {
			log.Println("windows")
			time.Sleep(time.Second * 2)
			// 执行windows命令，启动URL
			exec.Command("cmd", "/C", "start", "http://localhost:"+port).Run()
		}
	}()
	log.Println(runtime.GOOS + " Listening port:" + port)
	log.Fatal(http.ListenAndServe(":"+port, nil))
}
