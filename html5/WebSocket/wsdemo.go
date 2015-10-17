package main

import (
	"golang.org/x/net/websocket"
	"log"
	"net/http"
)

func wsHandler(ws *websocket.Conn) {
	var message string
	websocket.Message.Receive(ws, &message)
	log.Println("receive:" + message)
}

func main() {
	http.Handle("/", http.FileServer(http.Dir("./")))
	http.Handle("/wsDemo", websocket.Handler(wsHandler))
	log.Fatal(http.ListenAndServe(":80", nil))
}
