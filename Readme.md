# 🤝 P2P_CHAT_APP

[![C/C++](https://img.shields.io/badge/language-C-blue.svg)](https://isocpp.org/)

A lightweight, terminal-based peer-to-peer chat application in C, built using BSD sockets and `select()` for multiplexed I/O. No central server required—each peer can act as client or server, automatically negotiating connection roles.

---

## 🚀 Features

* **True P2P** – Each peer tries to connect first; if that fails, it falls back to listening mode.
* **Full-Duplex Chat** – Simultaneous send/receive using `select()` across stdin and the socket.
* **Graceful Exit** – Type `exit` to close connection on either side.
* **Single-file Programs** – Two simple executables (`peer1` and `peer2`), each \~200 lines of code.
* **No External Dependencies** – Only standard POSIX sockets libraries (`arpa/inet.h`, `sys/socket.h`, etc.).

---

## 📁 Repository Structure

```
.
├── peer1.c         # Listens on port 8080, connects to 127.0.0.1:8081
├── peer2.c         # Listens on port 8081, connects to 127.0.0.1:8080
└── README.md       # (You are here!)
```

---

## 🔧 Build & Run

1. **Clone the repo**

   ```bash
   git clone https://github.com/Vikas2171/P2P_CHAT_APP.git
   cd P2P_CHAT_APP
   ```

2. **Compile**

   ```bash
   gcc peer1.c -o peer1
   gcc peer2.c -o peer2
   ```

3. **Launch two terminals**

   * In **Terminal A**, start `peer1`:

     ```bash
     ./peer1
     ```
   * In **Terminal B**, start `peer2`:

     ```bash
     ./peer2
     ```

   One of them will print:

   ```
   Connected to peer...
   ```

   and the other:

   ```
   Waiting for peer connection...
   Peer connected...
   ```

4. **Chat away!**

   * Type messages in either terminal and hit **Enter**.
   * Type `exit` to terminate the session.

---

## 💡 How It Works

1. **Connection negotiation**

   * Each program first calls `try_connect()` to dial the other peer.
   * If the connection fails, it switches to server mode with `start_server()`.
2. **I/O Multiplexing**

   * Uses `select()` to watch both **stdin** and the **socket**.
   * Allows non-blocking, full-duplex chat without threads.
3. **Shutdown**

   * If either side sends `"exit"`, both peers gracefully close the socket and exit.

---

## 🌟 Example Session

```text
Peer1 ▶ Waiting for peer connection...
Peer2 ▶ Connected to peer...

Peer1 ▶ From peer: Hello there!
Peer1 ▶ Hi! How are you?
Peer2 ▶ From peer: Hi! How are you?
Peer2 ▶ I'm great—testing P2P chat!
Peer1 ▶ From peer: I'm great—testing P2P chat!
Peer1 ▶ exit
Peer1 ▶ Exiting chat...
Peer2 ▶ From peer: exit
Peer2 ▶ Exiting chat...
```

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

1. Fork the repo
2. Create a feature branch (`git checkout -b feature/YourFeature`)
3. Commit your changes (`git commit -m "Add YourFeature"`)
4. Push to your branch (`git push origin feature/YourFeature`)
5. Open a Pull Request

Please adhere to the existing code style and include comments where needed.

---