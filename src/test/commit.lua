local M = {}

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
