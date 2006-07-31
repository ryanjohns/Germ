#ifndef LIBRARYFILEBUILDER_H_
#define LIBRARYFILEBUILDER_H_

#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#define SEPARATOR (char)6

class LibraryFileBuilder	{
public:
	LibraryFileBuilder();
	virtual ~LibraryFileBuilder();

	void generate_library_file(std::string &, std::string &);

private:
	std::ofstream m_libraryFileStream;

	void process_file(std::string &);
	void recurse_directory(std::string &);

	// wrapper functions
	static inline DIR * Opendir(const char * name) {
		DIR * dir;
		if ((dir = opendir(name)) == NULL) {
			perror("opendir() error!");
			exit(EXIT_FAILURE);
		}
		return dir;
	}

	static inline int Closedir(DIR * dir) {
		int rc;
		if ((rc = closedir(dir)) == -1) {
			perror("closedir() error!");
			exit(EXIT_FAILURE);
		}
		return rc;
	}

	static inline struct dirent * Readdir(DIR * dir) {
		struct dirent * ret;
		if ((ret = readdir(dir)) == NULL) {
			if (errno == EBADF) {
				perror("readdir() error!");
				exit(EXIT_FAILURE);
			}
		}
		return ret;
	}

	static inline int Stat(const char * path, struct stat * buf) {
		int rc;
		if ((rc = stat(path, buf)) == -1) {
			perror("stat() error!");
			exit(EXIT_FAILURE);
		}
		return rc;
	}
};

#endif /*LIBRARYFILEBUILDER_H_*/
