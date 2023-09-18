#include <NGIN/Meta/UUID.hpp>
#include <random>
#include <sstream>
#include <iomanip>
namespace NGIN::Meta
{
    UUID::UUID()
        : uuid(GenerateUUID())
    {
    }

    UUID::UUID(uint64_t uuid)
        : uuid(uuid)
    {
    }

    std::string UUID::ToString() const
    {
        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << uuid;
        return ss.str();
    }

    bool UUID::operator==(const UUID &other) const
    {
        return uuid == other.uuid;
    }

    bool UUID::operator!=(const UUID &other) const
    {
        return uuid != other.uuid;
    }

    uint64_t UUID::GenerateUUID()
    {
        static std::random_device rd;
        static std::mt19937_64 generator(rd());
        static std::uniform_int_distribution<uint64_t> distribution;
        return distribution(generator);
    }
}