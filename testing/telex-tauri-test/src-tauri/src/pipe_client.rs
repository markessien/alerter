use std::time::Duration;
use tokio::net::windows::named_pipe::ClientOptions;
use tokio::time;
use windows_sys::Win32::Foundation::ERROR_PIPE_BUSY;
use tokio::io::AsyncWriteExt;

use tauri::AppHandle;
use tauri_plugin_dialog::DialogExt;

const PIPE_NAME: &str = r"\\.\pipe\telex-notifications-c2s1";

pub async fn send_notification(
    app: AppHandle,
    message: &str,
    sender_name: &str,
    channel: &str,
    icon_path: &str,
    timestamp: &str,
) -> Result<(), Box<dyn std::error::Error>> {

    app.dialog()
        .message("Entered pipe send_notification")
        .show(|_| {});


    let client = loop {
        match ClientOptions::new().write(true).open(PIPE_NAME) {
            Ok(client) => {
                app.dialog()
                    .message("Connected to named pipe")
                    .show(|_| {});
                break client;
            }
            Err(e) if e.raw_os_error() == Some(ERROR_PIPE_BUSY as i32) => (),
            Err(e) => {
                app.dialog()
                    .message(format!("xFailed to connect to named pipe: {}", e))
                    .show(|_| {});
                return Err(e.into());
            }
        }

        time::sleep(Duration::from_millis(50)).await;
    };

    let mut client = client;

    let payload = serde_json::json!({
        "message": message,
        "senderName": sender_name,
        "channel": channel,
        "iconPath": icon_path,
        "timestamp": timestamp,
    });

    let payload_str = payload.to_string();
    let bytes = payload_str.as_bytes();

    match client.write_all(bytes).await {
        Ok(_) => {
            app.dialog().message("Write to pipe successful").show(|_| {});
            Ok(())
        }
        Err(e) => {
            app.dialog()
                .message(format!("Write to pipe failed: {}", e))
                .show(|_| {});
            Err(e.into())
        }
    }
}