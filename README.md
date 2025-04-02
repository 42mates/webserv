# Webserv

Webserv is a lightweight HTTP server implemented in C++ as part of the 42 school curriculum. It supports multiple features such as configuration file parsing, non-blocking socket operations, CGI script handling, and robust request/response management.

---

## Features

### 1. Configuration File Management
- The server is configured using a custom configuration file (`webserv.conf`), which allows defining:
  - Server blocks with specific settings (e.g., `root`, `methods`, `index_file`, etc.).
  - Route-specific configurations like `upload_dir`, `cgi_path`, and `http_redirect`.
  - HTTP methods allowed (`GET`, `POST`, `DELETE`, etc.).
- The configuration file is parsed into structured objects (`ServerConfig`, `RouteConfig`) for easy access during runtime.
- Errors in the configuration file (e.g., unknown directives, invalid arguments) are detected and reported.

### 2. Non-Blocking Sockets and Event Monitoring
- Webserv uses non-blocking sockets to handle multiple connections simultaneously.
- The `poll()` system call is used to monitor events on listening sockets and client connections.
- Based on the events (e.g., `POLLIN`, `POLLOUT`), the server performs actions such as:
  - Accepting new connections.
  - Reading incoming requests.
  - Sending responses to clients.
- This architecture ensures high performance and scalability.

### 3. Request Parsing and Response Handling
- The server parses incoming HTTP requests, including:
  - Start line (method, URI, version).
  - Headers (e.g., `Host`, `Content-Type`).
  - Body (supports `chunked` and `multipart` encoding).
- Responses are generated with appropriate status codes (`200 OK`, `404 Not Found`, `500 Internal Server Error`, etc.).
- Error handling is robust, with detailed error messages for invalid requests or server-side issues.

### 4. CGI Script Management
- Webserv supports executing CGI scripts in python for dynamic content generation.
- CGI execution is managed with:
  - Environment variable initialization.
  - Timeout handling to prevent hanging scripts.
  - Proper error handling for script failures or invalid outputs.
- The server ensures that CGI scripts do not block other operations.
### 5. Stress-Tested Non-Blocking Mode
- Webserv operates in non-blocking mode, ensuring high availability and responsiveness under heavy load.
- The server successfully passed a stress test using `siege`:
	```
	Lifting the server siege...
	Transactions:                 121815 hits
	Availability:                 100.00 %
	Elapsed time:                  59.40 secs
	Data transferred:             182.51 MB
	Response time:                  0.00 secs
	Transaction rate:            2050.76 trans/sec
	Throughput:                     3.07 MB/sec
	Concurrency:                    1.46
	Successful transactions:      121815
	Failed transactions:               0
	Longest transaction:            1.04
	Shortest transaction:           0.00
	```
- This stress test can be replicated using the command:
	```bash
	make siege
	```
	Note: Ensure `siege` is installed as a dependency before running the test.

---

## How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/webserv.git
   cd webserv
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the server:
   ```bash
   ./webserv [config_file]
   ```
   If no configuration file is provided, the default `tools/config/webserv.conf` is used.

---

## Directory Structure
- **`src/`**: Contains the source code for the server, including request/response handling, configuration parsing, and CGI management.
- **`tools/website/`**: Static and dynamic content for testing the server.
- **`tools/config/`**: Default configuration file (`webserv.conf`).

---

## Authors
- [@marinsucks](https://github.com/marinsucks)
- [@sokaraku](https://github.com/sokaraku)
