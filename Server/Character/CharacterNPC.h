#ifndef _CHARACTER_NPC_H_
#define _CHARACTER_NPC_H_

#include "Character.h"

class CharacterNPC : public Character
{
public:
	CharacterNPC(const CHARACTER_TYPE& type, const std::string& name)
		:
		Character(type, name)
	{}
	virtual ~CharacterNPC(){ destroy(); }
	void destroy(){}
};

#endif
