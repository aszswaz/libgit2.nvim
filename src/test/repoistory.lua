local M = {}

function M.init(repo, isBare)
    print("Initialize the repository:", repo)
    git.repository.init(repo, isBare)
end

return M
