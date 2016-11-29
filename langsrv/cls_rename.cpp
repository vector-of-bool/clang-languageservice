#include "language_service.hpp"

#include "opt_bind.hpp"

#include "types.hpp"

#include "compilation_database.hpp"

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Frontend/ASTUnit.h>

using namespace cls;
using namespace langsrv;
using namespace langsrv::optional_bind_op;

namespace cl = clang::tooling;

future<WorkspaceEdit> LanguageService::rename(const RenameParams& params) {
    const auto uri = params.textDocument.uri;
    GetCompilationInfoParams req;
    req.uri = params.textDocument.uri;
    return getCompilationDatabasePath().then([=](future<GetCompilationDatabasePathResult> fut) {
        auto res = fut.get();
        if (!res.filepath) {
            _show_message(MessageType::Error, "Rename failed: Cannot find compilation database");
        }

        const auto db = PathNormalizingCompilationDatabase(*res.filepath);
        cl::ClangTool tool(db.underlying(), db.underlying().getAllFiles());
        std::vector<std::unique_ptr<clang::ASTUnit>> tus;
        tool.buildASTs(tus);
        for (auto& unit_ptr : tus) {
            if (!unit_ptr) continue;
            auto& tu = *unit_ptr;
            auto& sm = tu.getSourceManager();
            auto& fm = tu.getFileManager();
            auto file_path = uri.find("file://") != uri.npos ? uri.substr(7) : uri;
            auto file_entry = fm.getFile(file_path);
            if (!file_entry) {
                _show_message(MessageType::Warning, "Unable to find file: ", file_path);
            }

            auto loc = tu.getLocation(file_entry, params.position.line, params.position.character);
            _show_message(MessageType::Info, "Found source location");
        }
        _show_message(MessageType::Info, "Loaded compilation database");
        return WorkspaceEdit{};
    });
}