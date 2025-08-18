fn mainold() {
    tauri_build::build()
}

fn main() {
    tauri_build::try_build(
        tauri_build::Attributes::new().app_manifest(
            tauri_build::AppManifest::new()
                // This is where you declare your custom commands
                .commands(&["greet"]),
        ),
    )
    .unwrap();
}