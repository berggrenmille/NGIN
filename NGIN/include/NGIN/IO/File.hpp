#pragma once
#include <NGIN/Meta/StoragePolicy/DynamicStorage.hpp>
#include <NGIN/Memory/Allocator.hpp>

#include <memory>
#include <type_traits>
#include <utility>

namespace NGIN::IO
{
    /// \namespace FileConcepts
    /// \brief Namespace for File-related concepts.
    namespace FileConcepts
    {
        /// \concept HasSeek
        /// \brief A concept that checks if a type T has a Seek method.
        /// \tparam T The type to check.
        template <typename T>
        concept HasSeek = requires(T a, std::streampos position) {
            {
                a.Seek(position)
            } -> std::same_as<void>;
        };

        /// \concept HasOpen
        /// \brief A concept that checks if a type T has an Open method.
        /// \tparam T The type to check.
        template <typename T>
        concept HasOpen = requires(T a, const std::string &filename) {
            {
                a.Open(filename)
            } -> std::same_as<bool>;
        };

        /// \concept HasRead
        /// \brief A concept that checks if a type T has a Read method.
        /// \tparam T The type to check.
        template <typename T>
        concept HasRead = requires(T a, char *buffer, std::size_t size) {
            {
                a.Read(buffer, size)
            } -> std::same_as<std::size_t>;
        };

        /// \concept HasWrite
        /// \brief A concept that checks if a type T has a Write method.
        /// \tparam T The type to check.
        template <typename T>
        concept HasWrite = requires(T a, const char *data, std::size_t size) {
            {
                a.Write(data, size)
            } -> std::same_as<std::size_t>;
        };

        /// \concept HasClose
        /// \brief A concept that checks if a type T has a Close method.
        /// \tparam T The type to check.
        template <typename T>
        concept HasClose = requires(T a) {
            {
                a.Close()
            } -> std::same_as<void>;
        };

    } // namespace FileConcepts

    /// \class File
    /// \tparam StoragePolicy The storage policy to use, defaults to NGIN::TypeErasure::DynamicStoragePolicy.
    /// \brief A class representing a generic File object.
    ///
    /// Example usage:
    /// \code{.cpp}
    /// class MyFileClass {
    /// public:
    ///     std::size_t Read(char* buffer, std::size_t size) {
    ///         // Implement your reading logic here
    ///         return size; // Return the number of bytes read
    ///     }
    ///
    ///     std::size_t Write(const char* data, std::size_t size) {
    ///         // Implement your writing logic here
    ///         return size; // Return the number of bytes written
    ///     }
    ///
    ///     void Close() {
    ///         // Implement your closing logic here
    ///     }
    /// };
    ///
    /// int main() {
    ///     MyFileClass myFile;
    ///     NGIN::IO::File<> file(std::move(myFile));
    ///
    ///     char buffer[100];
    ///     file.Read(buffer, 100);
    ///     file.Write("Hello, world!", 13);
    ///     file.Close();
    ///
    ///     return 0;
    /// }
    /// \endcode
    template <typename StoragePolicy = TypeErasure::DynamicStoragePolicy>
    class File
    {
    public:
        /// \brief Constructor using a file object.
        /// \tparam T The type of the file object.
        /// \param file The file object.
        template <typename T>
        File(T &&file)
            : pImpl(std::move(file))
        {
            SetupFunctionPointers<T>();
        }

        /// \brief Constructor using a file object and a custom allocator.
        /// \tparam T The type of the file object.
        /// \param file The file object.
        /// \param allocator The custom allocator to use.
        template <typename T>
        File(T &&file, Memory::Allocator<> &allocator)
            : pImpl(std::move(file), allocator)
        {
            SetupFunctionPointers<T>();
        }

        /// \brief Reads from the file.
        /// \param buffer The buffer to read into.
        /// \param size The number of bytes to read.
        /// \return The number of bytes read.
        std::size_t Read(char *buffer, std::size_t size)
        {
            return read(pImpl.get(), buffer, size);
        }

        /// \brief Writes to the file.
        /// \param data The data to write.
        /// \param size The number of bytes to write.
        /// \return The number of bytes written.
        std::size_t Write(const char *data, std::size_t size)
        {
            return write(pImpl.get(), data, size);
        }

        /// \brief Seeks to a specific position in the file.
        /// \param position The position to seek to.
        void Seek(std::streampos position)
        {
            seek(pImpl.get(), position);
        }

        /// \brief Opens a file.
        /// \param filename The name of the file to open.
        /// \return True if the file was successfully opened, false otherwise.
        bool Open(const std::string &filename);
        {
            return open(pImpl.get(), filename);
        }

        /// \brief Closes the file.
        void Close()
        {
            close(pImpl.get());
        }

    private:
        /// \brief Storage policy implementation.
        StoragePolicy pImpl;

        /// \brief Function pointer for seeking.
        void (*seek)(void *, std::streampos) = nullptr;

        /// \brief Function pointer for opening.
        bool (*open)(void *, const std::string &) = nullptr;

        /// \brief Function pointer for reading.
        std::size_t (*read)(void *, char *, std::size_t) = nullptr;

        /// \brief Function pointer for writing.
        std::size_t (*write)(void *, const char *, std::size_t) = nullptr;

        /// \brief Function pointer for closing.
        void (*close)(void *) = nullptr;

        /// \brief Sets up function pointers based on the type T.
        /// \tparam T The type to set up function pointers for.
        template <typename T>
        void SetupFunctionPointers()
        {
            seek = [](void *obj, std::streampos position)
            {
                if constexpr (FileConcepts::HasSeek<T>)
                {
                    static_cast<T *>(obj)->Seek(position);
                }
            };
            open = [](void *obj, const std::string &filename)
            {
                if constexpr (FileConcepts::HasOpen<T>)
                {
                    return static_cast<T *>(obj)->Open(filename);
                }
                return false;
            };
            read = [](void *obj, char *buffer, std::size_t size)
            {
                if constexpr (FileConcepts::HasRead<T>)
                {
                    return static_cast<T *>(obj)->Read(buffer, size);
                }
                return static_cast<std::size_t>(0);
            };
            write = [](void *obj, const char *data, std::size_t size)
            {
                if constexpr (FileConcepts::HasWrite<T>)
                {
                    return static_cast<T *>(obj)->Write(data, size);
                }
                return static_cast<std::size_t>(0);
            };
            close = [](void *obj)
            {
                if constexpr (FileConcepts::HasClose<T>)
                {
                    static_cast<T *>(obj)->Close();
                }
            };
        }
    };
}
