package main

// 大端、小端int和byte数组的装换
// 网络字节序是big endian
// 参考：http://stackoverflow.com/questions/17260107/golang-int16-to-byte-array
import (
	"encoding/binary"
	"log"
)

func main() {
	var i uint32 = 0x12345678
	b := make([]byte, 4)
	// int转byte数组
	binary.LittleEndian.PutUint32(b, i)
	log.Printf("0x%x LittleEndian\t: %02x %02x %02x %02x", i, b[0], b[1], b[2], b[3])
	binary.BigEndian.PutUint32(b, i)
	log.Printf("0x%x BigEndian\t: %02x %02x %02x %02x", i, b[0], b[1], b[2], b[3])

	t := []byte{1, 1, 1, 1, 1, 1}
	binary.LittleEndian.PutUint32(t[2:], i)
	log.Printf("%#v", t)

	// byte数组转int
	d := []byte{0x12, 0x34, 0x56, 0x78}
	i = binary.BigEndian.Uint32(d)
	log.Printf("BigEndian\t:0x%x", i)
	i = binary.LittleEndian.Uint32(d)
	log.Printf("LittleEndian:0x%x", i)

	i = 0x12345678
	var i8 uint8 = uint8(i)
	log.Printf("0x%x uint8:0x%x", i, i8)
	var i16 uint16 = uint16(i >> 16)
	log.Printf("0x%x high 16:0x%x", i, i16)
}
