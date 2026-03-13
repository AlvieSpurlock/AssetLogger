// Copyright © Mad Tea Party Development 2025
// Proprietary software — see license.
// Demo version provided for evaluation only. Full version available at [Link To Be Added].

#include "AssetLoggerApp.h"
#include "AssetLoggerFrame.h"
#include <wx/msgdlg.h>

wxIMPLEMENT_APP(AssetLoggerApp);

bool AssetLoggerApp::OnInit()
{
    // ── Demo-mode toggle ──────────────────────────────────────────────────
    // Set to `true` to run as demo (hides Location / Name / License fields).
    constexpr bool DEMO_MODE = false;
    // ─────────────────────────────────────────────────────────────────────

    SetAppName("Asset Logger");
    SetAppDisplayName("Asset Logger — Mad Tea Party Development");

    AssetLoggerFrame* frame = new AssetLoggerFrame(DEMO_MODE);
    frame->Show(true);
    return true;
}
