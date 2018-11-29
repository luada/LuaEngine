--[[Writen by Luada

列表类

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
	判断列表是否为空
	--]]
	return (self:size() == 0)
end

--------------------------------------------------------------------------------
function List:size()
	--[[
	获得列表的大小
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function List:has(elem)
	--[[
	判断指定元素是否在列表内
	@param	elem:
	@type	elem: any
	--]]
	return (self:indexOf(elem) ~= -1)
end

--------------------------------------------------------------------------------
function List:indexOf(elem)
	--[[
	获得指定元素的索引
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
	获得指定元素
	--]]
	return prop[self].elements[index]
end

--------------------------------------------------------------------------------
function List:pushBack(elem)
	--[[
	在列表尾部添加一个元素
	--]]
	table.insert(prop[self].elements, elem)
end

--------------------------------------------------------------------------------
function List:pushFront(elem)
	--[[
	在列表头部添加一个元素
	--]]
	table.insert(prop[self].elements, 1, elem)
end

--------------------------------------------------------------------------------
function List:remove(elem)
	--[[
	从列表中删除一个指定元素
	--]]	
	local index = self:indexOf(elem)
	self:delete(index)
	return self
end

--------------------------------------------------------------------------------
function List:delete(index)
	--[[
	从列表中删除一个指定位置元素
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
	清空列表
	--]]
	local this = prop[self]
	this.elements = {}
end

--------------------------------------------------------------------------------
function List:elems()
	--[[
	返回所有元素
	--]]
	local this = prop[self]
	return this.elements
end

--------------------------------------------------------------------------------
function List:iterator()
	--[[
	返回一个可以遍历所有元素的迭代函数
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
	返回一个可以遍历所有元素被排序后的迭代函数
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