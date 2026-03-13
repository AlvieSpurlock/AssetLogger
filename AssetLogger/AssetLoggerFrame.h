#pragma once
// Copyright © Mad Tea Party Development 2025
// Proprietary software - see license.

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/filedlg.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <vector>
#include <string>

// ─── Colour Palette ─────────────────────────────────────────────────────────
namespace Theme
{
    // Background layers
    const wxColour BG_DEEP   (0x0D, 0x0F, 0x14);   // near-black
    const wxColour BG_PANEL  (0x16, 0x19, 0x22);   // dark blue-grey
    const wxColour BG_CARD   (0x1F, 0x23, 0x2E);   // slightly lighter
    const wxColour BG_INPUT  (0x12, 0x15, 0x1C);   // input fields

    // Accents
    const wxColour ACCENT    (0xE8, 0xB4, 0x5A);   // warm gold
    const wxColour ACCENT2   (0xC8, 0x78, 0x2A);   // deeper amber
    const wxColour DANGER    (0xC0, 0x3F, 0x3F);   // muted red

    // Text
    const wxColour TEXT_PRI  (0xEA, 0xE8, 0xE0);   // warm white
    const wxColour TEXT_SEC  (0x7A, 0x7F, 0x96);   // muted blue-grey
    const wxColour TEXT_HEAD (0xE8, 0xB4, 0x5A);   // gold headings

    // Grid
    const wxColour GRID_LINE (0x2A, 0x2F, 0x3D);
    const wxColour GRID_SEL  (0x2C, 0x34, 0x4A);
}

// ─── Asset Record ────────────────────────────────────────────────────────────
struct AssetRecord
{
    wxString platform;
    wxString location;
    wxString use;
    wxString name;
    wxString maker;
    wxString license;
};

// ─── Main Frame ──────────────────────────────────────────────────────────────
class AssetLoggerFrame : public wxFrame
{
public:
    AssetLoggerFrame(bool demoMode);

private:
    bool m_demo;
    bool m_dirty = false;
    wxString m_currentFile;
    std::vector<AssetRecord> m_records;

    // Layout regions
    wxPanel*     m_sidebar   = nullptr;
    wxPanel*     m_mainArea  = nullptr;
    wxListCtrl*  m_grid      = nullptr;

    // Sidebar buttons
    wxButton* m_btnNew    = nullptr;
    wxButton* m_btnEdit   = nullptr;
    wxButton* m_btnDelete = nullptr;
    wxButton* m_btnOpen   = nullptr;
    wxButton* m_btnSave   = nullptr;
    wxButton* m_btnSaveAs = nullptr;

    // Status bar label
    wxStaticText* m_statusLabel = nullptr;

    // Build UI
    void BuildLayout();
    wxPanel* BuildSidebar(wxWindow* parent);
    wxPanel* BuildMainArea(wxWindow* parent);
    wxButton* MakeSideButton(wxWindow* parent, const wxString& label,
                             wxWindowID id, const wxColour& bg,
                             const wxColour& fg, bool bold = false);

    // Grid helpers
    void RebuildGrid();
    void RefreshRow(long row, const AssetRecord& rec);
    int  SelectedRow() const;

    // CRUD
    void OnNew(wxCommandEvent&);
    void OnEdit(wxCommandEvent&);
    void OnDelete(wxCommandEvent&);
    void OnOpen(wxCommandEvent&);
    void OnSave(wxCommandEvent&);
    void OnSaveAs(wxCommandEvent&);
    void OnGridActivate(wxListEvent&);
    void OnClose(wxCloseEvent&);

    // File I/O
    bool LoadCSV(const wxString& path);
    bool SaveCSV(const wxString& path);

    // Helpers
    void SetStatus(const wxString& msg, bool warn = false);
    void MarkDirty(bool dirty);
    bool ConfirmDiscard();

    wxDECLARE_EVENT_TABLE();
};

// ─── Asset Edit Dialog ───────────────────────────────────────────────────────
class AssetEditDialog : public wxDialog
{
public:
    AssetEditDialog(wxWindow* parent, bool demoMode,
                    const AssetRecord& rec, const wxString& title);
    AssetRecord GetRecord() const;

private:
    bool m_demo;

    wxTextCtrl* m_platform  = nullptr;
    wxTextCtrl* m_location  = nullptr;   // full only
    wxTextCtrl* m_use       = nullptr;
    wxTextCtrl* m_name      = nullptr;   // full only
    wxTextCtrl* m_maker     = nullptr;
    wxTextCtrl* m_license   = nullptr;   // full only

    wxTextCtrl* MakeField(wxWindow* parent, wxSizer* sizer,
                          const wxString& label, const wxString& value,
                          bool multiLine = false);
    wxStaticText* MakeLabel(wxWindow* parent, const wxString& text);
};
