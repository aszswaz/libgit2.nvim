-- 在 lua 库搜索路径中添加脚本的所在目录
local scriptPath = arg[0]:match ".*/"
package.cpath = package.cpath .. ";" .. scriptPath .. "?.so"
package.path = package.path .. ";" .. scriptPath .. "?.lua"

if not git then
    git = require "libgit2"
end

local MODULES = {
    require "info",
    require "option",
    require "status",
    require "remote",
    require "repoistory",
    require "index",
    require "commit",
}

local argparse = require "argparse"
local lfs = require "lfs"

local M = {}

local parser = argparse("libgit2-nvim-test", "libgit2-nvim 的测试脚本")
parser:option("-C --repo", "存储库的路径，默认是工作目录")

for _, module in pairs(MODULES) do
    module.regCmd(parser)
end

local args = parser:parse()

if not args.repo then
    args.repo = lfs.currentdir()
end

git.init()
for _, module in pairs(MODULES) do
    if module.run(args) then
        break
    end
end
git.shutdown()
