package main

import (
	"encoding/json"
	"golang.org/x/net/websocket"
	"log"
	"net/http"
	"strconv"
	"time"
)

type mousePos struct {
	X int `json:"x"`
	Y int `json:"y"`
}

func wsWorker(ws *websocket.Conn, msg chan<- string, wsClosed chan<- bool) {
	var message string
	for nil == websocket.Message.Receive(ws, &message) {
		log.Println("receive:" + message)
		msg <- message
	}
	wsClosed <- true
	log.Println("wsWorker end=========")
}

func testWorker(msg chan<- string, quit <-chan bool) {
	i := 0
	for {
		select {
		case msg <- "#" + strconv.FormatInt(int64(i*0x100000), 16):
			log.Printf("testWorker:%d", i)
			time.Sleep(5 * time.Second)
			i = (i + 1) % 16
		case <-quit:
			// 退出测试Goroutine
			log.Println("testWorker return---------------")
			return
		default:
			log.Println("testWorker default!!!")
		}
	}
	log.Println("testWorker end============")
}

func wsHandler(ws *websocket.Conn) {
	c1 := make(chan string)
	c2 := make(chan string)
	wsClosed := make(chan bool)
	quit := make(chan bool)

	go wsWorker(ws, c1, wsClosed)
	go testWorker(c2, quit)

forLoop:
	for {
		// 使用Select选择要发送的数据
		select {
		case msg1 := <-c1:
			log.Println("c1 receive:" + msg1)
			var pos mousePos
			if err := json.Unmarshal([]byte(msg1), &pos); err != nil {
				log.Panic("Unmarshal error!")
				break
			}
			// 根据坐标回复颜色值
			msg1 = "#" + strconv.FormatInt(int64(pos.X*10), 16) + strconv.FormatInt(int64(pos.Y*10), 16)
			websocket.Message.Send(ws, msg1)
		case msg2 := <-c2:
			log.Println("c2 receive:" + msg2)
			websocket.Message.Send(ws, msg2)
		case <-wsClosed:
			quit <- true
			log.Printf("wsClosed")
			break forLoop
		}
	}
	log.Println("wsHandler end======")
}

func main() {
	http.Handle("/", http.FileServer(http.Dir("./")))
	http.Handle("/wsDemo", websocket.Handler(wsHandler))
	log.Fatal(http.ListenAndServe(":80", nil))
}
