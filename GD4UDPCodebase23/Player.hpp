#pragma once
class Player
{
public:
	Player() : m_health(3), m_ammo(10), m_name("Default"), m_position(10, 10, 10), m_rotation(0, 0, 0, 1), m_weapons{ 60, 70, 80 } {}
	uint32_t GetHealth() const;
	uint32_t GetAmmo() const;
	void Write(OutputMemoryStream& out_stream) const;
	void Read(InputMemoryStream& in_stream);
	virtual void toString() const;
	void WriteBits(OutputMemoryBitStream& out_stream) const;
	void ReadBits(InputMemoryBitStream& in_stream);

private:
	uint32_t m_health;
	uint32_t m_ammo;
	char m_name[128];
	Vector3 m_position;
	Quaternion m_rotation;
	vector<int> m_weapons;
};

