// 参考https://gist.github.com/denji/12b3a568f092ab951456

package main

import (
	"crypto/tls"
	"log"
	"net"
)

func main() {
	localAddr := ":3391"               // 监听本机3391端口
	remoteAddr := "192.168.7.109:7171" // 7171端口的rds服务应该是未加密的
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
	rAddr, err := net.ResolveTCPAddr("tcp", remoteAddr)
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
		go handleConnection(conn, config, rAddr)
	}
}

func handleConnection(conn net.Conn, config *tls.Config, rAddr *net.TCPAddr) {
	defer conn.Close()
	rConn, err := net.DialTCP("tcp", nil, rAddr)
	if err != nil {
		panic(err)
	}
	defer rConn.Close()

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
				// 自行处理第一组数据
				// ret := []byte{0x03, 0x00, 0x00, 0x0b, 0x06, 0xd0, 0x00, 0x00, 0x12, 0x34, 0x00}
				// ret := []byte{0x03, 0x00, 0x00, 0x13, 0x0e, 0xd0, 0x00, 0x00, 0x12, 0x34, 0x00, 0x02, 0x01, 0x08, 0x00, 0x02, 0x00, 0x00, 0x00}
				ret := []byte{0x03, 0x00, 0x00, 0x13, 0x0e, 0xd0, 0x00, 0x00, 0x12, 0x34, 0x00, 0x02, 0x01, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00} // 使用tls
				conn.Write(ret)

				fakeUp := []byte{0x03, 0x00, 0x00, 0x25, 0x20, 0xe0, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x43, 0x6f, 0x6f, 0x6b, 0x69, 0x65, 0x3a, 0x20, 0x6d, 0x73, 0x74, 0x73, 0x68,
					0x61, 0x73, 0x68, 0x3d, 0x69, 0x65, 0x63, 0x6a, 0x65, 0x6b, 0x6e, 0x0d, 0x0a}
				rConn.Write(fakeUp)
				rConn.Read(buf)

				ser := tls.Server(conn, config)
				defer ser.Close()
				go transferTCP(rConn, ser, true)
				transferTCP(ser, rConn, false)
				return
			}
		}
	}
}

func transferTCP(src net.Conn, dst net.Conn, down bool) {
	log.Println("transferTCP in")
	buf := make([]byte, 10*1024)
	for {
		nr, er := src.Read(buf)
		if er != nil {
			log.Println(er)
			break
		}
		if down {
			log.Printf("down len:%d %#v\n", nr, buf[0:8])
		} else {
			log.Printf("up len:%d %#v\n", nr, buf[0:8])
		}
		if nr > 0 {
			// log.Println(nr)
			nw, ew := dst.Write(buf[0:nr])
			if ew != nil {
				log.Println(ew)
				break
			}
			if nr != nw {
				log.Println("write!=read!")
				break
			}
		}
	}
	log.Println("transferTCP end")
}
