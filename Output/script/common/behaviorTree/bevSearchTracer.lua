--[[Created by Luada
	������������Ϊ�ڵ�����������
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
	��������ʱ
	@param	obj: �߼���ϵ�������
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
	�˳�����ʱ
	@param	obj: �߼���ϵ�������
	@type	obj: object of BevNode
	@param	result: �߼����
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
	��ǰ����Ľڵ�����
	@param	name:
	@type	name: string
	@param	result: ��ǰ�������
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
	�����ǰ���
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
	�������
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