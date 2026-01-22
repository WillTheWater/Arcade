#pragma once

#include <optional>

#include "SystemManagers/GUIManager.h"

enum class OverlaySelection
{
	Resume,
	Restart,
	Main_Menu,
	Quit
};

class Overlay
{
public:
	Overlay(GUIManager& HUD);

	std::optional<OverlaySelection> GetSelection();
	void SetVisible(bool Visibility);
	bool IsVisible() const;

private:
	std::optional<OverlaySelection> Selection;
	tgui::Group::Ptr Group;
	tgui::Texture ButtonTexture;

private:
	void InitBackground();
	void InitButtons();
};