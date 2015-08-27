package main

import "log"
import "net/http"
import "io"

func HelloServer(w http.ResponseWriter, req *http.Request) {
	log.Println("HelloServer enter")
	io.WriteString(w, "Hello, World! Docker update!!\n")
}

func main() {
	port := "5000"
	http.HandleFunc("/", HelloServer)
	log.Println("Server started: http://localhost:" + port)
	log.Fatal(http.ListenAndServe(":"+port, nil))
}
