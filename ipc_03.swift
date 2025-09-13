

import Foundation

// MARK: - Config

let fifoPath = "/tmp/swift_ipc_pipe"

// MARK: - Setup

func createFIFO(at path: String) {
    if FileManager.default.fileExists(atPath: path) {
        unlink(path)
    }
    
    if mkfifo(path, 0o644) != 0 {
        perror("mkfifo")
        exit(1)
    }
}

// MARK: - IPC

func listenOnPipe(at path: String) {
    print("[IPC] Listening on pipe: \(path)")
    
    guard let file = fopen(path, "r") else {
        perror("fopen")
        exit(1)
    }

    defer {
        fclose(file)
    }

    let bufferSize = 1024
    var buffer = [CChar](repeating: 0, count: bufferSize)

    while fgets(&buffer, Int32(bufferSize), file) != nil {
        let message = String(cString: buffer).trimmingCharacters(in: .whitespacesAndNewlines)
        print("[IPC] Received: \(message)")
    }
}

// MARK: - Entry Point

func main() {
    print("🚀 Swift IPC App Starting...")
    
    createFIFO(at: fifoPath)
    listenOnPipe(at: fifoPath)
    
    print("👋 Exiting.")
}

main()

