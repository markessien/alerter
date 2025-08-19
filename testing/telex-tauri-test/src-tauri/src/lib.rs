// src/lib.rs
use tauri::{Manager, Listener};

mod http_client;

#[tauri::command]
fn greet(name: String) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}


pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_dialog::init())
        .setup(|app| {
            let app_handle = app.handle().clone();
            // The event listener is the "Rust function" that will be entered.
            app.listen("ipc-router", move |_event| {
                // This message will appear in the terminal where you ran `cargo tauri dev`.
                println!("Received a broadcast on the 'ipc-router' channel!");
                println!("Payload: {:?}", _event.payload());

                let app_handle_clone = app_handle.clone();
                tauri::async_runtime::spawn(async move {
                    if let Err(e) = http_client::send_notification(
                        app_handle_clone,
                        "dummy message",
                        "dummy sender",
                        "dummy channel",
                        "dummy/icon/path",
                        "dummy_timestamp",
                    )
                    .await
                    {
                        eprintln!("Error sending notification: {}", e);
                    }
                });
            });
            Ok(())
        })
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}