package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"strings"
	// "time"
)

type Authenticate_info struct {
	Result  bool   `json:"result"`
	Message string `json:"message"`
}

func get_test() {
	url := "http://127.0.0.1:19080/validate.php"
	resp, err := http.Get(url)
	if err != nil {
		log.Fatal(err)
		return
	}
	defer resp.Body.Close()
	result, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatal(err)
		return
	}
	log.Printf("%s", result)
}

func post_test() {
	url := "http://192.168.7.82/portal.jpad/authenticate.php"
	log.Println("URL:", url)
	// server=192.168.7.82&macSerial=0081050129EA&login=zsy&password=lTXhvSj4mzk%3D&version=test_160511&mobile=0
	var jsonStr = []byte(`{"server":"192.168.7.82",
		"macSerial":"0081050129EA",
		"login":"zsy",
		"password":"lTXhvSj4mzk%3D",
		"version":"test_160511",
		"mobile":"0"}`)
	req, err := http.NewRequest("POST", url, bytes.NewBuffer(jsonStr))
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()

	fmt.Println("response Status:", resp.Status)
	fmt.Println("response Headers:", resp.Header)
	body, _ := ioutil.ReadAll(resp.Body)
	fmt.Println("response Body:", string(body))
}

func get_test_proxy() {
	get_url := "http://192.168.7.82/validate.php"
	proxyUrl, err := url.Parse("http://127.0.0.1:8888")
	httpClient := &http.Client{Transport: &http.Transport{Proxy: http.ProxyURL(proxyUrl)}}
	response, err := httpClient.Get(get_url)
	if err != nil {
		fmt.Println(err.Error())
	} else {
		body, _ := ioutil.ReadAll(response.Body)
		fmt.Println("OK: ", len(body))
		fmt.Println(string(body))
	}
}

func post_test_proxy() {
	post_url := "http://192.168.7.82/portal.jpad/authenticate.php"
	req, err := http.NewRequest("POST", post_url, strings.NewReader("server=192.168.7.82&macSerial=0081050129EA&login=zsy&password=lTXhvSj4mzk%3D&version=test_160511&mobile=0"))
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	proxyUrl, err := url.Parse("http://127.0.0.1:8888")
	httpClient := &http.Client{Transport: &http.Transport{Proxy: http.ProxyURL(proxyUrl)}}
	resp, err := httpClient.Do(req)
	if err != nil {
		panic(err)
	}
	defer resp.Body.Close()

	fmt.Println("response Status:", resp.Status)
	fmt.Println("response Headers:", resp.Header)
	// body, _ := ioutil.ReadAll(resp.Body)
	// fmt.Println("response Body:", string(body))
	// fmt.Println("\u554a")
	// json
	decoder := json.NewDecoder(resp.Body)
	var data Authenticate_info
	err = decoder.Decode(&data)
	if err != nil {
		log.Fatal(err)
		return
	}
	fmt.Println(data.Message)
}

func main() {
	get_test()
	// post_test()
	// get_test_proxy()
	// for i := 1; i <= 10; i++ {
	// 	time.Sleep(time.Second * 2)
	// 	post_test_proxy()
	// }
}
