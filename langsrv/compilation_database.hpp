#ifndef CLS_COMPILATION_DATABASE_HPP_INCLUDED
#define CLS_COMPILATION_DATABASE_HPP_INCLUDED

#include <clang/Tooling/JSONCompilationDatabase.h>

namespace cls {

class PathNormalizingCompilationDatabase {
    std::unique_ptr<clang::tooling::CompilationDatabase> _database;

public:
    PathNormalizingCompilationDatabase(std::string filepath) {
        using namespace clang::tooling;
        std::string error;
        _database = JSONCompilationDatabase::loadFromFile(filepath, error);
        if (!_database) {
            throw std::runtime_error{error};
        }
    }
    clang::tooling::CompilationDatabase& underlying() { return *_database; }
    const clang::tooling::CompilationDatabase& underlying() const { return *_database; }
};
}

#endif  // CLS_COMPILATION_DATABASE_HPP_INCLUDED