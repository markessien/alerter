use tauri::AppHandle;
use tauri_plugin_dialog::DialogExt;

const HTTP_ENDPOINT: &str = "http://127.0.0.1:27810";

pub async fn send_notification(
    app: AppHandle,
    message: &str,
    sender_name: &str,
    channel: &str,
    icon_path: &str,
    timestamp: &str,
) -> Result<(), Box<dyn std::error::Error>> {
    app.dialog()
        .message("Entered http send_notification")
        .show(|_| {});

    let payload = serde_json::json!({
        "message": message,
        "senderName": sender_name,
        "channel": channel,
        "iconPath": icon_path,
        "timestamp": timestamp,
        "type": "notification"
    });

    let json_payload = payload.to_string();
    let payload_length = json_payload.as_bytes().len();
    let length_string = format!("{:08}", payload_length);
    let body = format!("+TELEX+:[{}]+{}", length_string, json_payload);

    let client = reqwest::Client::new();
    let res = client.post(HTTP_ENDPOINT)
        .body(body)
        .send()
        .await;

    match res {
        Ok(response) => {
            if response.status().is_success() {
                app.dialog().message("Write to http successful").show(|_| {});
                Ok(())
            } else {
                let error_message = format!("HTTP request failed with status: {}", response.status());
                app.dialog().message(&error_message).show(|_| {});
                Err(error_message.into())
            }
        }
        Err(e) => {
            app.dialog()
                .message(format!("Write to http failed: {}", e))
                .show(|_| {});
            Err(e.into())
        }
    }
}