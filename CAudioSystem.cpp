#include "CAudioSystem.h"

//////////////////////////////////////////////////////////////////////////

namespace KGF
{
	//////////////////////////////////////////////////////////////////////////
	sf::SoundBuffer soundBuffer;
	sf::Sound sound(soundBuffer);
	//////////////////////////////////////////////////////////////////////////
	sf::SoundBuffer& CAudioSystem::getBuffer(const std::string& filename)
	{
		auto it = bufferCache.find(filename);
		if (it != bufferCache.end())
		{
			return it->second;
		}
		else
		{
			sf::SoundBuffer buffer;
			if (buffer.loadFromFile(filename))
			{
				auto result = bufferCache.emplace(filename, std::move(buffer));
				return result.first->second;
			}
			else
			{
				static sf::SoundBuffer dummy;
				return dummy;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::playMusic1(const std::string& name, bool loop)
	{
		music1.openFromFile(name);

		if (loop)
		{
			music1.setLooping(true);
		}
		else
		{
			music1.setLooping(false);
		}

		if (ini.Open("data/settings/config.ini"))
		{
			volumeMaster = std::stoi(ini.GetValue("audio", "master", "0"));
			volumeMusic1 = std::stoi(ini.GetValue("audio", "music1", "0"));
		}

		music1.setVolume(volumeMusic1 * volumeMaster / 100);
		music1.play();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::playMusic2(const std::string& name, bool loop)
	{
		music2.openFromFile(name);
		

		if (loop)
		{
			music2.setLooping(true);
		}
		else
		{
			music2.setLooping(false);
		}

		if (ini.Open("data/settings/config.ini"))
		{
			volumeMaster = std::stoi(ini.GetValue("audio", "master", "0"));
			volumeMusic2 = std::stoi(ini.GetValue("audio", "music2", "0"));
		}

		music2.setVolume(volumeMusic2 * volumeMaster / 100);
		music2.play();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::playSound(const std::string& name, bool loop)
	{
		soundBuffer.loadFromFile(name);

		if (loop)
		{
			sound.setLooping(true);
		}
		else
		{
			sound.setLooping(false);
		}

		if (ini.Open("data/settings/config.ini"))
		{
			volumeMaster = std::stoi(ini.GetValue("audio", "master", "0"));
			volumeSound = std::stoi(ini.GetValue("audio", "sound1", "0"));
		}

		sound.setVolume(volumeSound * volumeMaster / 100);
		sound.play();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::playSFX(const std::string& name)
	{
		sf::SoundBuffer& buf = getBuffer(name);
		if (buf.getSampleCount() == 0)
		{
			return;
		}

		if (ini.Open("data/settings/config.ini"))
		{
			volumeMaster = std::stoi(ini.GetValue("audio", "master", "0"));
			volumeSFX = std::stoi(ini.GetValue("audio", "sound2", "0"));
		}

		auto soundPtr = std::make_unique<sf::Sound>(buf);
		sf::Sound* rawSoundPtr = soundPtr.get(); 

		activeSounds.push_back(std::move(soundPtr));

		rawSoundPtr->setVolume(volumeSFX * volumeMaster / 100);
		rawSoundPtr->play();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::MusicLoopEnd()
	{
		fadingOutM = true;
		fadeClockM.restart(); 		
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::AmbientLoopEnd()
	{
		fadingOutA = true;
		fadeClockA.restart(); 		
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::setVolMaster(unsigned short vol)
	{
		volumeMaster = vol;
		ini.SetValue("audio", "master", std::to_string(volumeMaster));
		music1.setVolume(volumeMusic1 * volumeMaster / 100);
		music2.setVolume(volumeMusic2 * volumeMaster / 100);
		sound.setVolume(volumeSound * volumeMaster / 100);
		for (auto& soundPtr : activeSounds)
		{
			soundPtr->setVolume(volumeSFX * volumeMaster / 100);
		}
		ini.Save(); 
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::setVolMusic1(unsigned short vol)
	{
		volumeMusic1 = vol;
		ini.SetValue("audio", "music1", std::to_string(volumeMusic1));
		music1.setVolume(volumeMusic1 * volumeMaster / 100);
		ini.Save();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::setVolMusic2(unsigned short vol)
	{
		volumeMusic2 = vol;
		ini.SetValue("audio", "music2", std::to_string(volumeMusic2));
		music2.setVolume(volumeMusic2 * volumeMaster / 100);
		ini.Save();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::setVolSound(unsigned short vol)
	{
		volumeSound = vol;
		ini.SetValue("audio", "sound1", std::to_string(volumeSound));
		sound.setVolume(volumeSound * volumeMaster / 100);
		ini.Save();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::setVolSFX(unsigned short vol)
	{
		volumeSFX = vol;
		ini.SetValue("audio", "sound2", std::to_string(volumeSFX));
		for (auto& soundPtr : activeSounds)
		{
			soundPtr->setVolume(volumeSFX * volumeMaster / 100);
		}
		ini.Save();
	}
	//////////////////////////////////////////////////////////////////////////
	void CAudioSystem::update()
	{
		auto it = activeSounds.begin();
		while (it != activeSounds.end())
		{
			if ((*it)->getStatus() == sf::Sound::Status::Stopped)
			{
				it = activeSounds.erase(it);
			}
			else
			{
				++it;
			}
		}

		if (fadingOutM)
		{
			float deltaTime = fadeClockM.getElapsedTime().asSeconds();
			float fadeSpeed = 40.0f; 
			float currentVolume = music1.getVolume();
			float newVolume = currentVolume - fadeSpeed * deltaTime;

			if (newVolume <= 0)
			{
				newVolume = 0;
				music1.stop();
				fadingOutM = false;
			}
			music1.setVolume(newVolume);

			if (newVolume > 0)
			{
				fadeClockM.restart();
			}
		}
		if (fadingOutA)
		{
			float deltaTime = fadeClockA.getElapsedTime().asSeconds();
			float fadeSpeed = 40.0f; 
			float currentVolume = music2.getVolume();
			float newVolume = currentVolume - fadeSpeed * deltaTime;

			if (newVolume <= 0)
			{
				newVolume = 0;
				music2.stop();
				fadingOutA = false; 
			}
			music2.setVolume(newVolume);

			if (newVolume > 0)
			{
				fadeClockA.restart();
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
}
