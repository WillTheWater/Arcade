#pragma once

#include <SFML/System/Vector3.hpp>

class AudioManager
{
public:
	void SetGlobalVolume(float Volume);
	float GetGlobalColume() const;
	
	void SetListenerPosition(sf::Vector3f Position);
	sf::Vector3f GetListenerPosition() const;
	
	void SetListenerDirection(sf::Vector3f Direction);
	sf::Vector3f GetListenerDirection() const;	
};