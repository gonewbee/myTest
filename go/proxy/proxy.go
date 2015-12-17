package main

import (
	"io"
	"log"
	"net"
)

func handleTCPProxy(conn net.Conn, rAddr *net.TCPAddr) {
	log.Println("enter connection")
	defer conn.Close()
	rConn, err := net.DialTCP("tcp", nil, rAddr)
	if err != nil {
		panic(err)
	}
	defer rConn.Close()
	go io.Copy(conn, rConn)
	io.Copy(rConn, conn)
	log.Println("connection end")
}

func main() {
	// localAddr := "localhost:3389" // 监听localhost时只能本机访问，别的电脑访问不了。
	localAddr := ":3389" // 不指定IP，本机和其它电脑都可以访问到。
	remoteAddr := "192.168.102.60:3389"
	lAddr, err := net.ResolveTCPAddr("tcp", localAddr)
	if err != nil {
		panic(err)
	}
	rAddr, err := net.ResolveTCPAddr("tcp", remoteAddr)
	if err != nil {
		panic(err)
	}
	listener, err := net.ListenTCP("tcp", lAddr)
	if err != nil {
		panic(err)
	}
	log.Println("listen::", localAddr)
	for {
		conn, err := listener.AcceptTCP()
		if err != nil {
			panic(err)
		}
		go handleTCPProxy(conn, rAddr)
	}
}
