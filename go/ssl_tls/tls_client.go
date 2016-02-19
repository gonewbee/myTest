package main

import (
	"crypto/tls"
	"log"
)

func main() {
	config := &tls.Config{InsecureSkipVerify: true}

	conn, err := tls.Dial("tcp", "127.0.0.1:3391", config)
	if err != nil {
		log.Println(err)
		return
	}
	defer conn.Close()

	n, err := conn.Write([]byte("Client Message!\n"))
	if err != nil {
		log.Println(n, err)
		return
	}

	buf := make([]byte, 100)
	n, err = conn.Read(buf)
	if err != nil {
		log.Println(n, err)
		return
	}
	log.Println(string(buf[:n]))
}
