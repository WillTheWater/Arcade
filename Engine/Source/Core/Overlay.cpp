#include <utility>
#include <magic_enum/magic_enum.hpp>

#include "Core/Overlay.h"

Overlay::Overlay(GUIManager& HUD)
{
	Group = tgui::Group::create();
	Group->setVisible(false);
	InitBackground();
	InitButtons();
	HUD.AddWidget(Group);
}

std::optional<OverlaySelection> Overlay::GetSelection()
{
	return std::exchange(Selection, std::nullopt);
}

void Overlay::SetVisible(bool Visibility)
{
	Group->setVisible(Visibility);
}

bool Overlay::IsVisible() const
{
	return Group->isVisible();
}

void Overlay::InitBackground()
{
	auto Background = tgui::Panel::create();
	Background->getRenderer()->setBackgroundColor({ 0, 0, 0, 175 });
	Group->add(Background);
}

void Overlay::InitButtons()
{
	auto Layout = tgui::VerticalLayout::create();
	Layout->setSize("37%", "63%");
	Layout->setPosition("31%", "18%");

	for (const auto& [Selection, Name] : magic_enum::enum_entries<OverlaySelection>())
	{
		auto Button = tgui::Button::create(Name.data());
		Button->setTextSize(30);
		Button->onPress([this, Selection] {this->Selection = Selection; });
		Layout->add(Button);
	}
	Group->add(Layout);
}