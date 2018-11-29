--[[Writen by Luada

�б���

Exported API:
	List()

	list:empty()
	list:size()
	list:has(elem)
	list:indexOf(elem)
	list:get(index)
	list:pushBack(elem)
	list:pushFront(elem)
	list:remove(elem)
	list:delete(index)

Example:

--]]

require "base.class"

List = class()

local prop = Property(List)

function List:__init()
	local this = prop[self]
	this.elements = {}
end

function List:__release()
	local this = prop[self]
	for k, v in pairs(this.elements) do
		release(v)
	end

	this.elements = nil
end

--------------------------------------------------------------------------------
function List:empty()
	--[[
	�ж��б��Ƿ�Ϊ��
	--]]
	return (self:size() == 0)
end

--------------------------------------------------------------------------------
function List:size()
	--[[
	����б�Ĵ�С
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function List:has(elem)
	--[[
	�ж�ָ��Ԫ���Ƿ����б���
	@param	elem:
	@type	elem: any
	--]]
	return (self:indexOf(elem) ~= -1)
end

--------------------------------------------------------------------------------
function List:indexOf(elem)
	--[[
	���ָ��Ԫ�ص�����
	--]]
	local elements = prop[self].elements
	for k, v in pairs(elements) do
		if v == elem then
			return k
		end
	end
	return -1
end

--------------------------------------------------------------------------------
function List:get(index)
	--[[
	���ָ��Ԫ��
	--]]
	return prop[self].elements[index]
end

--------------------------------------------------------------------------------
function List:pushBack(elem)
	--[[
	���б�β�����һ��Ԫ��
	--]]
	table.insert(prop[self].elements, elem)
end

--------------------------------------------------------------------------------
function List:pushFront(elem)
	--[[
	���б�ͷ�����һ��Ԫ��
	--]]
	table.insert(prop[self].elements, 1, elem)
end

--------------------------------------------------------------------------------
function List:remove(elem)
	--[[
	���б���ɾ��һ��ָ��Ԫ��
	--]]	
	local index = self:indexOf(elem)
	self:delete(index)
	return self
end

--------------------------------------------------------------------------------
function List:delete(index)
	--[[
	���б���ɾ��һ��ָ��λ��Ԫ��
	--]]
	local elements = prop[self].elements
	local elem = elements[index]
	if elem then
		table.remove(elements, index)
	end
	return elem
end

--------------------------------------------------------------------------------
function List:clear()
	--[[
	����б�
	--]]
	local this = prop[self]
	this.elements = {}
end

--------------------------------------------------------------------------------
function List:elems()
	--[[
	��������Ԫ��
	--]]
	local this = prop[self]
	return this.elements
end

--------------------------------------------------------------------------------
function List:iterator()
	--[[
	����һ�����Ա�������Ԫ�صĵ�������
	--]]
	local elements = prop[self].elements

	local index = 0
	return function()
		if index < #elements then
			index = index + 1
			return elements[index]
		end
	end
end

--------------------------------------------------------------------------------
function List:sortIterator(comparator)
	--[[
	����һ�����Ա�������Ԫ�ر������ĵ�������
	--]]
	local auxTable = {}
	table.foreach(prop[self].elements, 
		function(_, elem)
			table.insert(auxTable, elem)
		end
		)
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
function List:tostring()
	local auxTable = {}
	table.foreach(prop[self].elements, 
		function(_, elem)
			table.insert(auxTable, elem)
		end
		)
	return toString(auxTable)
end