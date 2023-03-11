local M = {}

function M.regCmd(parser)
    local cmd = parser:command("commit", "记录对存储库的更改")
    cmd:option("-m --message", "提交信息")
end

function M.run(args)
    if not args.commit then
        return
    end
    M.commit(args.repo, args.message)
end

function M.commit(repo, msg)
    repo = git.repository.open(repo)
    local parent, ref = git.revparse.ext(repo, "HEAD")
    local indexHandle = git.repository.index(repo)
    local treeOid = git.index.write_tree(indexHandle)
    git.index.write(indexHandle)
    local treeHandle = git.tree.lookup(repo, treeOid)
    local sign = git.signature.default(repo)
    local commitId = git.commit.create(repo, "HEAD", sign, sign, nil, msg, treeHandle, { parent })
end

return M
