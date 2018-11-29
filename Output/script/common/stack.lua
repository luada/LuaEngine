--[[Writen by Luada

�����ṩ����ȳ�����

Exported API:
	Stack(capacity)

	stack:empty()
	stack:size()
	stack:push(elem)
	stack:pop()
	stack:peek()
	stack:search(obj)

Example:

--]]

require "base.class"

local DEFAULT_CAPACITY = 10

Stack = class()

local prop = Property(Stack)
prop:accessor("capacity")

function Stack:__init(capacity)
	local this = prop[self]
	this.capacity = capacity or DEFAULT_CAPACITY
	this.elements = {}
end

function Stack:__release()
	local this = prop[self]
	for k, v in pairs(this.elements) do
		release(v)
	end

	this.elements = nil
end

--------------------------------------------------------------------------------
function Stack:empty()
	--[[
	�ж϶�ջ�Ƿ�Ϊ��
	--]]
	return self:size() == 0
end

--------------------------------------------------------------------------------
function Stack:size()
	--[[
	��ö�ջ�Ĵ�С
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function Stack:push(elem)
	--[[
	�Ѷ���ѹ��ջ��
	--]]
	if self:size() >= self:getCapacity() then
		WARN_MSG("Stack overflow.")
		return false
	end
	table.insert(prop[self].elements, elem)
	return true
end

--------------------------------------------------------------------------------
function Stack:pop()
	--[[
	��ջ�������Ƴ�������
	--]]
	if self:empty() then
		WARN_MSG("Stack is empty.")
		return nil
	end

	return table.remove(prop[self].elements, self:size())
end

--------------------------------------------------------------------------------
function Stack:peek()
	--[[
	�鿴ջ����������Ƴ���
	--]]
	return prop[self].elements[self:size()]
end

--------------------------------------------------------------------------------
function Stack:search(obj)
	--[[
	���ض�����ջ�е�λ��,��1Ϊ����
	--]]
	for i, v in pairs(prop[self].elements) do
		if v == obj then return self:size() - i + 1 end
	end
	return -1
end

--------------------------------------------------------------------------------
function Stack:tostring()
	return toString(prop[self].elements)
end