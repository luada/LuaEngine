--[[Writen by Luada

������,�ڲ�Ԫ����������,����Ψһ.

Exported API:
	Set()

	set:empty()
	set:size()
	set:has(elem)
	set:add(elem)
	set:remove(elem)

Example:

--]]

require "base.class"

Set = class()

local prop = Property(Set)

function Set:__init()
	local this = prop[self]
	this.elements = {}
end

function Set:__release()
	local this = prop[self]
	for k, v in pairs(this.elements) do
		release(v)
	end

	this.elements = nil
end

--------------------------------------------------------------------------------
function Set:empty()
	--[[
	�жϼ����Ƿ�Ϊ��
	--]]
	return (self:size() == 0)
end

--------------------------------------------------------------------------------
function Set:size()
	--[[
	��ü��ϵĴ�С
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function Set:has(elem)
	--[[
	�ж�ָ��Ԫ���Ƿ��ڼ�����
	--]]
	return (prop[self].elements[elem] == true)
end

--------------------------------------------------------------------------------
function Set:get(elem)
	--[[
	�ж�ָ��Ԫ��elem��Set������Ĵ������
	--]]
	return prop[self].elements[elem]
end

--------------------------------------------------------------------------------
function Set:add(elem)
	--[[
	���һ��ָ��Ԫ�ص�������,�����Ԫ�ز�����,�򷵻�true,���򷵻�false
	--]]
	if not self:has(elem) then
		prop[self].elements[elem] = true
		return true
	end
	return false
end

--------------------------------------------------------------------------------
function Set:union(set)
	--[[
	���ϲ�����
	@param	set:
	@type	set: object of Set
	@param	return: ���Ľ��
	@type	return: object of Set
	--]]
	local retSet = Set()
	for v in self:iterator() do
		retSet:add(v)
	end
	
	for v in set:iterator() do
		retSet:add(v)
	end
	
	return retSet
end

--------------------------------------------------------------------------------
function Set:join(set)
	--[[
	���Ͻ�����
	@param	set:
	@type	set: object of Set
	@param	return: ���Ľ��
	@type	return��object of Set
	--]]
	local retSet = Set()
	for v0 in self:iterator() do
		for v1 in set:iterator() do
			if v0 == v1 then
				retSet:add(v0)
				break
			end
		end
	end
	return retSet
end

--------------------------------------------------------------------------------
function Set:difference(set)
	--[[
	���ϲ����
	@param	set:
	@type	set: object of Set
	@param	return: ��Ľ��
	@type	return: object of Set
	--]]
	local retSet = Set()
	for v0 in self:iterator() do
		local find = false
		for v1 in set:iterator() do
			if v0 == v1 then
				find = true
				break
			end
		end
		if not find then
			retSet:add(v0)
		end
	end
	return retSet
end

--------------------------------------------------------------------------------
function Set:remove(elem)
	--[[
	�Ӽ�����ɾ��һ��ָ��Ԫ��
	--]]
	local this = prop[self]
	if this.elements[elem] == true then
		this.elements[elem] = nil
		return true
	end
	return false
end

--------------------------------------------------------------------------------
function Set:clear()
	--[[
	��ռ���
	--]]
	local this = prop[self]
	this.elements = {}
end

--------------------------------------------------------------------------------
function Set:elems()
	--[[
	��������Ԫ��
	--]]
	local this = prop[self]
	return this.elements
end

--------------------------------------------------------------------------------
function Set:iterator()
	--[[
	����һ�����Ա�������Ԫ�صĵ�������
	--]]
	local auxTable = {}
	table.foreach(prop[self].elements, function(elem, _)
		table.insert(auxTable, elem)
	end)

	local index = 0
	return function()
		if index < #auxTable then
			index = index + 1
			return auxTable[index]
		end
	end
end

--------------------------------------------------------------------------------
function Set:sortIterator(comparator)
	--[[
	����һ�����Ա�������Ԫ�صĵ�������,��ЩԪ�������
	--]]
	local auxTable = {}
	table.foreach(prop[self].elements, function(elem, _)
		table.insert(auxTable, elem)
	end)

	table.sort(auxTable, comparator)

	local index = 0
	return function()
		if index < #auxTable then
			index = index + 1
			return auxTable[index]
		end
	end
end

--------------------------------------------------------------------------------
function Set:tostring()
	local auxTable = {}
	table.foreach(prop[self].elements, function(elem, _)
		table.insert(auxTable, elem)
	end)
	return toString(auxTable)
end
