package main

import (
	"fmt"
	"net/http"
	"testing"
)

func main() {
	fmt.Println("Hello, web_app!")
	http.HandleFunc(":8080", func(writter http.ResponseWriter, request *http.Request) {

	})
	//wait for all goroutines to close
}

func Addition(a int, b int) int {
	return a + b
}

func TestAddition(t *testing.T) {
	if Addition(2, 2) != 4 {
		t.Error("2 + 2 should be 4")
	}

}
