// Copyright © Mad Tea Party Development 2025
// Proprietary software — see license.

#include "AssetLoggerFrame.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/font.h>
#include <wx/settings.h>
#include <fstream>
#include <sstream>

// ════════════════════════════════════════════════════════════════════════════
//  Event table
// ════════════════════════════════════════════════════════════════════════════
enum
{
    ID_BTN_NEW = wxID_HIGHEST + 1,
    ID_BTN_EDIT,
    ID_BTN_DELETE,
    ID_BTN_OPEN,
    ID_BTN_SAVE,
    ID_BTN_SAVE_AS,
    ID_GRID,
};

wxBEGIN_EVENT_TABLE(AssetLoggerFrame, wxFrame)
EVT_BUTTON(ID_BTN_NEW, AssetLoggerFrame::OnNew)
EVT_BUTTON(ID_BTN_EDIT, AssetLoggerFrame::OnEdit)
EVT_BUTTON(ID_BTN_DELETE, AssetLoggerFrame::OnDelete)
EVT_BUTTON(ID_BTN_OPEN, AssetLoggerFrame::OnOpen)
EVT_BUTTON(ID_BTN_SAVE, AssetLoggerFrame::OnSave)
EVT_BUTTON(ID_BTN_SAVE_AS, AssetLoggerFrame::OnSaveAs)
EVT_LIST_ITEM_ACTIVATED(ID_GRID, AssetLoggerFrame::OnGridActivate)
EVT_CLOSE(AssetLoggerFrame::OnClose)
wxEND_EVENT_TABLE()

// ════════════════════════════════════════════════════════════════════════════
//  Construction
// ════════════════════════════════════════════════════════════════════════════
AssetLoggerFrame::AssetLoggerFrame(bool demoMode)
    : wxFrame(nullptr, wxID_ANY,
        demoMode ? "Asset Logger [DEMO] — Mad Tea Party Development"
        : "Asset Logger — Mad Tea Party Development",
        wxDefaultPosition, wxSize(1020, 660)),
    m_demo(demoMode)
{
    SetBackgroundColour(Theme::BG_DEEP);
    SetMinSize(wxSize(760, 480));
    BuildLayout();
    Centre();
}

// ════════════════════════════════════════════════════════════════════════════
//  Layout
// ════════════════════════════════════════════════════════════════════════════
void AssetLoggerFrame::BuildLayout()
{
    wxBoxSizer* root = new wxBoxSizer(wxHORIZONTAL);

    m_sidebar = BuildSidebar(this);
    m_mainArea = BuildMainArea(this);

    root->Add(m_sidebar, 0, wxEXPAND);
    root->Add(m_mainArea, 1, wxEXPAND);

    SetSizer(root);
    Layout();
}

