--[[Created by Luada
组件节点的基类
--]]

ComponentBevNode = class(BevNode)

function ComponentBevNode:__init(name)
	--[[
	初始化
	@param	name: 条件类的名字
	@type	name: string
	--]]
	BevNode.__init(self, name, nil)
end

function ComponentBevNode:__release()
end
