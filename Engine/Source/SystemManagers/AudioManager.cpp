#include <SFML/Audio/Listener.hpp>

#include "SystemManagers/AudioManager.h"

void AudioManager::SetGlobalVolume(float Volume)
{
	sf::Listener::setGlobalVolume(Volume);
}

float AudioManager::GetGlobalVolume() const
{
	return sf::Listener::getGlobalVolume();
}

void AudioManager::SetListenerPosition(sf::Vector3f Position)
{
	sf::Listener::setPosition(Position);
}

sf::Vector3f AudioManager::GetListenerPosition() const
{
	return sf::Listener::getPosition();
}

void AudioManager::SetListenerDirection(sf::Vector3f Direction)
{
	sf::Listener::setDirection(Direction);
}

sf::Vector3f AudioManager::GetListenerDirection() const
{
	return sf::Listener::getDirection();
}
