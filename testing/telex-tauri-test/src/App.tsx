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

  async function sendNotificationToHttp() {
    console.log("Sending notification to http...");
    try {
      await invoke("send_notification_to_http", {
        payload: {
          message: "Hello from React via http!",
          sender_name: "React http Client",
          channel: "http",
          icon_path: "path/to/icon.png",
          timestamp: new Date().toISOString(),
        },
      });
      console.log("Notification sent successfully to http.");
    } catch (error) {
      console.error("Error sending notification to http:", error);
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
        <button onClick={sendNotificationToHttp}>Send Notification to Http</button>
      </div>
    </main>
  );
}

export default App;
