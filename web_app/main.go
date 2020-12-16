package main

import (
    "fmt"
    "log"
    "net/http"
    "github.com/gorilla/websocket"
    "time"
    "io/ioutil"
    "os/exec"
    "strings"
)

func main() {
    go init_websocket()
    init_staticwebpage()
    //TODO:
    //wait for all goroutines to close
}

func init_staticwebpage() {
    fmt.Println("Hello, web_app!")
    http.Handle("/", http.FileServer(http.Dir("./static_site")))

    fmt.Println("Listening on :8080")
    err := http.ListenAndServe(":8080", nil)
    if err != nil {
        log.Fatal(err)
        return
    }
}

var upgrader = websocket.Upgrader{
    ReadBufferSize: 1024,
    WriteBufferSize: 1024,
}

func init_websocket(){
    fmt.Println("websocket on port :3000")
    http.HandleFunc("/ws", ws_endpoint)
    log.Fatal(http.ListenAndServe(":3000", nil))
}

func reader(conn *websocket.Conn){
    for {
        messageType, client_message, err := conn.ReadMessage()
        if err != nil {
            log.Println(err)
            return
        }else if len(client_message) > 0 && client_message[0] == 'k'{
            log.Println(string(client_message))
            cmd := exec.Command("kill", strings.Split(string(client_message), ":")[1])

            err := cmd.Run()
            if err != nil {
                log.Fatal(err)
            }
        }

        //@BAD: we just write everything we could ever need in a json if the client is only interested
        //in a part of that info
        cpu_module_content, err := ioutil.ReadFile("/proc/cpu_201612383")
        if err != nil {
            log.Fatal(err)
            return
        }
        if len(cpu_module_content) < 2 {
            log.Fatal("cpu_201612383 esta vacio!")
            return
        }
        
        memo_module_content, err := ioutil.ReadFile("/proc/memo_201612383")
        if err != nil {
            log.Fatal(err)
            return
        }
        if len(memo_module_content) < 2 {
            log.Fatal("memo_201612383 esta vacio!")
            return
        }

        //@BADish perf: inefficient way of concatenating the proc files we dont really need a third buffer
        server_message := append(cpu_module_content[0:len(cpu_module_content)-2], memo_module_content[1:]...)

        //@debug:
        //log.Printf("sending: %v",string(server_message))

        if err := conn.WriteMessage(messageType, []byte(string(server_message))); err != nil {
            log.Fatal(err)
            return
        }

        time.Sleep(1 * time.Second)
    }
}

func ws_endpoint(w http.ResponseWriter, r *http.Request){
    upgrader.CheckOrigin = func(r *http.Request) bool { return true }

    ws, err := upgrader.Upgrade(w, r, nil)

    if err != nil {
        log.Println(err)
    }

    log.Println("Client successfully connected...")

    reader(ws);
}

