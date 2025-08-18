import { useState } from "react";
import reactLogo from "./assets/react.svg";
import { invoke } from "@tauri-apps/api/core";
import "./App.css";

function App() {
  const [greetMsg, setGreetMsg] = useState("");
  const [name, setName] = useState("");

  async function greet() {
    // Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
    setGreetMsg(await invoke("greet", { name }));
  }

  async function sendNotification() {
    console.log("Sending notification...");
    try {
      await invoke("send_telex_notification", {
        sender: "React",
        message: "Hello from React!",
      });
      console.log("Notification sent successfully.");
    } catch (error) {
      console.error("Error sending notification:", error);
    }
  }

  async function sendNotificationToPipe() {
    console.log("Sending notification to pipe...");
    try {
      await invoke("send_notification_to_pipe", {
        payload: {
          message: "Hello from React via named pipe!",
          sender_name: "React Pipe Client",
          channel: "Pipe",
          icon_path: "path/to/icon.png",
          timestamp: new Date().toISOString(),
        },
      });
      console.log("Notification sent successfully to pipe.");
    } catch (error) {
      console.error("Error sending notification to pipe:", error);
    }
  }

  async function startPipeServer() {
    console.log("Starting pipe server...");
    try {
      await invoke("start_pipe_server");
      console.log("Pipe server started successfully.");
    } catch (error) {
      console.error("Error starting pipe server:", error);
    }
  }

  async function sendMessageToWin32Pipe() {
    console.log("Sending message to Win32 pipe...");
    try {
      const response = await invoke("send_message_to_pipe", {
        message: "Hello from React via Win32 pipe!",
      });
      console.log("Response from pipe:", response);
    } catch (error) {
      console.error("Error sending message to Win32 pipe:", error);
    }
  }

  return (
    <main className="container">
      <h1>Welcome to Tauri + React</h1>

      <div className="row">
        <a href="https://vitejs.dev" target="_blank">
          <img src="/vite.svg" className="logo vite" alt="Vite logo" />
        </a>
        <a href="https://tauri.app" target="_blank">
          <img src="/tauri.svg" className="logo tauri" alt="Tauri logo" />
        </a>
        <a href="https://reactjs.org" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <p>Click on the Tauri, Vite, and React logos to learn more.</p>

      <form
        className="row"
        onSubmit={(e) => {
          e.preventDefault();
          greet();
        }}
      >
        <input
          id="greet-input"
          onChange={(e) => setName(e.currentTarget.value)}
          placeholder="Enter a name..."
        />
        <button type="submit">Greet</button>
      </form>
      <p>{greetMsg}</p>

      <div className="row">
        <button onClick={sendNotification}>Send Notification</button>
        <button onClick={sendNotificationToPipe}>Send Notification to Pipe</button>
        <button onClick={startPipeServer}>Start Win32 Pipe Server</button>
        <button onClick={sendMessageToWin32Pipe}>Send Message to Win32 Pipe</button>
      </div>
    </main>
  );
}

export default App;
