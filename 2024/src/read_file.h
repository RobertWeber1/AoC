
// #include <iostream>
// #include <ranges>
#include <string>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define THROW_SYSTEM_ERROR \
 throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)))


struct FileDescriptor
{
	FileDescriptor(FileDescriptor const &) = delete;
	FileDescriptor(FileDescriptor &&) = delete;
	void operator=(FileDescriptor const &) = delete;
	void operator=(FileDescriptor &&x) = delete;

	FileDescriptor(std::string const& input)
	: fd_(open(input.c_str(), O_RDONLY))
	{
		if(fd_ == -1)
		{
			THROW_SYSTEM_ERROR;
		}
	}

	~FileDescriptor()
	{
		if(fd_ != -1)
		{
			close(fd_);
		}
	}

	int get() const
	{
		return fd_;
	}

private:
	int fd_ = -1;
};


struct MemoryMappedFile
{
	MemoryMappedFile(MemoryMappedFile const &) = delete;
	MemoryMappedFile(MemoryMappedFile &&) = delete;
	void operator=(MemoryMappedFile const &) = delete;
	void operator=(MemoryMappedFile &&x) = delete;


	MemoryMappedFile(FileDescriptor const& fd)
	{
		struct stat file_stats;
		if (fstat(fd.get(), &file_stats) == -1)
		{
			THROW_SYSTEM_ERROR;
		}

		size_ = file_stats.st_size;
		data_ = mmap(
			nullptr,
			size_,
			PROT_READ,
			MAP_PRIVATE,
			fd.get(),
			0);

		if(data_ == MAP_FAILED)
		{
			THROW_SYSTEM_ERROR;
		}
	}

	~MemoryMappedFile()
	{
		if(data_ != MAP_FAILED)
		{
			munmap(data_, size_);
		}
	}

	char const* data() const
	{
		return reinterpret_cast<char const*>(data_);
	}

	size_t size() const
	{
		return size_;
	}

	char const* begin() const
	{
		return data();
	}

	char const* end() const
	{
		return data() + size();
	}

	std::string_view get() const
	{
		return std::string_view(begin(), end());
	}
private:
	void* data_ = MAP_FAILED;
	size_t size_ = 0;
};
