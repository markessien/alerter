use crate::pipe_client;
use serde::Deserialize;

#[derive(Deserialize)]
pub struct NotificationPayload {
    message: String,
    sender_name: String,
    channel: String,
    icon_path: String,
    timestamp: String,
}

use tauri::AppHandle;
use tauri_plugin_dialog::DialogExt;

#[tauri::command]
pub async fn send_notification_to_pipe(
    app: AppHandle,
    payload: NotificationPayload,
) -> Result<(), String> {
    app.dialog()
        .message("Entered send_notification_to_pipe")
        .show(|_| {});

    if let Err(e) = pipe_client::send_notification(
        app,
        &payload.message,
        &payload.sender_name,
        &payload.channel,
        &payload.icon_path,
        &payload.timestamp,
    )
    .await
    {
        return Err(e.to_string());
    }

    Ok(())
}