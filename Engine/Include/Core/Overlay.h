#pragma once

#include <optional>

#include "SystemManagers/GUIManager.h"

enum class OverlaySelection
{
	Resume,
	Restart,
	MainMenu,
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

private:
	void InitBackground();
	void InitButtons();
};