// In the remote site's JS


document.addEventListener('DOMContentLoaded', function() {

    const messageContainer = document.getElementById('js-message');

    // Set an initial message while waiting for Tauri's response
    if (messageContainer) {
        messageContainer.textContent = 'Attempting to contact the Tauri app...';
    }

    // --- ADDITIONS START HERE ---

    // 1. Check if the webpage is running inside another window (like Tauri).
    //    This prevents errors if someone visits the site in a normal browser.
    if (window.parent !== window) {
        // 2. Send a message to the parent window (the Tauri app).
        //    The preload script in Tauri will be listening for this.
        window.parent.postMessage(
            {
                command: 'callSecret', // The command the preload script looks for
                payload: 'your remote website' // The data to send to the Rust function
            },
            '*' // Use a wildcard origin; security is handled by the preload script
        );
    } else if (messageContainer) {
        messageContainer.textContent = 'This page is not running inside a Tauri window.';
    }

    // 3. Listen for the response message coming back from the Tauri preload script.
    window.addEventListener('message', (event) => {
        const response = event.data;

        if (messageContainer && response) {
            // Check if the response object contains a 'result' (on success)
            if (response.result) {
                messageContainer.textContent = `✅ Success! Tauri says: "${response.result}"`;
            } 
            // Check if the response object contains an 'error'
            else if (response.error) {
                messageContainer.textContent = `❌ Error from Tauri: "${response.error}"`;
            }
        }
    });

    // --- ADDITIONS END HERE ---
});