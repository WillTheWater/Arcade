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
	ButtonTexture.load("Content/Assets/Textures/button2.png");
	auto Layout = tgui::VerticalLayout::create();
	Layout->setSize("37%", "43%");
	Layout->setPosition("32%", "18%");
	Layout->getRenderer()->setSpaceBetweenWidgets(14);
	Layout->getRenderer()->setPadding(40);

	for (const auto& [Selection, Name] : magic_enum::enum_entries<OverlaySelection>())
	{
		std::string ButtonText = Name.data();
		std::replace(ButtonText.begin(), ButtonText.end(), '_', ' ');
		auto Button = tgui::Button::create(ButtonText.data());
		Button->getRenderer()->setBorders(0);
		Button->getRenderer()->setTexture(ButtonTexture);
		Button->setTextSize(16);
		Button->getRenderer()->setTextColorHover({ 180, 180, 180, 255 });
		Button->getRenderer()->setTextColorDown({ 70, 70, 70, 255 });
		Button->onPress([this, Selection] {this->Selection = Selection; });
		Layout->add(Button);
	}
	Group->add(Layout);
}