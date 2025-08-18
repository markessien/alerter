// src/lib.rs
use tauri::{Manager, Listener};

#[tauri::command]
fn greet(name: String) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}


pub fn run() {
    tauri::Builder::default()
        .setup(|app| {
            // The event listener is the "Rust function" that will be entered.
            app.listen("ipc-router", |_event| {
                // This message will appear in the terminal where you ran `cargo tauri dev`.
                println!("Received a broadcast on the 'ipc-router' channel!");
                println!("Payload: {:?}", _event.payload());
            });
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}