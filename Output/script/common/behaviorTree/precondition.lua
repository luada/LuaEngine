--[[Created by Luada
行为树的前提条件类
--]]

BevPrecondition = class()

function BevPrecondition:__init(name) 
	--[[
	初始化
	@param	name: 条件类的名字
	@type	name: string
	--]]
	self._objName = name
end

function BevPrecondition:getName()
	--[[
	获取名字
	@param	return：
	@type	return：string
	--]]
	return self._objName
end

function BevPrecondition:__release()
end

function BevPrecondition:load(...)
end

function BevPrecondition:test(input)
end
