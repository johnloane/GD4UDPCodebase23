#include "SocketWrapperPCH.hpp"

uint32_t Player::GetHealth() const
{
	return m_health;
}

uint32_t Player::GetAmmo() const
{
	return m_ammo;
}

void Player::Write(OutputMemoryStream& out_stream) const
{
	out_stream.Write(m_health);
	out_stream.Write(m_ammo);
	out_stream.Write(m_name, 128);
	out_stream.Write(m_position);
	out_stream.Write(m_rotation);
	out_stream.Write(m_weapons);
}

void Player::Read(InputMemoryStream& in_stream)
{
	in_stream.Read(m_health);
	in_stream.Read(m_ammo);
	in_stream.Read(m_name, 128);
	in_stream.Read(m_position);
	in_stream.Read(m_rotation);
	in_stream.Read(m_weapons);
}

void Player::toString() const
{
	std::cout << "Health: " << m_health << std::endl;
	std::cout << "Ammo: " << m_ammo << std::endl;
	std::cout << "Name: " << m_name << std::endl;
	std::cout << "Position: " << m_position.mX << ", " << m_position.mY << ", " << m_position.mZ << std::endl;
	std::cout << "Rotation: " << m_rotation.mX << ", " << m_rotation.mY << ", " << m_rotation.mZ << ", " << m_rotation.mW << std::endl;
	std::cout << "Weapons: ";
	for (int i = 0; i < m_weapons.size(); i++)
	{
		std::cout << m_weapons[i] << ", ";
	}
	std::cout << std::endl;
}

void Player::WriteBits(OutputMemoryBitStream& out_stream) const
{
	out_stream.WriteBits(m_health, 7);
	out_stream.WriteBits(m_ammo, 9);
	uint8_t name_length = static_cast<uint8_t>(strlen(m_name));
	out_stream.WriteBits(name_length, 8);
	out_stream.WriteBits(m_name, (name_length * 8));
	out_stream.Write(m_weapons);
	out_stream.WritePos(m_position);
	out_stream.Write(m_rotation);
}

void Player::ReadBits(InputMemoryBitStream& in_stream)
{
	in_stream.ReadBits(&m_health, 7);
	std::cout << "Health" << std::endl;
	in_stream.ReadBits(&m_ammo, 9);
	std::cout << "Ammo" << std::endl;
	uint8_t name_length;
	in_stream.Read(name_length);
	std::cout << "Name length" << std::endl;
	in_stream.ReadBits(&m_name, (name_length * 8));
	std::cout << "Name" << std::endl;
	in_stream.Read(m_weapons);
	std::cout << "Weapons" << std::endl;
	in_stream.ReadPos(m_position);
	std::cout << "Position" << std::endl;
	in_stream.Read(m_rotation);
	std::cout << "Rotation" << std::endl;
}




