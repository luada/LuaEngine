--[[Created by Luada
	满足条件的行为节点搜索跟踪器
--]]

require "common.list"

BevSearchTracer = class(Singleton)

function BevSearchTracer:__init()
	self._process = {}
	self._cur = self._process
	self._stack = List()
	self._stack:pushBack(self._process)
end

function BevSearchTracer:__release()
	self._process = nil
	release(self._stack)
	self._stack = nil
end

function BevSearchTracer:enterSearch(obj)
	--[[
	进入搜索时
	@param	obj: 逻辑关系条件组件
	@type	obj: object of BevNode
	--]]
	if not DebugAISearch then
		return
	end

	local info = string.format("BeginLogic>%s", obj:getName())
	local tbl = {info}
	table.insert(self._cur, tbl)
	self._cur = tbl
	self._stack:pushBack(tbl)
end

function BevSearchTracer:leaveSearch(obj, result)
	--[[
	退出搜索时
	@param	obj: 逻辑关系条件组件
	@type	obj: object of BevNode
	@param	result: 逻辑结果
	@type	result: boolean
	--]]
	if not DebugAISearch then
		return
	end

	self._stack:delete(self._stack:size())
	self._cur = self._stack:get(self._stack:size())
	local info = string.format("EndLogic>%s:%s", obj:getName(), result and "true" or "false")
	table.insert(self._cur, info)
end

function BevSearchTracer:process(name, result)
	--[[
	当前处理的节点名字
	@param	name:
	@type	name: string
	@param	result: 当前搜索结果
	@type	result: boolean
	--]]
	if not DebugAISearch then
		return
	end

	local info = string.format("%s:%s", name, result and "true" or "false")
	table.insert(self._cur, info)
end

function BevSearchTracer:dump()
	--[[
	输出当前结果
	@type	return:
	@type	return: string
	--]]
	if not DebugAISearch then
		return ""
	end

	return serialize(self._process)
end

function BevSearchTracer:clear()
	--[[
	清空数据
	--]]
	if not DebugAISearch then
		return
	end

	self._process = {}
	self._stack:clear()
	self._cur = self._process
	self._stack:pushBack(self._process)
end

bevSearchTracer = BevSearchTracer()