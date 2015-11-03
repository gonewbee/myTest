package main

import (
	"bufio"
	"bytes"
	"encoding/base64"
	"image"
	"image/color"
	"image/png"
	"log"
	"os"
)

func main() {
	w := 16
	h := 16
	img := image.NewRGBA(image.Rect(0, 0, w, h))
	col := color.RGBA{255, 0, 0, 255}
	for x := 0; x < w; x++ {
		if x > 8 {
			col = color.RGBA{0, 0, 255, 255}
		}
		for y := 0; y < h; y++ {
			img.Set(x, y, col)
		}
	}

	// 通过bytes创建writer
	// http://stackoverflow.com/questions/23454940/getting-bytes-buffer-does-not-implement-io-writer-error-message
	var b bytes.Buffer
	foo := bufio.NewWriter(&b)
	png.Encode(foo, img)
	foo.Flush()
	log.Println(b)
	data := b.Bytes()
	// 使用base64编码后，将str复制到浏览器中可以直接显示
	str := base64.StdEncoding.EncodeToString(data)
	str = "data:image/png;base64," + str
	log.Println(str)

	f, err := os.Create("test.png")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	png.Encode(f, img)
}
