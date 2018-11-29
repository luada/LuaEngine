--[[Writen by Luada

集合类,内部元素无序排列,并且唯一.

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
	判断集合是否为空
	--]]
	return (self:size() == 0)
end

--------------------------------------------------------------------------------
function Set:size()
	--[[
	获得集合的大小
	--]]
	return table.size(prop[self].elements)
end

--------------------------------------------------------------------------------
function Set:has(elem)
	--[[
	判断指定元素是否在集合内
	--]]
	return (prop[self].elements[elem] == true)
end

--------------------------------------------------------------------------------
function Set:get(elem)
	--[[
	判断指定元素elem在Set集合体的存在情况
	--]]
	return prop[self].elements[elem]
end

--------------------------------------------------------------------------------
function Set:add(elem)
	--[[
	添加一个指定元素到集合中,如果该元素不存在,则返回true,否则返回false
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
	集合并操作
	@param	set:
	@type	set: object of Set
	@param	return: 并的结果
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
	集合交操作
	@param	set:
	@type	set: object of Set
	@param	return: 交的结果
	@type	return：object of Set
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
	集合差操作
	@param	set:
	@type	set: object of Set
	@param	return: 差的结果
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
	从集合中删除一个指定元素
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
	清空集合
	--]]
	local this = prop[self]
	this.elements = {}
end

--------------------------------------------------------------------------------
function Set:elems()
	--[[
	返回所有元素
	--]]
	local this = prop[self]
	return this.elements
end

--------------------------------------------------------------------------------
function Set:iterator()
	--[[
	返回一个可以遍历所有元素的迭代函数
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
	返回一个可以遍历所有元素的迭代函数,这些元素排序过
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
