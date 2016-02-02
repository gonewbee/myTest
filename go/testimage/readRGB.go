package main

import (
	"image"
	"image/color"
	"image/png"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	w := 1366
	h := 768
	filename := "data.tmp"
	img := image.NewRGBA(image.Rect(0, 0, w, h))
	b, err := ioutil.ReadFile(filename)
	if err != nil {
		panic(err)
	}
	log.Printf("%x %x %x %x", b[0], b[1], b[2], b[3])
	n := 0
	col := color.RGBA{255, 0, 0, 255}
	for y := h - 1; y >= 0; y-- {
		for x := 0; x < w; x++ {
			col = color.RGBA{b[n+2], b[n+1], b[n+0], 255}
			n += 4
			img.Set(x, y, col)
		}
	}
	f, err := os.Create("draw.png")
	if err != nil {
		panic(err)
	}
	defer f.Close()
	png.Encode(f, img)
}
