local M = {}

function M.regCmd(parser)
    local cmd = parser:command("add", "把指定的文件添加到存储库")
    cmd:argument("file", "需要添加到存储库的文件")
end

function M.run(args)
    if not args.add then
        return
    end
    M.add(args.repo, args.file)
end

function M.add(repo, file)
    repo = git.repository.open(repo)
    local index = git.repository.index(repo)
    git.index.add_bypath(index, file)
    git.index.write(index)
end

return M
