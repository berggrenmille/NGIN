#include <cstdint>
#include <string>

namespace NGIN::Meta
{
    /// \brief A class for representing universally unique identifiers (UUIDs).
    class UUID
    {
    public:
        /// \brief Default constructor. Generates a new UUID.
        UUID();

        /// \brief Constructor that initializes the UUID with a given value.
        /// \param uuid The 64-bit value to use as the UUID.
        explicit UUID(uint64_t uuid);

        /// \brief Returns the UUID as a hexadecimal string.
        /// \return The UUID represented as a string.
        std::string ToString() const;

        /// \brief Compares two UUIDs for equality.
        /// \param other The UUID to compare with.
        /// \return true if the UUIDs are equal, false otherwise.
        bool operator==(const UUID &other) const;

        /// \brief Compares two UUIDs for inequality.
        /// \param other The UUID to compare with.
        /// \return true if the UUIDs are not equal, false otherwise.
        bool operator!=(const UUID &other) const;

    private:
        /// \brief The 64-bit representation of the UUID.
        uint64_t uuid;

        /// \brief Generates a random 64-bit UUID.
        /// \return A random 64-bit value.
        static uint64_t GenerateUUID();
    };
}
