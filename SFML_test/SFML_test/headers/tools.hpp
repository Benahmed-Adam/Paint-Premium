#pragma once

#include <windows.h>
#include <string>
#include <commdlg.h>
#include <iostream>

std::string showSaveFileDialog() {
    OPENFILENAME ofn;       // Structure de contrôle pour la boîte de dialogue
    char szFile[260] = { 0 }; // Buffer pour le nom de fichier

    // Initialisation de la structure OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;  // Aucun propriétaire
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

    // Affichage de la boîte de dialogue
    if (GetSaveFileName(&ofn) == TRUE) {
        return std::string(ofn.lpstrFile);
    }
    else {
        return "";
    }
}