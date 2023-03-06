local lfs = require "lfs"

local M = {}

function M.init(repo, isBare)
    print("Initialize the repository:", repo)
    git.repository.init(repo, isBare)
end

function M.add(repo, file)
    repo = git.repository.open(repo)
    local index = git.repository.index(repo)
    git.index.add_bypath(index, file)
    git.index.write(index)
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

M.remote = {}
function M.remote.create(repo, name, url)
    repo = git.repository.open(repo)
    local remote = git.remote.create(repo, name, url)
end

function M.remote.list(repo, verbose)
    repo = git.repository.open(repo)
    local remotes = git.remote.list(repo)
    if verbose then
        for index, iterm in pairs(remotes) do
            local remote = git.remote.lookup(repo, iterm)
            local url = git.remote.url(remote)
            if not url then
                url = git.remote.pushurl(remote)
            end
            print(index .. ": " .. iterm .. " " .. url)
        end
    else
        for index, iterm in pairs(remotes) do
            print(index .. ": " .. iterm)
        end
    end
end

function M.remote.push(repo)
    repo = git.repository.open(repo)
    local remote = git.remote.lookup(repo, "origin")
    local opts = git.push.options_init(git.enum.PUSH_OPTIONS_VERSION)
    git.remote.push(remote, "refs/heads/master", opts)
end

return M
