local M = {}

function M.regCmd(parser)
    local cmd = parser:command("remote", "管理存储库的远程仓库")
    local list = cmd:command("list", "列出所有远程仓库")
    list:flag("-v --verbose", "显示远程仓库的地址")
    local create = cmd:command("create", "创建远程仓库")
    create:argument("name", "远程仓库的名称")
    create:argument("url", "远程仓库的地址")
    parser:command("push", "推送到远程仓库")
end

function M.run(args)
    if args.remote and args.create then
        M.create(args.repo, args.name, args.url)
    elseif args.remote and args.list then
        M.list(args.repo, args.verbose)
    elseif args.push then
        M.push(args.repo)
    end
end

function M.create(repo, name, url)
    repo = git.repository.open(repo)
    local remote = git.remote.create(repo, name, url)
end

function M.list(repo, verbose)
    repo = git.repository.open(repo)
    local remotes = git.remote.list(repo)
    if verbose then
        for index = 1, #remotes do
            local remote = git.remote.lookup(repo, remotes[index])
            local url = git.remote.url(remote)
            if not url then
                url = git.remote.pushurl(remote)
            end
            print(index .. ": " .. remotes[index] .. " " .. url)
        end
    else
        for index = 1, #remotes do
            print(index .. ": " .. remotes[index])
        end
    end
end

function M.push(repo)
    repo = git.repository.open(repo)
    local remote = git.remote.lookup(repo, "origin")
    local opts = git.push.options_init(git.enum.PUSH_OPTIONS_VERSION)
    git.remote.push(remote, "refs/heads/master", opts)
end

return M
