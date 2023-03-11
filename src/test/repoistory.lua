local M = {}

function M.regCmd(parser)
    local cmd = parser:command("init", "初始化存储库")
    cmd:flag("--bare", "创建裸存储库")
end

function M.run(args)
    if not args.init then
        return
    end
    M.init(args.repo, args.bare)
end

function M.init(repo, isBare)
    print("Initialize the repository:", repo)
    git.repository.init(repo, isBare)
end

return M
