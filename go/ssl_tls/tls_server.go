// 参考https://gist.github.com/denji/12b3a568f092ab951456

package main

import (
	"bufio"
	"crypto/tls"
	"log"
	"net"
)

func main() {
	cer, err := tls.LoadX509KeyPair("server.pem", "server.key")
	if err != nil {
		log.Println(err)
		return
	}

	config := &tls.Config{Certificates: []tls.Certificate{cer}}
	ln, err := tls.Listen("tcp", ":3391", config)
	if err != nil {
		log.Println(err)
		return
	}
	defer ln.Close()

	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Println(err)
			continue
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	r := bufio.NewReader(conn)
	for {
		msg, err := r.ReadString('\n')
		if err != nil {
			log.Println(err)
			return
		}
		log.Println(msg)

		n, err := conn.Write([]byte("Hello World!\n"))
		if err != nil {
			log.Println(n, err)
			return
		}
	}
}
