package main

import (
	"log"
	"time"
)

func chanStrTest(info chan<- string) {
	ticker := time.NewTicker(time.Millisecond * 3000)
	for t := range ticker.C {
		log.Println("chanStrTest Tick at", t)
		select {
		case info <- t.String():
			log.Println("chanStrTest send ok")
		case <-time.After(time.Second * 1):
			log.Println("chanStrTest timeout")
			ticker.Stop()
		}
	}
	log.Println("chanStrTest end------------")
}

func createBytes(i int, info chan<- []byte) {
	b := make([]byte, 144000+i)
	b[0] = byte(i)
	b[1] = byte(i)
	info <- b
}

func chanBytesTest(info chan<- []byte) {
	for i := 0; i < 100; i++ {
		time.Sleep(time.Second * 1)
		createBytes(i, info)
	}
}

func main() {
	infoStr := make(chan string)
	infoBytes := make(chan []byte)
	go chanStrTest(infoStr)
	go chanBytesTest(infoBytes)

forLoop:
	for {
		select {
		case str := <-infoStr:
			log.Println(str)
		case bs := <-infoBytes:
			log.Printf("[]byte len:%d;;%x %x ", cap(bs), bs[0], bs[1])
		case <-time.After(time.Second * 2):
			log.Println("time out")
			break forLoop
			// return
		}
	}
	log.Println("main func end===========")
}
