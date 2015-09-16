package main

// 大端、小端int和byte数组的装换
// 网络字节序是big endian
// 参考：http://stackoverflow.com/questions/17260107/golang-int16-to-byte-array
import (
	"encoding/binary"
	"log"
)

func main() {
	var i uint32 = 0x1234
	b := make([]byte, 4)
	// int转byte数组
	binary.LittleEndian.PutUint32(b, i)
	log.Printf("0x%x LittleEndian\t: %02x %02x %02x %02x", i, b[0], b[1], b[2], b[3])
	binary.BigEndian.PutUint32(b, i)
	log.Printf("0x%x BigEndian\t: %02x %02x %02x %02x", i, b[0], b[1], b[2], b[3])

	// byte数组转int
	d := []byte{0x12, 0x34, 0x56, 0x78}
	i = binary.BigEndian.Uint32(d)
	log.Printf("BigEndian\t:0x%x", i)
	i = binary.LittleEndian.Uint32(d)
	log.Printf("LittleEndian:0x%x", i)
}
