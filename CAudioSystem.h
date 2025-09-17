#ifndef C_AUDIOSYSTEM_H
#define C_AUDIOSYSTEM_H

#pragma warning(disable : 4834)
#pragma warning(disable : 4244)

#include <SFML/Audio.hpp>
#include "CIni.h"

namespace KGF
{
	class CAudioSystem
	{
	public:
		CAudioSystem() = default;
		~CAudioSystem() = default;
	
		unsigned short volumeMaster, volumeMusic1, volumeMusic2, volumeSound, volumeSFX;

		void update();

		void playMusic1(const std::string& name, bool loop);
		void playMusic2(const std::string& name, bool loop);
		void playSound(const std::string& name, bool loop);
		void playSFX(const std::string& name);

		void setVolMaster(unsigned short vol);
		void setVolMusic1(unsigned short vol);
		void setVolMusic2(unsigned short vol);
		void setVolSound(unsigned short vol);
		void setVolSFX(unsigned short vol);

		void MusicLoopEnd();
		void AmbientLoopEnd();

		sf::Music music1, music2;

	private:
		CIni ini;
		CLog log;

		sf::Clock fadeClockM;  
		sf::Clock fadeClockA; 

		bool fadingOutM = false;
		bool fadingOutA = false;  


		std::unordered_map<std::string, sf::SoundBuffer> bufferCache;
		std::vector<std::unique_ptr<sf::Sound>> activeSounds;
		sf::SoundBuffer& getBuffer(const std::string& filename);

	};

}


#endif