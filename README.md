# NetWatch - Professional Network Traffic Monitor

## Overview

NetWatch is a high-performance network traffic monitoring application developed in C++ using Boost.Asio. It captures and analyzes TCP and UDP packets, displaying metadata such as source/destination IPs, ports, protocols, and packet sizes. Designed with scalability and modularity in mind, it serves as a real-world project simulating enterprise-grade network visibility tools.

## Real-world Use Case

NetWatch was developed as a proof-of-concept for firm. The firm needed a lightweight, portable tool to monitor internal network activity for performance analytics and early threat detection in small to medium-sized corporate networks.

---

## Technologies & Justifications

### Programming Language

* **C++**: Chosen for performance and low-level network access capabilities. Suitable for real-time packet processing.

### Networking

* **Boost.Asio**: Provides asynchronous TCP and UDP socket handling, highly performant and scalable.

### Multithreading

* **std::thread / Boost.Thread**: Handles concurrent packet capturing and parsing efficiently.

### Parsing

* **Custom packet header parsers**: Designed for extensibility and accurate dissection of IPv4 headers, TCP, and UDP segments.

### Persistence Layer

* **SQLite (default)**: Embedded and lightweight database for local storage of packet metadata.
* **PostgreSQL (optional)**: For enterprise deployment scenarios requiring remote querying and better scalability.
* **TimescaleDB (optional)**: Extension of PostgreSQL used if time-series analysis of packets over time is needed.

#### Why SQLite?

* Easy integration, zero setup, perfect for portable deployments.

#### Why PostgreSQL?

* Supports complex querying, user access management, and scalability.

#### Why TimescaleDB?

* Excellent fit for time-series data like packet traffic monitoring with built-in compression and retention policies.

---

## Project Structure

```
NetWatch/
│
├── src/                       # Core application logic
│   ├── main.cpp               # Entry point
│   ├── network/               # TCP/UDP handling
│   ├── parser/                # Packet header parsers
│   ├── storage/               # Database interaction layer
│   ├── config/                # Config loader
│   └── utils/                 # Logging, helpers
│
├── diagrams/                 # Architecture and usage diagrams
├── tests/                    # Unit and integration tests
├── docs/                     # Documentation and API references
├── CMakeLists.txt            # Build system
└── README.md                 # Project documentation
```

---

## Diagrams (to be included in `/diagrams`)

### 1. Use Case Diagram

* Illustrates user interactions (Network Admins, Security Auditors).

### 2. Sequence Diagram

* Packet capture -> Parse -> Persist -> Display

### 3. Class Diagram

* Modules like PacketSniffer, TCPHandler, UDPHandler, DatabaseAdapter.

### 4. Deployment Diagram

* Client machine with NetWatch agent, PostgreSQL server, and remote dashboard.

### 5. ER Diagram (Database)

* Tables: `packets`, `ip_addresses`, `logs`, `alerts`

---

## Architectural Pattern

### Hexagonal Architecture (Ports and Adapters)

* **Core**: Packet processing, protocol parsing, and rule engines
* **Ports**: Abstract interfaces (e.g., IDataStorage)
* **Adapters**: SQLite/PostgreSQL adapters, CLI UI adapter

**Benefits**:

* Clear separation of concerns
* Improved testability and modularity
* Easier to replace infrastructure components

---

## API Design (Optional)

If a REST interface is required:

* **C++ REST SDK (Casablanca)**
* Endpoint examples:

  * `GET /api/packets`
  * `GET /api/stats`
  * `POST /api/config`

---

## Testing

* **Unit tests**: Catch2 framework
* **Integration tests**: Simulated network traffic scenarios

---

## Build & Run

```sh
mkdir build && cd build
cmake ..
make
./NetWatch
```

---

## Future Enhancements

* Web UI using React + REST API
* TLS decryption (for lawful inspection)
* AI-based anomaly detection

---

## License

MIT License

---

## Authors

* Victor Carlota (Lead Developer)

---

## Contributions

Feel free to fork and submit PRs for improvements and new features!
