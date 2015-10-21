package main

import (
	"bufio"
	// "golang.org/x/image/bmp"
	"image/jpeg"
	"image/png"
	"log"
	"os"
)

func main() {
	// fi, err := os.Open("test.bmp")
	fi, err := os.Open("test.jpg")
	defer fi.Close()
	if err != nil {
		panic(err)
	}
	r := bufio.NewReader(fi)
	// image, err := bmp.Decode(r)
	image, err := jpeg.Decode(r)
	fo, err := os.Create("test.png")
	defer fo.Close()
	if err != nil {
		panic(err)
	}
	w := bufio.NewWriter(fo)
	err = png.Encode(w, image)
	if err != nil {
		panic(err)
	}
	log.Println("success")
}
