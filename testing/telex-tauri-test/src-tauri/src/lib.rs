mod notification_client;
mod pipe_client;
use tauri_plugin_dialog::DialogExt;

// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
fn send_telex_notification(app: tauri::AppHandle, sender: &str, message: &str) {

    // hardco. Set sender and mssage
    let sender = if sender.is_empty() { "Unknown" } else { sender };
    let message = if message.is_empty() { "No message provided" } else { message };
    
    let msg = format!("Message from {}: {}", sender, message);
    app.dialog()
        .message(msg)
        .title("Telex Notification")
        .show(|_| {});
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .plugin(tauri_plugin_dialog::init())
        .invoke_handler(tauri::generate_handler![
            greet,
            send_telex_notification,
            notification_client::send_notification_to_pipe
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
