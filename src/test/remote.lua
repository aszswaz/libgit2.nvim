local M = {}

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
