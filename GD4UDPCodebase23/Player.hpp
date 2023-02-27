#pragma once
class Player
{
public:
	Player() : m_health(3), m_ammo(10) {}
	virtual void Update();

private:
	uint32_t m_health;
	uint32_t m_ammo;
};