// ─── Sidebar ─────────────────────────────────────────────────────────────────
wxPanel* AssetLoggerFrame::BuildSidebar(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    panel->SetBackgroundColour(Theme::BG_PANEL);
    panel->SetMinSize(wxSize(180, -1));

    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    // ── Branding block ──────────────────────────────────────────────────
    wxPanel* brand = new wxPanel(panel, wxID_ANY);
    brand->SetBackgroundColour(Theme::BG_CARD);
    {
        wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);

        wxStaticText* title = new wxStaticText(brand, wxID_ANY, "Asset\nLogger");
        wxFont tf = title->GetFont();
        tf.SetPointSize(20);
        tf.SetWeight(wxFONTWEIGHT_BOLD);
        title->SetFont(tf);
        title->SetForegroundColour(Theme::TEXT_HEAD);

        wxStaticText* sub = new wxStaticText(brand, wxID_ANY,
            m_demo ? "DEMO EDITION" : "Mad Tea Party Dev");
        wxFont sf = sub->GetFont();
        sf.SetPointSize(7);
        sf.SetStyle(wxFONTSTYLE_ITALIC);
        sub->SetFont(sf);
        sub->SetForegroundColour(m_demo ? Theme::DANGER : Theme::TEXT_SEC);

        bs->AddSpacer(16);
        bs->Add(title, 0, wxLEFT, 18);
        bs->AddSpacer(4);
        bs->Add(sub, 0, wxLEFT | wxBOTTOM, 18);
        brand->SetSizer(bs);
    }
    col->Add(brand, 0, wxEXPAND);

    // thin gold separator
    wxStaticLine* sep = new wxStaticLine(panel, wxID_ANY);
    sep->SetBackgroundColour(Theme::ACCENT);
    col->Add(sep, 0, wxEXPAND);

    col->AddSpacer(14);

    // ── Section label ───────────────────────────────────────────────────
    auto addLabel = [&](const wxString& text)
        {
            wxStaticText* lbl = new wxStaticText(panel, wxID_ANY, text);
            wxFont lf = lbl->GetFont();
            lf.SetPointSize(7);
            lbl->SetFont(lf);
            lbl->SetForegroundColour(Theme::TEXT_SEC);
            col->Add(lbl, 0, wxLEFT, 18);
            col->AddSpacer(6);
        };

    addLabel("RECORDS");
    m_btnNew = MakeSideButton(panel, "+ New Asset", ID_BTN_NEW,
        Theme::ACCENT, Theme::BG_DEEP, true);
    m_btnEdit = MakeSideButton(panel, "  Edit", ID_BTN_EDIT,
        Theme::BG_CARD, Theme::TEXT_PRI);
    m_btnDelete = MakeSideButton(panel, "  Delete", ID_BTN_DELETE,
        Theme::BG_CARD, Theme::DANGER);
    col->Add(m_btnNew, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);
    col->AddSpacer(4);
    col->Add(m_btnEdit, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);
    col->AddSpacer(4);
    col->Add(m_btnDelete, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);

    col->AddSpacer(22);
    addLabel("FILE");
    m_btnOpen = MakeSideButton(panel, "  Open CSV", ID_BTN_OPEN,
        Theme::BG_CARD, Theme::TEXT_PRI);
    m_btnSave = MakeSideButton(panel, "  Save", ID_BTN_SAVE,
        Theme::BG_CARD, Theme::TEXT_PRI);
    m_btnSaveAs = MakeSideButton(panel, "  Save As", ID_BTN_SAVE_AS,
        Theme::BG_CARD, Theme::TEXT_PRI);
    col->Add(m_btnOpen, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);
    col->AddSpacer(4);
    col->Add(m_btnSave, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);
    col->AddSpacer(4);
    col->Add(m_btnSaveAs, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);

    col->AddStretchSpacer();

    // ── Copyright footer ────────────────────────────────────────────────
    wxStaticText* copy = new wxStaticText(panel, wxID_ANY,
        "© Mad Tea Party Dev 2025", wxDefaultPosition, wxDefaultSize,
        wxALIGN_CENTRE_HORIZONTAL);
    wxFont cf = copy->GetFont();
    cf.SetPointSize(6);
    copy->SetFont(cf);
    copy->SetForegroundColour(Theme::TEXT_SEC);
    col->Add(copy, 0, wxALIGN_CENTRE | wxBOTTOM, 10);

    panel->SetSizer(col);
    return panel;
}

wxButton* AssetLoggerFrame::MakeSideButton(wxWindow* parent,
    const wxString& label, wxWindowID id,
    const wxColour& bg, const wxColour& fg, bool bold)
{
    wxButton* btn = new wxButton(parent, id, label,
        wxDefaultPosition, wxSize(-1, 36));
    btn->SetBackgroundColour(bg);
    btn->SetForegroundColour(fg);
    wxFont f = btn->GetFont();
    if (bold) f.SetWeight(wxFONTWEIGHT_BOLD);
    f.SetPointSize(9);
    btn->SetFont(f);
    return btn;
}

