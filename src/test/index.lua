local M = {}

function M.add(repo, file)
    repo = git.repository.open(repo)
    local index = git.repository.index(repo)
    git.index.add_bypath(index, file)
    git.index.write(index)
end

return M
