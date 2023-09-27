package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	MAX_SERVERS  = 2
	MAX_CLIENTS  = 20
	MAX_SERVICES = 50
)

type Client struct {
	id int
}

type Server struct {
	id int
}

type Middleman struct {
	max_clients int
	max_servers int
	num_servers int
	left_server chan *Server
	stay_server chan *Server
}

type Queue[T any] struct {
	data []T
}

func NewQueue[T any]() Queue[T] {
	return Queue[T]{}
}

func (q *Queue[T]) Pop() (T, error) {
	if len(q.data) == 0 {
		var noop T
		return noop, fmt.Errorf("Empty pop")
	}
	val := q.data[0]
	q.data = q.data[1:]
	return val, nil
}

func (q *Queue[T]) IsEmpty() bool {
	return len(q.data) == 0
}

func (q *Queue[T]) Push(val T) {
	q.data = append(q.data, val)
}

func (q *Queue[T]) len() int {
	return len(q.data)
}

func clientStreamer() (chan *Client, func()) {
	rng := rand.New(rand.NewSource(time.Hour.Nanoseconds() + 82))
	cchan := make(chan *Client)
	done := make(chan struct{})
	cancel := func() {
		close(done)
	}
	go func() {
		for {
			select {
			case <-done:
				fmt.Println("Client streamer says fare the well")
				return
			case cchan <- &Client{id: rng.Int()}:
				time.Sleep(time.Microsecond)
			}
		}
	}()
	return cchan, cancel
}

func serverStreamer() (chan *Server, func()) {
	rng := rand.New(rand.NewSource(time.Hour.Nanoseconds() + 42))
	schan := make(chan *Server)
	done := make(chan struct{})
	cancel := func() {
		close(done)
	}
	go func() {
		for {
			select {
			case <-done:
				fmt.Println("Server streamer says fare the well")
				return
			case schan <- &Server{id: rng.Int()}:
				time.Sleep(time.Microsecond * 10)
			}
		}
	}()
	return schan, cancel
}

func (m *Middleman) pair(server *Server, client *Client, done chan struct{}) {
	fmt.Printf("server %d served client %d\n", server.id, client.id)
	time.Sleep(time.Millisecond * 100)
	los := (server.id+client.id)%2 == 0
	los = false
	if los {
		for {
			select {
			case <-done:
				fmt.Printf("Server %d says bye bye\n", server.id)
				return
			case m.left_server <- server:
			}
		}
	} else {
		fmt.Printf("Server %d is staying\n", server.id)
		for {
			select {
			case <-done:
				fmt.Printf("Server %d says bye bye\n", server.id)
				return
			case m.stay_server <- server:
			}
		}
	}
}

func (m *Middleman) serve(wg *sync.WaitGroup) {
	defer wg.Done()
	var cnt int
	idle_servers := Queue[*Server]{}
	waiting_client := Queue[*Client]{}
	m.stay_server = make(chan *Server)
	m.left_server = make(chan *Server)
	cchan, ccancel := clientStreamer()
	schan, scancel := serverStreamer()
	done := make(chan struct{})

	mupdate := func() {
		cnt++
		if cnt >= MAX_SERVICES {
			close(done)
			ccancel()
			scancel()
		}
	}

	for {
		select {
		case <-done:
			fmt.Println("Middleman done service!")
			return
		case client := <-cchan:
			if idle_servers.IsEmpty() {
				if waiting_client.len() < m.max_clients {
					waiting_client.Push(client)
				} else {
					fmt.Printf("Client %d is denied\n", client.id)
				}
			} else {
				server, _ := idle_servers.Pop()
				fmt.Printf("Server %d is awoken by client %d\n", server.id, client.id)
				go m.pair(server, client, done)
				mupdate()
			}
		case server := <-schan:
			if m.num_servers < m.max_servers {
				fmt.Printf("Server %d is joining\n", server.id)
				m.num_servers++
				if !waiting_client.IsEmpty() {
					client, _ := waiting_client.Pop()
					go m.pair(server, client, done)
					mupdate()
				} else {
					idle_servers.Push(server)
				}
			} else {
				fmt.Printf("Server %d is denied\n", server.id)
			}
		case server := <-m.left_server:
			m.num_servers--
			fmt.Printf("Server %d is leaving\n", server.id)
		case server := <-m.stay_server:
			if waiting_client.len() > 0 {
				client, _ := waiting_client.Pop()
				fmt.Printf("Server %d picks client %d\n", server.id, client.id)
				go m.pair(server, client, done)
				mupdate()
			} else {
				fmt.Printf("Server %d is idle\n", server.id)
				idle_servers.Push(server)
			}
		default:
			if !idle_servers.IsEmpty() && !waiting_client.IsEmpty() {
				server, _ := idle_servers.Pop()
				client, _ := waiting_client.Pop()
				go m.pair(server, client, done)
				mupdate()
			}
			time.Sleep(time.Microsecond * 100)
		}
	}
}

func main() {
	m := &Middleman{max_clients: MAX_CLIENTS, max_servers: MAX_SERVERS, num_servers: 0}
	wg := sync.WaitGroup{}
	wg.Add(1)
	go m.serve(&wg)
	wg.Wait()
}