// ─── Main area ───────────────────────────────────────────────────────────────
wxPanel* AssetLoggerFrame::BuildMainArea(wxWindow* parent)
{
    wxPanel* panel = new wxPanel(parent, wxID_ANY);
    panel->SetBackgroundColour(Theme::BG_DEEP);

    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    // ── Top bar ─────────────────────────────────────────────────────────
    wxPanel* topBar = new wxPanel(panel, wxID_ANY);
    topBar->SetBackgroundColour(Theme::BG_CARD);
    {
        wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText* heading = new wxStaticText(topBar, wxID_ANY, "Asset Registry");
        wxFont hf = heading->GetFont();
        hf.SetPointSize(13);
        hf.SetWeight(wxFONTWEIGHT_BOLD);
        heading->SetFont(hf);
        heading->SetForegroundColour(Theme::TEXT_PRI);

        m_statusLabel = new wxStaticText(topBar, wxID_ANY, "No file open");
        wxFont sf = m_statusLabel->GetFont();
        sf.SetPointSize(8);
        sf.SetStyle(wxFONTSTYLE_ITALIC);
        m_statusLabel->SetFont(sf);
        m_statusLabel->SetForegroundColour(Theme::TEXT_SEC);

        bs->Add(heading, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 18);
        bs->AddStretchSpacer();
        bs->Add(m_statusLabel, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 18);
        bs->AddSpacer(4);
        topBar->SetSizer(bs);
        topBar->SetMinSize(wxSize(-1, 50));
    }
    col->Add(topBar, 0, wxEXPAND);

    // thin gold line under top bar
    wxStaticLine* sep2 = new wxStaticLine(panel, wxID_ANY);
    sep2->SetBackgroundColour(Theme::ACCENT2);
    col->Add(sep2, 0, wxEXPAND);

    // ── List control (the grid) ──────────────────────────────────────────
    m_grid = new wxListCtrl(panel, ID_GRID, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_SINGLE_SEL | wxBORDER_NONE);
    m_grid->SetBackgroundColour(Theme::BG_DEEP);
    m_grid->SetTextColour(Theme::TEXT_PRI);
    m_grid->SetFont(wxFont(9, wxFONTFAMILY_MODERN,
        wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Columns
    if (m_demo)
    {
        m_grid->AppendColumn("Platform", wxLIST_FORMAT_LEFT, 160);
        m_grid->AppendColumn("Use", wxLIST_FORMAT_LEFT, 220);
        m_grid->AppendColumn("Maker", wxLIST_FORMAT_LEFT, 200);
    }
    else
    {
        m_grid->AppendColumn("Platform", wxLIST_FORMAT_LEFT, 140);
        m_grid->AppendColumn("Location", wxLIST_FORMAT_LEFT, 140);
        m_grid->AppendColumn("Use", wxLIST_FORMAT_LEFT, 140);
        m_grid->AppendColumn("Name", wxLIST_FORMAT_LEFT, 140);
        m_grid->AppendColumn("Maker", wxLIST_FORMAT_LEFT, 130);
        m_grid->AppendColumn("License", wxLIST_FORMAT_LEFT, 130);
    }

    col->Add(m_grid, 1, wxEXPAND | wxALL, 10);

    // ── Hint ────────────────────────────────────────────────────────────
    wxStaticText* hint = new wxStaticText(panel, wxID_ANY,
        "Double-click a row to edit  ·  Select a row, then use Edit or Delete");
    wxFont hf2 = hint->GetFont();
    hf2.SetPointSize(7);
    hint->SetFont(hf2);
    hint->SetForegroundColour(Theme::TEXT_SEC);
    col->Add(hint, 0, wxLEFT | wxBOTTOM, 12);

    panel->SetSizer(col);
    return panel;
}

// ════════════════════════════════════════════════════════════════════════════
//  Grid helpers
// ════════════════════════════════════════════════════════════════════════════
void AssetLoggerFrame::RebuildGrid()
{
    m_grid->DeleteAllItems();
    for (long i = 0; i < (long)m_records.size(); ++i)
        RefreshRow(i, m_records[i]);
}

void AssetLoggerFrame::RefreshRow(long row, const AssetRecord& rec)
{
    // Insert or update
    if (row >= m_grid->GetItemCount())
    {
        m_grid->InsertItem(row, rec.platform);
        // alternate row tint
        m_grid->SetItemBackgroundColour(row,
            (row % 2 == 0) ? Theme::BG_DEEP : Theme::BG_PANEL);
    }
    else
    {
        m_grid->SetItemText(row, rec.platform);
    }

    if (m_demo)
    {
        m_grid->SetItem(row, 1, rec.use);
        m_grid->SetItem(row, 2, rec.maker);
    }
    else
    {
        m_grid->SetItem(row, 1, rec.location);
        m_grid->SetItem(row, 2, rec.use);
        m_grid->SetItem(row, 3, rec.name);
        m_grid->SetItem(row, 4, rec.maker);
        m_grid->SetItem(row, 5, rec.license);
    }
}

int AssetLoggerFrame::SelectedRow() const
{
    return (int)m_grid->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

// ════════════════════════════════════════════════════════════════════════════
//  CRUD handlers
// ════════════════════════════════════════════════════════════════════════════
void AssetLoggerFrame::OnNew(wxCommandEvent&)
{
    AssetRecord blank;
    AssetEditDialog dlg(this, m_demo, blank, "New Asset");
    if (dlg.ShowModal() == wxID_OK)
    {
        m_records.push_back(dlg.GetRecord());
        long row = (long)m_records.size() - 1;
        RefreshRow(row, m_records[row]);
        MarkDirty(true);
        SetStatus(wxString::Format("%zu record(s)", m_records.size()));
    }
}

void AssetLoggerFrame::OnEdit(wxCommandEvent&)
{
    int row = SelectedRow();
    if (row < 0)
    {
        wxMessageBox("Please select a row first.", "No Selection",
            wxOK | wxICON_INFORMATION, this);
        return;
    }
    AssetEditDialog dlg(this, m_demo, m_records[row], "Edit Asset");
    if (dlg.ShowModal() == wxID_OK)
    {
        m_records[row] = dlg.GetRecord();
        RefreshRow(row, m_records[row]);
        MarkDirty(true);
    }
}

void AssetLoggerFrame::OnDelete(wxCommandEvent&)
{
    int row = SelectedRow();
    if (row < 0)
    {
        wxMessageBox("Please select a row first.", "No Selection",
            wxOK | wxICON_INFORMATION, this);
        return;
    }
    if (wxMessageBox("Delete this asset?", "Confirm Delete",
        wxYES_NO | wxNO_DEFAULT | wxICON_WARNING, this) == wxYES)
    {
        m_records.erase(m_records.begin() + row);
        RebuildGrid();
        MarkDirty(true);
        SetStatus(wxString::Format("%zu record(s)", m_records.size()));
    }
}

void AssetLoggerFrame::OnGridActivate(wxListEvent& e)
{
    int row = (int)e.GetIndex();
    if (row < 0 || row >= (int)m_records.size()) return;
    AssetEditDialog dlg(this, m_demo, m_records[row], "Edit Asset");
    if (dlg.ShowModal() == wxID_OK)
    {
        m_records[row] = dlg.GetRecord();
        RefreshRow(row, m_records[row]);
        MarkDirty(true);
    }
}

void AssetLoggerFrame::OnOpen(wxCommandEvent&)
{
    if (!ConfirmDiscard()) return;
    wxFileDialog fd(this, "Open CSV", "", "",
        "CSV files (*.csv)|*.csv|All files (*.*)|*.*",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (fd.ShowModal() == wxID_CANCEL) return;
    if (LoadCSV(fd.GetPath()))
    {
        m_currentFile = fd.GetPath();
        MarkDirty(false);
        SetStatus(wxString::Format("Opened: %s  (%zu record(s))",
            fd.GetFilename(), m_records.size()));
    }
}

void AssetLoggerFrame::OnSave(wxCommandEvent&)
{
    if (m_currentFile.IsEmpty())
    {
        wxCommandEvent dummy;
        OnSaveAs(dummy);
        return;
    }
    if (SaveCSV(m_currentFile))
    {
        MarkDirty(false);
        SetStatus("Saved: " + m_currentFile);
    }
}

void AssetLoggerFrame::OnSaveAs(wxCommandEvent&)
{
    wxFileDialog fd(this, "Save CSV As", "", "assets.csv",
        "CSV files (*.csv)|*.csv|All files (*.*)|*.*",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (fd.ShowModal() == wxID_CANCEL) return;
    if (SaveCSV(fd.GetPath()))
    {
        m_currentFile = fd.GetPath();
        MarkDirty(false);
        SetStatus("Saved: " + fd.GetFilename());
    }
}

void AssetLoggerFrame::OnClose(wxCloseEvent& e)
{
    if (m_dirty && !ConfirmDiscard())
    {
        e.Veto();
        return;
    }
    Destroy();
}

// ════════════════════════════════════════════════════════════════════════════
//  File I/O
// ════════════════════════════════════════════════════════════════════════════
bool AssetLoggerFrame::LoadCSV(const wxString& path)
{
    std::ifstream file(path.ToStdString());
    if (!file.is_open())
    {
        wxMessageBox("Could not open: " + path, "Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    m_records.clear();

    std::string line;
    bool firstLine = true;
    while (std::getline(file, line))
    {
        // strip \r
        if (!line.empty() && line.back() == '\r') line.pop_back();
        // skip header row
        if (firstLine) { firstLine = false; continue; }
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string tok;
        std::vector<std::string> cols;
        while (std::getline(ss, tok, ',')) cols.push_back(tok);

        // Pad if short
        while (cols.size() < 6) cols.push_back("");

        AssetRecord rec;
        rec.platform = cols[0];
        rec.location = cols[1];
        rec.use = cols[2];
        rec.name = cols[3];
        rec.maker = cols[4];
        rec.license = cols[5];
        m_records.push_back(rec);
    }
    RebuildGrid();
    return true;
}

bool AssetLoggerFrame::SaveCSV(const wxString& path)
{
    std::ofstream file(path.ToStdString());
    if (!file.is_open())
    {
        wxMessageBox("Could not write: " + path, "Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    file << "Platform,Location(in Project),Use(in Project),"
        "Asset Name(on Platform),Asset Maker(on Platform),License Type\n";
    for (auto& rec : m_records)
    {
        if (m_demo)
        {
            file << rec.platform.ToStdString() << ','
                << "Full Version Only" << ','
                << rec.use.ToStdString() << ','
                << "Full Version Only" << ','
                << rec.maker.ToStdString() << ','
                << "Full Version Only" << '\n';
        }
        else
        {
            file << rec.platform.ToStdString() << ','
                << rec.location.ToStdString() << ','
                << rec.use.ToStdString() << ','
                << rec.name.ToStdString() << ','
                << rec.maker.ToStdString() << ','
                << rec.license.ToStdString() << '\n';
        }
    }
    return true;
}

// ════════════════════════════════════════════════════════════════════════════
//  Helpers
// ════════════════════════════════════════════════════════════════════════════
void AssetLoggerFrame::SetStatus(const wxString& msg, bool warn)
{
    if (m_statusLabel)
    {
        m_statusLabel->SetLabel(msg);
        m_statusLabel->SetForegroundColour(warn ? Theme::DANGER : Theme::TEXT_SEC);
        m_statusLabel->GetParent()->Layout();
    }
}

void AssetLoggerFrame::MarkDirty(bool dirty)
{
    m_dirty = dirty;
    m_btnSave->Enable(dirty);
}

bool AssetLoggerFrame::ConfirmDiscard()
{
    if (!m_dirty) return true;
    int res = wxMessageBox(
        "You have unsaved changes. Discard them?",
        "Unsaved Changes",
        wxYES_NO | wxNO_DEFAULT | wxICON_WARNING, this);
    return res == wxYES;
}

// ════════════════════════════════════════════════════════════════════════════
//  AssetEditDialog
// ════════════════════════════════════════════════════════════════════════════
AssetEditDialog::AssetEditDialog(wxWindow* parent, bool demoMode,
    const AssetRecord& rec, const wxString& title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(500, -1),
        wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_demo(demoMode)
{
    SetBackgroundColour(Theme::BG_CARD);

    wxBoxSizer* outer = new wxBoxSizer(wxVERTICAL);

    // Title bar strip
    wxStaticText* dlgTitle = new wxStaticText(this, wxID_ANY, title);
    wxFont tf = dlgTitle->GetFont();
    tf.SetPointSize(11);
    tf.SetWeight(wxFONTWEIGHT_BOLD);
    dlgTitle->SetFont(tf);
    dlgTitle->SetForegroundColour(Theme::TEXT_HEAD);
    outer->Add(dlgTitle, 0, wxALL, 16);

    wxStaticLine* sep = new wxStaticLine(this);
    sep->SetBackgroundColour(Theme::ACCENT2);
    outer->Add(sep, 0, wxEXPAND | wxLEFT | wxRIGHT, 0);
    outer->AddSpacer(10);

    // Fields
    wxFlexGridSizer* grid = new wxFlexGridSizer(2, 8, 14);
    grid->AddGrowableCol(1, 1);

    auto addField = [&](const wxString& label, const wxString& val,
        wxTextCtrl*& ctrl)
        {
            wxStaticText* lbl = new wxStaticText(this, wxID_ANY, label);
            wxFont lf = lbl->GetFont();
            lf.SetPointSize(8);
            lbl->SetFont(lf);
            lbl->SetForegroundColour(Theme::TEXT_SEC);

            ctrl = new wxTextCtrl(this, wxID_ANY, val, wxDefaultPosition,
                wxDefaultSize, wxTE_LEFT);
            ctrl->SetBackgroundColour(Theme::BG_INPUT);
            ctrl->SetForegroundColour(Theme::TEXT_PRI);

            grid->Add(lbl, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 4);
            grid->Add(ctrl, 1, wxEXPAND);
        };

    addField("Platform", rec.platform, m_platform);

    if (!m_demo)
        addField("Location (in Project)", rec.location, m_location);

    addField("Use (in Project)", rec.use, m_use);

    if (!m_demo)
        addField("Name (on Platform)", rec.name, m_name);

    addField("Maker (on Platform)", rec.maker, m_maker);

    if (!m_demo)
        addField("License Type", rec.license, m_license);

    outer->Add(grid, 0, wxEXPAND | wxLEFT | wxRIGHT, 18);
    outer->AddSpacer(16);

    // Demo notice
    if (m_demo)
    {
        wxStaticText* notice = new wxStaticText(this, wxID_ANY,
            "Location, Name, and License fields are available in the full version.");
        wxFont nf = notice->GetFont();
        nf.SetPointSize(7);
        nf.SetStyle(wxFONTSTYLE_ITALIC);
        notice->SetFont(nf);
        notice->SetForegroundColour(Theme::DANGER);
        outer->Add(notice, 0, wxLEFT | wxBOTTOM, 18);
    }

    // Buttons
    wxStdDialogButtonSizer* btns = CreateStdDialogButtonSizer(wxOK | wxCANCEL);

    // Style OK button with accent
    if (wxButton* ok = wxDynamicCast(FindWindowById(wxID_OK, this), wxButton))
    {
        ok->SetBackgroundColour(Theme::ACCENT);
        ok->SetForegroundColour(Theme::BG_DEEP);
        wxFont bf = ok->GetFont();
        bf.SetWeight(wxFONTWEIGHT_BOLD);
        ok->SetFont(bf);
    }

    outer->Add(btns, 0, wxEXPAND | wxALL, 14);
    SetSizerAndFit(outer);
    Centre();
}

AssetRecord AssetEditDialog::GetRecord() const
{
    AssetRecord rec;
    rec.platform = m_platform ? m_platform->GetValue() : "";
    rec.use = m_use ? m_use->GetValue() : "";
    rec.maker = m_maker ? m_maker->GetValue() : "";

    if (!m_demo)
    {
        rec.location = m_location ? m_location->GetValue() : "";
        rec.name = m_name ? m_name->GetValue() : "";
        rec.license = m_license ? m_license->GetValue() : "";
    }
    return rec;
}