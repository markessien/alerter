#include "icons.h"

// Helper function to create a wxBitmap from a base64 string
wxBitmap BitmapFromBase64(const char* data) {
    wxString s(data);
    wxMemoryBuffer buffer = wxBase64Decode(s);
    if (buffer.GetDataLen() == 0) return wxBitmap();
    wxMemoryInputStream stream(buffer.GetData(), buffer.GetDataLen());
    return wxBitmap(wxImage(stream));
}