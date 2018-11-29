--[[Writen by Luada

�����ṩ�Ƚ��ȳ�����

Exported API:
	Queue(capacity)

	queue:empty()
	queue:size()
	queue:element()
	queue:peek()
	queue:offer(elem)
	queue:remove()
	queue:poll()

Example:

--]]

require "base.class"

local DEFAULT_CAPACITY = 10

Queue = class()

local prop = Property(Queue)
prop:reader("elements")
prop:accessor("capacity")

function Queue:__init(capacity)
	local this = prop[self]
	this.capacity = capacity or DEFAULT_CAPACITY
	this.elements = {}
end

function Queue:__release()
	local this = prop[self]
	for k, v in pairs(this.elements) do
		release(v)
	end

	this.elements = nil
end

--------------------------------------------------------------------------------
function Queue:empty()
	--[[
	�ж϶����Ƿ�Ϊ��
	--]]
	return self:size() == 0
end

--------------------------------------------------------------------------------
function Queue:full()
	--[[
	�ж϶����Ƿ�����
	--]]
	return self:size() == self:getCapacity()
end

--------------------------------------------------------------------------------
function Queue:size()
	--[[
	��ö��еĴ�С
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function Queue:element()
	--[[
	����,���ǲ��Ƴ��˶��е�ͷ
	--]]
	if self:empty() then
		error("Queue is empty.")
	end
	return self:peek()
end

--------------------------------------------------------------------------------
function Queue:peek()
	--[[
	����,���ǲ��Ƴ��˶��е�ͷ,����˶���Ϊ��,�򷵻�nil
	--]]
	return prop[self].elements[1]
end

--------------------------------------------------------------------------------
function Queue:offer(elem)
	--[[
	�������,��ָ����Ԫ�ز���˶���
	--]]
	if self:size() >= self:getCapacity() then
		WARN_MSG("Queue overflow.")
		return false
	end
	table.insert(prop[self].elements, elem)
	return true
end

--------------------------------------------------------------------------------
function Queue:remove()
	--[[
	�������Ƴ��˶��е�ͷ
	--]]
	if self:empty() then
		error("Queue is empty.")
	end
	return self:poll()
end

--------------------------------------------------------------------------------
function Queue:poll()
	--[[
	�������Ƴ��˶��е�ͷ,����˶���Ϊ��,�򷵻�nil
	--]]
	return table.remove(prop[self].elements, 1)
end

--------------------------------------------------------------------------------
function Queue:tostring()
	return toString(prop[self].elements)
end