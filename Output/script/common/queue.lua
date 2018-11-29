--[[Writen by Luada

该类提供先进先出操作

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
	判断队列是否为空
	--]]
	return self:size() == 0
end

--------------------------------------------------------------------------------
function Queue:full()
	--[[
	判断队列是否已满
	--]]
	return self:size() == self:getCapacity()
end

--------------------------------------------------------------------------------
function Queue:size()
	--[[
	获得队列的大小
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function Queue:element()
	--[[
	检索,但是不移除此队列的头
	--]]
	if self:empty() then
		error("Queue is empty.")
	end
	return self:peek()
end

--------------------------------------------------------------------------------
function Queue:peek()
	--[[
	检索,但是不移除此队列的头,如果此队列为空,则返回nil
	--]]
	return prop[self].elements[1]
end

--------------------------------------------------------------------------------
function Queue:offer(elem)
	--[[
	如果可能,将指定的元素插入此队列
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
	检索并移除此队列的头
	--]]
	if self:empty() then
		error("Queue is empty.")
	end
	return self:poll()
end

--------------------------------------------------------------------------------
function Queue:poll()
	--[[
	检索并移除此队列的头,如果此队列为空,则返回nil
	--]]
	return table.remove(prop[self].elements, 1)
end

--------------------------------------------------------------------------------
function Queue:tostring()
	return toString(prop[self].elements)
end