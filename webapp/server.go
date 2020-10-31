package main

import (
	"flag"
	"io/ioutil"
	"log"
	"net/http"
)

func main() {
	port := flag.String("p", "8000", "port to serve on")
	directory := flag.String("d", ".", "the directory of static file to host")
	flag.Parse()

	http.HandleFunc("/api", func(w http.ResponseWriter, r *http.Request) {
		resp, err := http.Get("http://192.168.1.54")
		if err != nil {
			w.WriteHeader(500)
			w.Write([]byte(err.Error()))
			log.Print(err.Error())
			return
		}
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			w.WriteHeader(500)
			w.Write([]byte(err.Error()))
			log.Print(err.Error())
			return
		}
		w.WriteHeader(200)
		w.Write(body);
	})
	http.Handle("/", http.FileServer(http.Dir(*directory)))

	log.Printf("Serving %s on HTTP port: %s\n", *directory, *port)
	log.Fatal(http.ListenAndServe(":"+*port, nil))
}