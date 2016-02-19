// 参考https://gist.github.com/denji/12b3a568f092ab951456

package main

import (
	"crypto/tls"
	"log"
	"net"
)

func main() {
	localAddr := ":3391"
	cer, err := tls.LoadX509KeyPair("server.pem", "server.key")
	if err != nil {
		log.Println(err)
		return
	}

	config := &tls.Config{Certificates: []tls.Certificate{cer}}
	lAddr, err := net.ResolveTCPAddr("tcp", localAddr)
	if err != nil {
		panic(err)
	}

	listener, err := net.ListenTCP("tcp", lAddr)
	if err != nil {
		panic(err)
	}
	defer listener.Close()

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Println(err)
			continue
		}
		go handleConnection(conn, config)
	}
}

func handleConnection(conn net.Conn, config *tls.Config) {
	defer conn.Close()
	buf := make([]byte, 10*1024)
	for {
		nr, err := conn.Read(buf)
		if err != nil {
			log.Println(err)
			break
		}
		if nr > 0 {
			log.Printf("%#v\n", buf[:4])
			if buf[0] == 0x03 && buf[1] == 0x00 && buf[2] == 0x00 {
				ret := []byte{0x03, 0x00, 0x00, 0x13, 0x0e, 0xd0, 0x00, 0x00, 0x12, 0x34, 0x00, 0x02, 0x01, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00}
				conn.Write(ret)
				ser := tls.Server(conn, config)
				defer ser.Close()
				for {
					n, err := ser.Read(buf)
					if err != nil {
						log.Println(err)
						return
					}
					log.Printf("%d  %#v\n", n, buf[:4])
				}
			}
		}
	}
}
