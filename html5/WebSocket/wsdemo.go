package main

import (
	"encoding/json"
	"golang.org/x/net/websocket"
	"log"
	"net/http"
	"strconv"
)

type mousePos struct {
	X int `json:"x"`
	Y int `json:"y"`
}

func wsHandler(ws *websocket.Conn) {
	var message string
	for nil == websocket.Message.Receive(ws, &message) {
		log.Println("receive:" + message)
		var pos mousePos
		if err := json.Unmarshal([]byte(message), &pos); err != nil {
			log.Panic("Unmarshal error!")
			break
		}
		// 根据坐标回复颜色值
		message = "#" + strconv.FormatInt(int64(pos.X*10), 16) + strconv.FormatInt(int64(pos.Y*10), 16)
		websocket.Message.Send(ws, message)
	}
	log.Println("wsHandler end!!")
}

func main() {
	http.Handle("/", http.FileServer(http.Dir("./")))
	http.Handle("/wsDemo", websocket.Handler(wsHandler))
	log.Fatal(http.ListenAndServe(":80", nil))
}
